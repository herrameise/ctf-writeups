#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


char buf[80];

int main (int argc, char **argv)
{

  unsigned int filler0 = 0x10757011;
  unsigned int filler1 = 0x10111115;
  unsigned int filler2 = 0x70011115;
  unsigned int filler3 = 0x11151057;
  unsigned int key = ???;
  unsigned int filler4 = 0x51757175;
  unsigned int filler5 = 0x70117175;
  unsigned int filler6 = 0x70111075;
  unsigned int filler7 = 0x10115117;
  unsigned int filler8 = 0x51101157;

  unsigned int red = read(STDIN_FILENO,buf,80);
  buf[red] = '\x00';
  printf(buf);
}
