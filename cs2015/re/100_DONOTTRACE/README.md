## DONOTTRACE [100]

## Solution
**WORK IN PROGRESS**:

Extract `supersecret[]` and `translate[]` global buffers with `gdb`:
```
ameise@STAHLKERN:~/CS2015/re/100_DONOTTRACE$ gdb ./donottrace 
Reading symbols from ./donottrace...(no debugging symbols found)...done.
(gdb) info variables translate
All variables matching regular expression "translate":

Non-debugging symbols:
0x0804a080  translate
(gdb) info variables supersecret 
All variables matching regular expression "supersecret":

Non-debugging symbols:
0x0804a180  supersecret
(gdb) dump binary memory translate 0x0804a080 0x0804a180
(gdb) dump binary memory supersecret 0x0804a180 0x0804a1a0
(gdb) quit
```

Keygen:
```C
#include <stdio.h>
#include <stdlib.h>

int main() {

  FILE* fpss = fopen("supersecret", "r");
  unsigned char ss[32];
  fread(ss, 1, 32, fpss);
  fclose(fpss);

  FILE* fptr = fopen("translate", "r");
  unsigned char tr[0x100];
  fread(tr, 1, 0x100, fptr);
  fclose(fptr);

  char key[33];
  int ctr;
  for (ctr = 0; ctr < 32; ctr++) {
    key[ctr] = tr[ss[ctr]];
  }

  key[32] = '\0';
  printf("key: %s\n", key);

  return 0;
}

```

The CyberStakes web interface accepted the key I generated as correct, but the binary did not for some reason ...
```
ameise@STAHLKERN:~/CS2015/re/100_DONOTTRACE$ gcc keygen.c -o keygen
ameise@STAHLKERN:~/CS2015/re/100_DONOTTRACE$ ./keygen 
key: 86403f3ed1d43e954127eaf4be68388c
ameise@STAHLKERN:~/CS2015/re/100_DONOTTRACE$ ./donottrace 86403f3ed1d43e954127eaf4be68388c
Nope

Nope

```
