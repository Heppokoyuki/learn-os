#include "haribote.h"

#define PORT_KEYDAT 0x0060

struct KEYBUF keybuf;

void init_pic(void)
{
    _io_out8(PIC0_IMR, 0xff);
    _io_out8(PIC1_IMR, 0xff);

    _io_out8(PIC0_ICW1, 0x11);
    _io_out8(PIC0_ICW2, 0x20);
    _io_out8(PIC0_ICW3, 1 << 2);
    _io_out8(PIC0_ICW4, 0x01);

    _io_out8(PIC1_ICW1, 0x11);
    _io_out8(PIC1_ICW2, 0x28);
    _io_out8(PIC1_ICW3, 2);
    _io_out8(PIC0_ICW4, 0x01);

    _io_out8(PIC0_IMR, 0xfb);
    _io_out8(PIC1_IMR, 0xff);

    return;
}

void inthandler21(int *esp)
{
    unsigned char data;
    _io_out8(PIC0_OCW2, 0x61);
    data = _io_in8(PORT_KEYDAT);
    if(keybuf.len < 32) {
        keybuf.data[keybuf.next_w] = data;
        keybuf.len++;
        keybuf.next_w++;
        if(keybuf.next_w == 32) {
            keybuf.next_w = 0;
        }
    }
    return;
}

void inthandler2c(int *esp)
{
    struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
    boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 2C (IRQ-12) : PS/2 mouse");
    for(;;) {
        _io_hlt();
    }
}

void inthandler27(int *esp)
{
    _io_out8(PIC0_OCW2, 0x67);
    return;
}