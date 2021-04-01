#include <stdio.h>
#include <stdlib.h>

int main() {

  FILE* fp1;
  FILE* fp2;
  FILE* fpad;
  unsigned int header[138];
  unsigned int data1[179916];
  unsigned int data2[179916];
  int ctr;

  fp1 = fopen("enc1.bmp","r");
  fread((void*)header, 1, 138, fp1);
  fread((void*)data1, 1, 179916, fp1);
  fclose(fp1);


  fp2 = fopen("enc2.bmp","r");
  fseek(fp2, 138, SEEK_SET);
  fread((void*)data2, 1, 179916, fp2);
  fclose(fp2);

  fpad = fopen("pad.bmp","w");
  for (ctr = 0; ctr < 179916; ctr++) {
    data1[ctr] = data1[ctr] ^ data2[ctr];
  }

  fwrite((void*)header, 1, 138, fpad);
  fwrite((void*)data1, 1, 179916, fpad);
  fclose(fpad);

  return 0;
}
