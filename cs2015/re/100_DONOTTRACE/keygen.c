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
