all: sandbox keygen



sandbox: sandbox.c sighandler.o
	gcc sandbox.c sighandler.o -m32 -O0 -fno-stack-protector -mpreferred-stack-boundary=2 -lcapstone -o sandbox

sighandler.o: sighandler.asm
	nasm sighandler.asm -f elf32 -o sighandler.o



keygen: keygen.c
	gcc keygen.c -o keygen



clean:
	rm sandbox sighandler.o keygen
