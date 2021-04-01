## CRACKME 0 [70]
**Tags**: crackme

**Description**: Find a valid key for [crackme_0](crackme_0). Source code for every function besides `decrypt()` is available at [crackme_0_empty.c](crackme_0_empty.c). The binary is also available on the shell servers at `/home/problems/98f4140a86899eaa07c0b1ed0e913a1c/crackme-0_4`.

## Solution:
This binary takes a *hexadecimal string* from the command line, converts it to a global *byte array* called `buffer[]` in `fromhex()`, runs `buffer[]` through a function called `decrypt()`, and then compares `buffer[]` with `strncmp()` to a global array called `secret[]`. Looking at `fromhex()` tells us that the hex string supplied on the command line must be 2 * `SECSIZE` or 2 * 16 = 32:
```
ameise@STAHLKERN:~/CS2015/re/070_CRACKME_0$ gdb ./crackme_0 -q
Reading symbols from ./crackme_0...(no debugging symbols found)...done.
(gdb) disas decrypt 
Dump of assembler code for function decrypt:
   0x0804850b <+0>:	    push   ebp
   0x0804850c <+1>:	    mov    ebp,esp
   0x0804850e <+3>:	    sub    esp,0x10
   0x08048511 <+6>:	    mov    DWORD PTR [ebp-0x4],0x0
   0x08048518 <+13>:	jmp    0x8048559 <decrypt+78>
   0x0804851a <+15>:	mov    eax,0xf
   0x0804851f <+20>:	sub    eax,DWORD PTR [ebp-0x4]
   0x08048522 <+23>:	movzx  eax,BYTE PTR [eax+0x804a057]
   0x08048529 <+30>:	mov    BYTE PTR [ebp-0x5],al
   0x0804852c <+33>:	mov    eax,0xf
   0x08048531 <+38>:	sub    eax,DWORD PTR [ebp-0x4]
   0x08048534 <+41>:	mov    edx,DWORD PTR [ebp-0x4]
   0x08048537 <+44>:	add    edx,0x804a057
   0x0804853d <+50>:	movzx  edx,BYTE PTR [edx]
   0x08048540 <+53>:	mov    BYTE PTR [eax+0x804a057],dl
   0x08048546 <+59>:	mov    eax,DWORD PTR [ebp-0x4]
   0x08048549 <+62>:	lea    edx,[eax+0x804a057]
   0x0804854f <+68>:	movzx  eax,BYTE PTR [ebp-0x5]
   0x08048553 <+72>:	mov    BYTE PTR [edx],al
   0x08048555 <+74>:	add    DWORD PTR [ebp-0x4],0x1
   0x08048559 <+78>:	cmp    DWORD PTR [ebp-0x4],0x7
   0x0804855d <+82>:	jle    0x804851a <decrypt+15>
   0x0804855f <+84>:	leave  
   0x08048560 <+85>:	ret    
End of assembler dump.
(gdb) 

```

The stack variable `[ebp-0x4]` is used as a counter to create a control flow structure that might have been a `for` loop in the C source code.
```
   0x08048511 <+6>:	    mov    DWORD PTR [ebp-0x4],0x0
   [...]
   0x08048555 <+74>:	add    DWORD PTR [ebp-0x4],0x1
   0x08048559 <+78>:	cmp    DWORD PTR [ebp-0x4],0x7
   0x0804855d <+82>:	jle    0x804851a <decrypt+15>
```

We will call this stack variable `ctr`. It looks like `BYTE PTR [ebp-0x5]` is used as a temporary variable for a `char`, so we will call this `tmp` for now. The disassembly is much clearer if we use these variable names instead of `ebp` offsets:
```
Dump of assembler code for function decrypt:
   0x0804850b <+0>:	    push   ebp
   0x0804850c <+1>:	    mov    ebp,esp
   0x0804850e <+3>:	    sub    esp,0x10
   
   0x08048511 <+6>:	    mov    {ctr},0x0
   0x08048518 <+13>:	jmp    0x8048559 <decrypt+78>
   
   LOOP_START:
   0x0804851a <+15>:	mov    eax,0xf
   0x0804851f <+20>:	sub    eax,{ctr}
   0x08048522 <+23>:	movzx  eax,BYTE PTR [eax+0x804a057]
   0x08048529 <+30>:	mov    {tmp},al
   
   0x0804852c <+33>:	mov    eax,0xf
   0x08048531 <+38>:	sub    eax,{ctr}
   0x08048534 <+41>:	mov    edx,{ctr}
   0x08048537 <+44>:	add    edx,0x804a057
   0x0804853d <+50>:	movzx  edx,BYTE PTR [edx]
   0x08048540 <+53>:	mov    BYTE PTR [eax+0x804a057],dl
   
   0x08048546 <+59>:	mov    eax,{ctr}
   0x08048549 <+62>:	lea    edx,[eax+0x804a057]
   0x0804854f <+68>:	movzx  eax,{tmp}
   0x08048553 <+72>:	mov    BYTE PTR [edx],al
   
   0x08048555 <+74>:	add    {counter},0x1
   
   0x08048559 <+78>:	cmp    {counter},0x7
   0x0804855d <+82>:	jle    LOOP_START <decrypt+15>
   
   0x0804855f <+84>:	leave  
   0x08048560 <+85>:	ret    
End of assembler dump.
```

Luckily this binary is *not* stripped, so we can use the `info` command in `gdb` to see the addresses of the global arrays in the source code:
```
(gdb) info variable buffer
All variables matching regular expression "buffer":

Non-debugging symbols:
0x0804a057  buffer
(gdb) info variable secret
All variables matching regular expression "secret":

Non-debugging symbols:
0x0804a034  secret
```

Now we can add these names into our `decrypt()` disassembly to make it even more human-readable:
```
Dump of assembler code for function decrypt:
   0x0804850b <+0>:	    push   ebp
   0x0804850c <+1>:	    mov    ebp,esp
   0x0804850e <+3>:	    sub    esp,0x10
   
   0x08048511 <+6>:	    mov    {ctr},0x0
   0x08048518 <+13>:	jmp    0x8048559 <decrypt+78>
   
   LOOP_START:
   0x0804851a <+15>:	mov    eax,0xf
   0x0804851f <+20>:	sub    eax,{ctr}
   0x08048522 <+23>:	movzx  eax,BYTE PTR [eax+{buffer}]
   0x08048529 <+30>:	mov    {tmp},al
   
   0x0804852c <+33>:	mov    eax,0xf
   0x08048531 <+38>:	sub    eax,{ctr}
   0x08048534 <+41>:	mov    edx,{ctr}
   0x08048537 <+44>:	add    edx,{buffer}
   0x0804853d <+50>:	movzx  edx,BYTE PTR [edx]
   0x08048540 <+53>:	mov    BYTE PTR [eax+{buffer}],dl
   
   0x08048546 <+59>:	mov    eax,{ctr}
   0x08048549 <+62>:	lea    edx,[eax+{buffer}]
   0x0804854f <+68>:	movzx  eax,{tmp}
   0x08048553 <+72>:	mov    BYTE PTR [edx],al
   
   0x08048555 <+74>:	add    {counter},0x1
   
   0x08048559 <+78>:	cmp    {counter},0x7
   0x0804855d <+82>:	jle    LOOP_START <decrypt+15>
   
   0x0804855f <+84>:	leave  
   0x08048560 <+85>:	ret    
End of assembler dump.
```

The first part of the loop:
```
   0x0804851a <+15>:	mov    eax,0xf
   0x0804851f <+20>:	sub    eax,{ctr}
   0x08048522 <+23>:	movzx  eax,BYTE PTR [eax+{buffer}]
   0x08048529 <+30>:	mov    {tmp},al
```
is the equivalent C code of:
```C
    tmp = buffer[15-ctr];
```

The second part of the loop:
```
   0x0804852c <+33>:	mov    eax,0xf
   0x08048531 <+38>:	sub    eax,{ctr}
   0x08048534 <+41>:	mov    edx,{ctr}
   0x08048537 <+44>:	add    edx,{buffer}
   0x0804853d <+50>:	movzx  edx,BYTE PTR [edx]
   0x08048540 <+53>:	mov    BYTE PTR [eax+{buffer}],dl
```
is the equivalent C code of:
```C
    buffer[15-ctr] = buffer[ctr];
```

The third part of the loop:
```
   0x08048546 <+59>:	mov    eax,{ctr}
   0x08048549 <+62>:	lea    edx,[eax+{buffer}]
   0x0804854f <+68>:	movzx  eax,{tmp}
   0x08048553 <+72>:	mov    BYTE PTR [edx],al
```
is the equivalent C code of:
```C
    buffer[ctr] = tmp;
```

Now we have the entire `decrypt()` function, and we can see that it is merely swapping the bytes of `buffer[]` such that `buffer[0]` swaps with `buffer[15]`, `buffer[1]` swaps with `buffer[14]`, etc.:
```C
void decrypt() {
  int ctr = 0;
  for (ctr = 0; ctr < 8; ctr++) {
    tmp = buffer[15-ctr];
    buffer[15-ctr] = buffer[ctr];
    buffer[ctr] = tmp;
  }
}
```

`secret[]` is fixed:
```
(gdb) x/16xb 0x0804a034
0x804a034 <secret>:	    0x37	0x63	0x65	0x36	0x66	0x32	0x33	0x36
0x804a03c <secret+8>:	0x62	0x33	0x31	0x33	0x35	0x35	0x35	0x38
```

Our hex string key then becomes the reverse of these bytes: `"38353535333133623633326636656337"`:
```
ameise@STAHLKERN:~/CS2015/re/070_CRACKME_0$ ./crackme_0 38353535333133623633326636656337
That is correct!
```