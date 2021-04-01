## SUPERSTRING [60]

**Summary**: Hard-coded key

**Description**: We got a simple one for ya, find a key that [this program](superstring) (also available at `/home/problems/2629978fd5b34968713d8eaf5ce24bfc/superstring_8` on the shell server) accepts.

## Solution

Opening the binary in `gdb`, we can see that it does a `strncmp()` in `main()` that compares the first command line argument (`argv[1]`) with a hard-coded string that is written into memory byte-by-byte:
```
ameise@STAHLKERN:~/CS2015/re/060_SUPERSTRING$ gdb ./superstring -q
Reading symbols from ./superstring...(no debugging symbols found)...done.
(gdb) disas main
Dump of assembler code for function main:
[...]
   0x080483fd <+45>:	mov    BYTE PTR [ebp-0x1d],0x0
   0x08048401 <+49>:	mov    BYTE PTR [ebp-0x3d],0x6e
   0x08048405 <+53>:	lea    eax,[ebp-0x3d]
   0x08048408 <+56>:	mov    BYTE PTR [ebp-0x3c],0x6f
   0x0804840c <+60>:	mov    BYTE PTR [ebp-0x3b],0x5f
   0x08048410 <+64>:	sub    esp,0x4
   0x08048413 <+67>:	mov    BYTE PTR [ebp-0x3a],0x6d
   0x08048417 <+71>:	mov    BYTE PTR [ebp-0x39],0x6f
   0x0804841b <+75>:	mov    BYTE PTR [ebp-0x38],0x72
   0x0804841f <+79>:	mov    BYTE PTR [ebp-0x37],0x65
   0x08048423 <+83>:	mov    BYTE PTR [ebp-0x36],0x5f
   0x08048427 <+87>:	mov    BYTE PTR [ebp-0x35],0x6d
   0x0804842b <+91>:	mov    BYTE PTR [ebp-0x34],0x72
   0x0804842f <+95>:	mov    BYTE PTR [ebp-0x33],0x5f
   0x08048433 <+99>:	mov    BYTE PTR [ebp-0x32],0x6e
   0x08048437 <+103>:	mov    BYTE PTR [ebp-0x31],0x69
   0x0804843b <+107>:	mov    BYTE PTR [ebp-0x30],0x63
   0x0804843f <+111>:	mov    BYTE PTR [ebp-0x2f],0x65
   0x08048443 <+115>:	mov    BYTE PTR [ebp-0x2e],0x5f
   0x08048447 <+119>:	mov    BYTE PTR [ebp-0x2d],0x73
   0x0804844b <+123>:	mov    BYTE PTR [ebp-0x2c],0x74
   0x0804844f <+127>:	mov    BYTE PTR [ebp-0x2b],0x72
   0x08048453 <+131>:	mov    BYTE PTR [ebp-0x2a],0x69
   0x08048457 <+135>:	mov    BYTE PTR [ebp-0x29],0x6e
   0x0804845b <+139>:	mov    BYTE PTR [ebp-0x28],0x67
   0x0804845f <+143>:	mov    BYTE PTR [ebp-0x27],0x73
   0x08048463 <+147>:	mov    BYTE PTR [ebp-0x26],0x5f
   0x08048467 <+151>:	mov    BYTE PTR [ebp-0x25],0x39
   0x0804846b <+155>:	mov    BYTE PTR [ebp-0x24],0x39
   0x0804846f <+159>:	mov    BYTE PTR [ebp-0x23],0x6e
   0x08048473 <+163>:	mov    BYTE PTR [ebp-0x22],0x63
   0x08048477 <+167>:	mov    BYTE PTR [ebp-0x21],0x53
   0x0804847b <+171>:	mov    BYTE PTR [ebp-0x20],0x50
   0x0804847f <+175>:	mov    BYTE PTR [ebp-0x1f],0x52
   0x08048483 <+179>:	mov    BYTE PTR [ebp-0x1e],0x44
   0x08048487 <+183>:	mov    esi,DWORD PTR [edx+0x4]
   0x0804848a <+186>:	push   0x21
   0x0804848c <+188>:	push   eax
   0x0804848d <+189>:	push   esi
   0x0804848e <+190>:	call   0x80483c0 <strncmp@plt>
[...]
End of assembler dump.
```

While we could manually copy out each hex byte and convert it to ASCII, it would be much easier to set a breakpoint just before the call to `strncmp()` and read the key directly from memory as a complete string:
```
(gdb) break *0x0804848e
Breakpoint 1 at 0x804848e
(gdb) set args whocares
(gdb) run
Starting program: /home/ameise/CS2015/re/060_SUPERSTRING/superstring whocares

Breakpoint 1, 0x0804848e in main ()
(gdb) x/s $eax
0xffffcf4b:	"no_more_mr_nice_strings_99ncSPRD"
```