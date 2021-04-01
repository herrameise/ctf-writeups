## TWO-TIME PAD [60]
**Tags**: simple XOR encryption

**Description**: My new encryption (two-time pad, under patent review) is twice as secure as one time pads. I am so certain about the security of my new scheme that I am releasing the encrypted versions of my two most secret pictures: [Picture 1](enc1.bmp) and [Picture 2](enc2.bmp)

## Solution
The BMP images are encrypted with the same pad, so taking the XOR of each pixel of the two encrypted images will yield the pad image. According to the BMP file format [here](https://en.wikipedia.org/wiki/BMP_file_format), the offset of the first pixel of image data will be a 4-byte value at offset `0xa` in the BMP header:
```
ameise@STAHLKERN:~/CS2015/crypto/060_TWO_TIME_PAD$ xxd enc1.bmp | head -n 1
00000000: 424d aabf 0200 0000 0000 8a00 0000 7c00  BM............|.
                                   ^       ^
                                   |_______|
```

The pixel data starts at `0x0000008a` (138) bytes into the file. Each file is 180054 bytes long, which means the image data is 179916 bytes. We will create a new BMP image file that is the XOR of the other two images' data sections while preserving the BMP header:
```C
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
```

This will yield the pad image [pad.bmp](pad.bmp):
```
ameise@STAHLKERN:~/CS2015/crypto/060_TWO_TIME_PAD$ gcc xor.c -o xor
ameise@STAHLKERN:~/CS2015/crypto/060_TWO_TIME_PAD$ ./xor 
ameise@STAHLKERN:~/CS2015/crypto/060_TWO_TIME_PAD$ ls
enc1.bmp  enc2.bmp  pad.bmp  xor  xor.c
ameise@STAHLKERN:~/CS2015/crypto/060_TWO_TIME_PAD$ 
```
![pad.bmp](pad.bmp)