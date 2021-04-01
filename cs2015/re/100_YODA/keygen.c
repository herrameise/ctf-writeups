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
