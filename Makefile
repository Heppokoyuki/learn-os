default:
	make img

#files

ipl.bin: ipl.asm Makefile
	nasm ipl.asm -o ipl.bin -l ipl.lst

asmhead.bin: asmhead.asm Makefile
	nasm asmhead.asm -o asmhead.bin -l asmhead.lst

nasmfunc.o: nasmfunc.asm Makefile
	nasm -g -f elf nasmfunc.asm -o nasmfunc.o

haribote.sys: asmhead.bin bootpack.hrb Makefile
	cat asmhead.bin bootpack.hrb > haribote.sys

bootpack.hrb: bootpack.c nasmfunc.o hankaku.o graphic.o dsctbl.o int.o fifo.o keyboard.o mouse.o sprintf.o Makefile
	gcc -march=i486 -m32 -nostdlib -fno-pie -fno-stack-protector -T hrb.ld -g bootpack.c nasmfunc.o hankaku.o graphic.o dsctbl.o int.o fifo.o keyboard.o mouse.o sprintf.o -o bootpack.hrb

HariboteOS.img: ipl.bin haribote.sys Makefile
	mformat -f 1440 -B ipl.bin -C -i HariboteOS.img ::
	mcopy haribote.sys -i HariboteOS.img ::

%.o: %.c
	gcc -c -m32 -fno-pie -nostdlib -fno-stack-protector -o $*.o $*.c

# command

img:
	make HariboteOS.img

run:
	make img
	qemu-system-i386 -fda HariboteOS.img

clean:
	rm ipl.bin ipl.lst asmhead.bin asmhead.lst haribote.sys bootpack.hrb *.o
