## YODA [100]

## Solution
**NEED FULL WRITEUP**

```
ameise@STAHLKERN:~/CS2015/re/100_YODA$ gdb ./yoda 
Reading symbols from ./yoda...(no debugging symbols found)...done.
(gdb) dump binary memory addends 0x08048098 0x080480b8
(gdb) quit
```

`byte_08048098` is 32 bytes (we'll call it `addends[]`). First byte is `0x30` (ASCII `0`). Each successive byte is added to the previous byte to produce a new character (all characters are close to `0x00` or close to `0xff`). Now we can write a keygen:
```C
#include <stdio.h>
#include <stdlib.h>

int main() {
  
  FILE* fp = fopen("addends","r");
  unsigned char addends[32];
  fread(addends, 1, 32, fp);
  fclose(fp);
  
  char c = addends[0];
  printf("key: %c", addends[0]);
  
  int ctr;
  for (ctr = 1; ctr < 32; ctr++) {
    c = c + addends[ctr];
    printf("%c", c);
  }
  
  printf("\n");
  
  return 0;
}
```

Compile keygen and get key:
```
ameise@STAHLKERN:~/CS2015/re/100_YODA$ gcc keygen.c -o keygen
ameise@STAHLKERN:~/CS2015/re/100_YODA$ ./keygen 
key: 04724165562761024105080155549904
ameise@STAHLKERN:~/CS2015/re/100_YODA$ ./yoda 04724165562761024105080155549904
That's correct! That's the right number! :)
```