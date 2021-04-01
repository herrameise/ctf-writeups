#include <stdio.h>
#include <stdlib.h>
#include <time.h>


extern int testa(int, int);
int testb(int, int);


void Test(int num1, int num2) {
  // x86 version

  if (testa(num1, num2) == testb(num1, num2)) {
    printf("SUCCESS\n");
  } else {
    printf("FAILURE\n");
  }
}

int main() {

  int num1 = 12345678;
  int num2 = 87654321;
  int rv;

  srand(time(NULL));

  int ctr;
  int iterations = 100;
  for (ctr = 0; ctr < iterations; ctr++) {
    Test(rand(), rand());
  }

  return 0;
}

int testb(int A, int B) {

  if (
       ((unsigned int)A /
       ((((unsigned int)((B + (A / (B|2))) * -1039577924) / (unsigned int)(B|2)) + ((A >= -668203058) ? 0 : 1)) | 2)
     ) == 0)
  {
    return (unsigned int)A / (unsigned int)((A + ((A >= -837399105) ? 0 : 1)) | 2);
  }
  else
  {
    return A + (((A >= -1357278678) ? 0 : 1) - A);
  }
}
