#include "haribote.h"

extern struct FIFO8 keyfifo;
extern struct FIFO8 mousefifo;

void enable_mouse(void);
void init_keyboard(void);

void HariMain(void)
{
    struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
    char *mcursor, s[40], keybuf[32], mousebuf[128];
    int mx, my, i;

    init_gdtidt();
    init_pic();
    _io_sti();
    fifo8_init(&keyfifo, 32, keybuf);
    fifo8_init(&mousefifo, 128, mousebuf);
    _io_out8(PIC0_IMR, 0xf9);
    _io_out8(PIC1_IMR, 0xef);

    init_keyboard();

    init_palette();
    init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
    mx = (binfo->scrnx - 16) / 2;
    my = (binfo->scrny - 28 - 16) / 2;
    init_mouse_cursor8(mcursor, COL8_008484);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
    sprintf(s, "(%d, %d)", mx, my);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

    enable_mouse();

    for(;;) {
        _io_cli();
        if(fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
            _io_stihlt();
        }
        else {
            if(fifo8_status(&keyfifo) != 0) {
                i = fifo8_get(&keyfifo);
                _io_sti();
                sprintf(s, "%x", i);
                boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
                putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
            }
            else if(fifo8_status(&mousefifo) != 0) {
                i = fifo8_get(&mousefifo);
                _io_sti();
                sprintf(s, "%x", i);
                boxfill8(binfo->vram, binfo->scrnx, COL8_848484, 32, 16, 47, 31);
                putfonts8_asc(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
            }
        }
    }
}

#define PORT_KEYDAT 0x0060
#define PORT_KEYSTA 0x0064
#define PORT_KEYCMD 0x0064
#define KEYSTA_SEND_NOTREADY 0x02
#define KEYCMD_WRITE_MODE 0x60
#define KBC_MODE 0x47
#define KEYCMD_SENDTO_MOUSE 0xd4
#define MOUSECMD_ENABLE 0xf4

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

void enable_mouse(void)
{
    wait_KBC_sendready();
    _io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_KBC_sendready();
    _io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
    return;
}