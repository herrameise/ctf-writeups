# PIN (Ubuntu 16.04)
**NOTE**: PIN will **not** work with Linux kernel version >= 4.3 due to some unknown bug. Searching through some StackOverflow and online forums shows that Intel is currently aware of the issue, but they have not released a fix yet. To get around this, you will need to download an older kernel version (I'm using version 4.2) and boot Linux from GRUB using that specific kernel.

If you are using Ubuntu, you can download older releases from http://kernel.ubuntu.com/~kernel-ppa/mainline/:
```
$ wget http://kernel.ubuntu.com/~kernel-ppa/mainline/v4.2-wily/linux-headers-4.2.0-040200_4.2.0-040200.201510260713_all.deb
$ wget http://kernel.ubuntu.com/~kernel-ppa/mainline/v4.2-wily/linux-headers-4.2.0-040200-generic_4.2.0-040200.201510260713_amd64.deb
$ wget http://kernel.ubuntu.com/~kernel-ppa/mainline/v4.2-wily/linux-image-4.2.0-040200-generic_4.2.0-040200.201510260713_amd64.deb
$ sudo dpkg -i *.deb
$ sudo update-grub
$ sudo reboot
```

Then, after rebooting, grab PIN from https://software.intel.com/en-us/articles/pin-a-dynamic-binary-instrumentation-tool:
```
ameise@STAHLKERN:~$ wget http://software.intel.com/sites/landingpage/pintool/downloads/pin-3.0-76991-gcc-linux.tar.gz
ameise@STAHLKERN:~$ tar zxf pin-3.0-76991-gcc-linux.tar.gz 
ameise@STAHLKERN:~$ cd pin-3.0-76991-gcc-linux/
ameise@STAHLKERN:~/pin-3.0-76991-gcc-linux$ ls -l
total 304
drwxr-x--- 3 ameise ameise   4096 Mar 17 04:18 doc
drwxr-x--- 2 ameise ameise   4096 Mar 17 04:18 extlicense
drwxr-x--- 8 ameise ameise   4096 Mar 17 04:18 extras
drwxr-x--- 6 ameise ameise   4096 Mar 17 04:17 ia32
drwxr-x--- 6 ameise ameise   4096 Mar 17 04:17 intel64
-rw-r----- 1 ameise ameise   7524 Mar 17 04:16 LICENSE
-rwxr-x--- 1 ameise ameise 231280 Mar 17 04:36 pin
-rw-r----- 1 ameise ameise  39661 Mar 17 04:16 README
-rw-r----- 1 ameise ameise     42 Mar 17 04:16 redist.txt
drwxr-x--- 5 ameise ameise   4096 Mar 17 04:18 source
```

The only PIN tool I currently use for CTFs is the `inscount0` tool, which counts the total number of instructions executed by a binary. In reversing problems that are checked byte-by-byte, this allows you to brute force one byte at a time and determine which one is "correct" by seeing where the variation in the number of instructions executed occurs.

The PIN documentation will tell you that to compile for 32-bit systems, you just use the `obj-ia32` prefix instead of the default `obj-intel64` prefix. However, this will not work vanilla:
```
ameise@STAHLKERN:~/pin-3.0-76991-gcc-linux/source/tools/ManualExamples$ make obj-ia32/inscount0.so
make: *** No rule to make target 'obj-ia32/inscount0.so'.  Stop.
```

After extensive Googling, I found the solution:
```
ameise@STAHLKERN:~/pin-3.0-76991-gcc-linux/source/tools/ManualExamples$ make TARGET=ia32 dir obj-ia32/inscount0.so
mkdir -p obj-ia32/
g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -D__PIN__=1 -DPIN_CRT=1 -fno-stack-protector -fno-exceptions -funwind-tables -fasynchronous-unwind-tables -fno-rtti -DTARGET_IA32 -DHOST_IA32 -DTARGET_LINUX -fabi-version=2  -I../../../source/include/pin -I../../../source/include/pin/gen -isystem /home/ameise/pin-3.0-76991-gcc-linux/extras/stlport/include -isystem /home/ameise/pin-3.0-76991-gcc-linux/extras/libstdc++/include -isystem /home/ameise/pin-3.0-76991-gcc-linux/extras/crt/include -isystem /home/ameise/pin-3.0-76991-gcc-linux/extras/crt/include/arch-x86 -isystem /home/ameise/pin-3.0-76991-gcc-linux/extras/crt/include/kernel/uapi -isystem /home/ameise/pin-3.0-76991-gcc-linux/extras/crt/include/kernel/uapi/asm-x86 -I../../../extras/components/include -I../../../extras/xed-ia32/include -I../../../source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing   -m32 -c -o obj-ia32/inscount0.o inscount0.cpp
g++ -shared -Wl,--hash-style=sysv ../../../ia32/runtime/pincrt/crtbeginS.o -Wl,-Bsymbolic -Wl,--version-script=../../../source/include/pin/pintool.ver -fabi-version=2    -m32 -o obj-ia32/inscount0.so obj-ia32/inscount0.o  -L../../../ia32/runtime/pincrt -L../../../ia32/lib -L../../../ia32/lib-ext -L../../../extras/xed-ia32/lib -lpin -lxed ../../../ia32/runtime/pincrt/crtendS.o -lpin3dwarf  -ldl-dynamic -nostdlib -lstlport-dynamic -lm-dynamic -lc-dynamic
ameise@STAHLKERN:~/pin-3.0-76991-gcc-linux/source/tools/ManualExamples$ cd obj-ia32/
ameise@STAHLKERN:~/pin-3.0-76991-gcc-linux/source/tools/ManualExamples/obj-ia32$ ls -l
total 2632
-rw-rw-r-- 1 ameise ameise   69104 Sep  8 12:29 inscount0.o
-rwxrwxr-x 1 ameise ameise 2622144 Sep  8 12:29 inscount0.so
```

Now we can use the `inscount0` tool on binaries. PIN tends to complain much less if you use absolute paths for everything, which is why I renamed the folder to just `"pin"`.
```
ameise@STAHLKERN:~$ mv pin-3.0-76991-gcc-linux pin
```

Make a simple C program and compile it with the `-32` flag so that `gcc` will compile it as a 32-bit binary:
```
ameise@STAHLKERN:~$ nano hello.c
#include <stdio.h>

int main() {
  printf("Hello, world!\n");
  return 0;
}
ameise@STAHLKERN:~$ gcc -m32 hello.c -o hello
ameise@STAHLKERN:~$ ./hello 
Hello, world!
ameise@STAHLKERN:~$ /home/ameise/pin/pin -t /home/ameise/pin/source/tools/ManualExamples/obj-ia32/inscount0.so -- /home/ameise/hello
Hello, world!
ameise@STAHLKERN:~$ cat inscount.out 
Count 158054
```

This binary executed exactly 158,054 instructions! The tool stores the data (number of instructions) in a file called `inscount.out` rather than printing to `stdout` since the binary you are instrumenting may tie up the standard pipes. Now you can use the template script [BlackBox.py](BlackBox.py) to brute force certain types of binaries instead of manually reversing them. Also see [BlackBoxExample.py](BlackBoxExample.py) for an example script to crack the [SYZYGY](re/100_SYZYGY) CyberStakes problem.