#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

char keybuf[38];

void echoer() {
  
  char tmp;
  char buffer[128];
  int ctr;

  memset(buffer, 0, 128);
  tmp = 'a';
  ctr = 0;
  
  while (ctr < 128) {

    read(0, &tmp, 1);
    if (tmp == '\n') break;
    buffer[ctr] = tmp;
    ctr++;
  }

  if (ctr == 128) buffer[ctr - 1] = '\0';

  printf(buffer);
}

int main() {

  int keyfd = open("./key", O_RDONLY);

  keybuf[0] = 'F';
  keybuf[1] = 'l';
  keybuf[2] = 'a';
  keybuf[3] = 'g';
  keybuf[4] = ':';
  keybuf[5] = ' ';
  keybuf[6] = '\0';

  read(keyfd, &keybuf[6], 32);
  close(keyfd);

  keybuf[38] = '\0';

  echoer();
  
  putchar('\n');
  return fflush(stdout);

}
