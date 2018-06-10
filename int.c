#include "haribote.h"

#define PORT_KEYDAT 0x0060

struct FIFO8 keyfifo;
struct FIFO8 mousefifo;

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
    fifo8_put(&keyfifo, data);
    return;
}

void inthandler2c(int *esp)
{
    unsigned char data;
    _io_out8(PIC1_OCW2, 0x64);
    _io_out8(PIC0_OCW2, 0x62);
    data = _io_in8(PORT_KEYDAT);
    fifo8_put(&mousefifo, data);
    return;
}

void inthandler27(int *esp)
{
    _io_out8(PIC0_OCW2, 0x67);
    return;
}