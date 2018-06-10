#include "haribote.h"

extern struct FIFO8 keyfifo;
extern struct FIFO8 mousefifo;

struct MOUSE_DEC {
    unsigned char buf[3], phase;
};

void init_keyboard(void);
void enable_mouse(struct MOUSE_DEC *mdec);
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat);

void HariMain(void)
{
    struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
    struct MOUSE_DEC mdec;
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

    enable_mouse(&mdec);

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
                if(mouse_decode(&mdec, i) != 0) {
                    sprintf(s, "%x %x %x", mdec.buf[0], mdec.buf[1], mdec.buf[2]);
                    boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16, 32 + 8 * 8 - 1, 31);
                    putfonts8_asc(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
                }
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

void enable_mouse(struct MOUSE_DEC *mdec)
{
    wait_KBC_sendready();
    _io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_KBC_sendready();
    _io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
    mdec->phase = 0;
    return;
}

int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat)
{
    if(mdec->phase == 0) {
        if(dat == 0xfa) mdec->phase = 1;
        return 0;
    }
    if(mdec->phase == 1) {
        mdec->buf[0] = dat;
        mdec->phase = 2;
        return 0;
    }
    if(mdec->phase == 2) {
        mdec->buf[1] = dat;
        mdec->phase = 3;
        return 0;
    }
    if(mdec->phase == 3) {
        mdec->buf[2] = dat;
        mdec->phase = 1;
        return 1;
    }
    return -1;
}