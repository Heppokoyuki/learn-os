#include "haribote.h"

struct FIFO8 keyfifo;

void wait_KBC_sendready(void)
{
    for(;;) {
        if((_io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
            break;
        }
    }
    return;
}

void init_keyboard(void)
{
    wait_KBC_sendready();
    _io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    wait_KBC_sendready();
    _io_out8(PORT_KEYDAT, KBC_MODE);
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