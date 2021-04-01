#include <stdio.h>
#include <stdlib.h>

int main()
{
  unsigned char block1[0x1000];
  unsigned char block2[0x1000];
  unsigned char block3[0x1000];
  
  FILE *fp1;
  FILE *fp2;
  FILE *fp3;
  
  fp1 = fopen("block1", "r");
  fread(block1, 1, 0x1000, fp1);  
  fclose(fp1);
  
  fp2 = fopen("block2", "r");
  fread(block2, 1, 0x1000, fp2);  
  fclose(fp2);
    
  fp3 = fopen("block3", "r");
  fread(block3, 1, 0x1000, fp3);
  fclose(fp3);
  
  unsigned char keyblock[0x1000];

  unsigned int ctr;
  for(ctr = 0x0; ctr < 0x1000; ctr++)
  {
    keyblock[ctr] = (block2[ctr] ^ block1[ctr]) + block3[ctr];
  }
  
  FILE* fpkey = fopen("userblock", "w");
  fwrite(keyblock, 1, 0x1000, fpkey);
  fclose(fpkey);
  
  return 0;
}
