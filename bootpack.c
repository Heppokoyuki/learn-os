#include "sprintf.c"
#include "graphic.c"
#include "dsctbl.c"
#include <stdarg.h>

/*** from assembly ***/
void _io_hlt(void);
void _io_cli(void);
void _io_out8(int port, int data);
int _io_load_eflags(void);
void _io_store_eflags(int eflags);

/*** from C ***/
extern void init_palette(void);
extern void set_palette(int start, int end, unsigned char *rgb);
extern void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
extern void init_screen(char *vram, int x, int y);
extern void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
extern void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
extern void init_mouse_cursor8(char *mouse, char bc);
extern void putblock8_8(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize);

struct BOOTINFO {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
};

extern void sprintf(char *str, char *fmt, ...);

#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

void HariMain(void)
{
    struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;
    char *mcursor;
    int mx, my;

    mx = (binfo->scrnx - 16) / 2;
    my = (binfo->scrny - 28 - 16) / 2;

    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
    putfonts8_asc(binfo->vram, binfo->scrnx, 8, 8, COL8_FFFFFF, "SecHack365");
    init_mouse_cursor8(mcursor, COL8_008484);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

    for(;;) {
        _io_hlt();
    }
}
