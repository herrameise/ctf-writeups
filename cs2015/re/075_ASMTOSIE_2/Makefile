all: test

test: testa.o test.c
	gcc -m32 -mpreferred-stack-boundary=2 -fno-stack-protector -O0 testa.o test.c -o test

testa.o: testa.asm
	nasm -f elf32 -o testa.o testa.asm

clean:
	rm test testa.o
