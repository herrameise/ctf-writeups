## BIGKEY [80]
**Tags**: crackme

**Description**: [This binary](bigkey) (also available at `/home/problems/b3e2e71f05b033748e4fc3a0d6af6dd9/bigkey_9` on the shell server) seems to want a *really* large key. Can you find the flag it accepts?

## Solution
This binary does some math with three 4096-byte hard-coded blocks and compares the result to 4096 bytes read in from a key file whose name the user supplies as `argv[1]` on the command line.

The first thing that happens in `main()` is these blocks being copied from the `.data` segment to buffers on the stack. They are copied DWORD by DWORD (4 bytes at a time) with the x86 instruction `rep movsd`. The implied counter register `ecx` is initialized to `0x400` (1024) and counts down to zero, decrementing by one every time `rep movsd` is executed. This will copy all 4096 bytes are copied for each block (1024 iterations x 4 bytes/iteration = 4096 bytes):
```
   0x080484a1 <+17>:	mov    esi,0x80488c0
   0x080484a6 <+22>:	lea    edi,[ebp-0x401c]
   [...]
   0x080484c3 <+51>:	mov    cx,0x400
   [...]
   0x080484ca <+58>:	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
   0x080484cc <+60>:	mov    esi,0x80498c0
   0x080484d1 <+65>:	lea    edi,[ebp-0x301c]
   0x080484d7 <+71>:	mov    cx,0x400
   0x080484db <+75>:	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
   0x080484dd <+77>:	mov    esi,0x804a8c0
   0x080484e2 <+82>:	lea    edi,[ebp-0x201c]
   0x080484e8 <+88>:	mov    cx,0x400
   0x080484ec <+92>:	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
```

The blocks are located at `0x80488c0`, `0x080498c0`, and `0x0804a8c0`. We will call these blocks `block1`, `block2`, and `block3`, respectively, and they are copied to `[ebp-0x401c]`, `[ebp-0x301c]`, and `[ebp-0x201c]` on the stack, respectively.

Then, the binary uses `fopen()` to open the file specified by the user on the command line in `argv[1]`. If this open is successful, it reads one byte at a time into `[ebp-0x101c]` until 4096 bytes have been read:
```
   0x08048511 <+129>:	lea    ebx,[ebp-0x101c]
   0x08048517 <+135>:	lea    edi,[ebp-0x1c]
   0x0804851a <+138>:	jmp    0x8048527 <main+151>
   
   0x0804851c <+140>:	lea    esi,[esi+eiz*1+0x0]        ; this is just an Intel-ism ... don't be scared
   0x08048520 <+144>:	add    ebx,0x1
   0x08048523 <+147>:	cmp    ebx,edi
   0x08048525 <+149>:	je     0x804856a <main+218>
   
   0x08048527 <+151>:	push   esi
   0x08048528 <+152>:	push   0x1
   0x0804852a <+154>:	push   0x1
   0x0804852c <+156>:	push   ebx
   0x0804852d <+157>:	call   0x8048430 <fread@plt>
   0x08048532 <+162>:	add    esp,0x10
   0x08048535 <+165>:	test   eax,eax
   0x08048537 <+167>:	jne    0x8048520 <main+144>
```

After all 4096 bytes have been read in from the user's file, the binary does some operations with the three hard-coded blocks and then a comparison:
```
   0x08048570 <+224>:	add    eax,0x1
   0x08048573 <+227>:	cmp    eax,0x1000
   0x08048578 <+232>:	je     0x80485c5 <main+309>
   
   0x0804857a <+234>:	movzx  edx,BYTE PTR [ebp+eax*1-0x301c]
   0x08048582 <+242>:	xor    dl,BYTE PTR [ebp+eax*1-0x401c]
   0x08048589 <+249>:	add    dl,BYTE PTR [ebp+eax*1-0x201c]
   
   0x08048590 <+256>:	cmp    BYTE PTR [ebp+eax*1-0x101c],dl
   0x08048597 <+263>:	je     0x8048570 <main+224>
```

We can substitute these stack variable offsets with the names to make the disassembly clearer:
```
   0x08048570 <+224>:	add    eax,0x1
   0x08048573 <+227>:	cmp    eax,0x1000
   0x08048578 <+232>:	je     0x80485c5 <main+309>
   
   0x0804857a <+234>:	movzx  edx,{block2[eax]}
   0x08048582 <+242>:	xor    dl,{block1[eax]}
   0x08048589 <+249>:	add    dl,{block3[eax]}
   
   0x08048590 <+256>:	cmp    {userblock[eax]},dl
   0x08048597 <+263>:	je     0x8048570 <main+224>
```

This is equivalent to the C code:
```C
int ctr;
for (ctr = 0; ctr < 0x1000; ctr++) {
  if ( ((block2[ctr] ^ block1[ctr]) + block3[ctr]) != userblock[ctr] ) failure();
}
success();
```

Now that we know how the user's block is checked, we need to extract each of the three hard-coded blocks out to disk:
```
(gdb) break main
Breakpoint 1 at 0x80484a1
(gdb) run
Starting program: /home/ameise/CS2015/re/080_BIGKEY/bigkey 

Breakpoint 1, 0x080484a1 in main ()
(gdb) dump binary memory block1 0x080488c0 0x080498c0
(gdb) dump binary memory block2 0x080498c0 0x0804a8c0
(gdb) dump binary memory block3 0x0804a8c0 0x0804b8c0
(gdb) quit
A debugging session is active.

	Inferior 1 [process 7500] will be killed.

Quit anyway? (y or n) y
ameise@STAHLKERN:~/CS2015/re/080_BIGKEY$
ameise@STAHLKERN:~/CS2015/re/080_BIGKEY$ ls
bigkey	block1	block2	block3
```

Once we have these blocks, we can write a [keygen.c](keygen.c) to create a user block that the binary will accept:
```
ameise@STAHLKERN:~/CS2015/re/080_BIGKEY$ gcc keygen.c -o keygen
ameise@STAHLKERN:~/CS2015/re/080_BIGKEY$ ./keygen 
ameise@STAHLKERN:~/CS2015/re/080_BIGKEY$ ls
bigkey	block1	block2	block3	keygen	keygen.c  userblock
ameise@STAHLKERN:~/CS2015/re/080_BIGKEY$ ./bigkey userblock 
um_i_think_i_prefer_smaller_keys_fBex0Lrzcq8KOqymNMvPQCnMik29G9B
```