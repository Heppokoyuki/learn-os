NASK = ../z_tools/nask.exe

default:
	make img

#files

ipl.bin: ipl.nasm Makefile
	$(NASK) ipl.nasm ipl.bin ipl.lst

haribote.sys: haribote.nasm Makefile
	$(NASK) haribote.nasm haribote.sys haribote.lst

HariboteOS.img: ipl.bin haribote.sys Makefile
	mformat -f 1440 -B ipl.bin -C -i HariboteOS.img ::
	mcopy haribote.sys -i HariboteOS.img ::

# command

img:
	make HariboteOS.img

run:
	make img
	qemu-system-i386 -fda HariboteOS.img

clean:
	rm ipl.bin ipl.lst haribote.sys haribote.lst