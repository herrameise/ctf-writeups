#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>

#define FLAGLEN 25

uint8_t GetIndex(uint8_t* A, uint8_t value);
void PrintBytes(uint8_t* bytes, uint32_t len);


int main(int argc, char** argv) {

  FILE* fp;
  int ctr;
  uint8_t idx;
  uint8_t magic;
  uint8_t A[0x100];
  uint8_t B[FLAGLEN];
  uint8_t C[FLAGLEN];
  uint8_t ans[FLAGLEN];
  char flag[26];


  uint8_t magicvals[25];



/*=============
    FORWARD
=============*/

  fp = fopen("a.dat","r");
  fread(A, 1, 0x100, fp);
  fclose(fp);
  
  memset(flag, 'a', 25);
  flag[25] = '\0';
  
  for (ctr = 0; ctr < FLAGLEN; ctr++) {
    B[ctr] = A[flag[ctr]];
  }
  
  idx = 0x00;
  magic = 0x00;
  for (ctr = 0; ctr < FLAGLEN; ctr++) {
  
    idx += 1;
    magic += A[idx];
    
    // swaps A[idx] and A[magic]
    A[idx] = A[idx] ^ A[magic];
    A[magic] = A[magic] ^ A[idx];
    A[idx] = A[idx] ^ A[magic];
    
    C[ctr] = A[ A[B[ctr]] + A[idx] + A[magic] ];
  }



/*==============
    REVERSE
==============*/
  
  fp = fopen("ans.dat","r");
  fread(C, 1, 25, fp);
  fclose(fp);
  
  uint8_t tmp1, tmp2;
  uint8_t bctr;
  
  magic = 0xCE;
  idx = FLAGLEN;
  for (ctr = (FLAGLEN-1); ctr >= 0; ctr--) {
  
    tmp1 = GetIndex(A, C[ctr]);
    tmp2 = tmp1 - A[idx] - A[magic];
    B[ctr] = GetIndex(A, tmp2);
    
    A[idx] = A[idx] ^ A[magic];
    A[magic] = A[magic] ^ A[idx];
    A[idx] = A[idx] ^ A[magic];
    
    magic -= A[idx];
    idx--;
  }
  
  for (ctr = (FLAGLEN-1); ctr >= 0; ctr--) {
    flag[ctr] = GetIndex(A, B[ctr]);
  }
  
  flag[FLAGLEN] = '\0';
  printf("flag: %s\n", flag);
  
  return 0;
}



uint8_t GetIndex(uint8_t* A, uint8_t value) {

  uint8_t ctr = 0x00;
  while (A[ctr] != value) ctr++;
  return ctr;
}



void PrintBytes(uint8_t* bytes, uint32_t len) {

  uint32_t ctr;
  for (ctr = 0; ctr < len; ctr++) {
    if ((ctr % 16) == 0) printf("0x%08x   ", ctr);
    printf("0x%02x  ", bytes[ctr]);
    if (((ctr+1) % 16) == 0) printf("\n");
  }
  printf("\n\n");
}







