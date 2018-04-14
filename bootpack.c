/*** from assembly ***/
void _io_hlt(void);
void _io_cli(void);
void _io_out8(int port, int data);
int _io_load_eflags(void);
void _io_store_eflags(int eflags);

/*** from C ***/
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

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
    int i;
    char *p;

    init_palette();

    p = (char *) 0xa0000;

    boxfill(p, 320, COL8_FF0000, 20, 20, 120, 120);
    boxfill(p, 320, COL8_00FF00, 70, 50, 170, 150);
    boxfill(p, 320, COL8_0000FF, 120, 80, 220, 180);

    for(;;) {
        _io_hlt();
    }
}

void init_palette(void)
{
    static unsigned char table_rgb[16 * 3] = {
        0x00, 0x00, 0x00, //black
        0xff, 0x00, 0x00, //bright red
        0x00, 0xff, 0x00, //bright green
        0xff, 0xff, 0x00, //bright yellow
        0x00, 0x00, 0xff, //bright blue
        0xff, 0x00, 0xff, //bright purple
        0x00, 0xff, 0xff, //bright light blue
        0xff, 0xff, 0xff, //white
        0xc6, 0xc6, 0xc6, //bright grey
        0x84, 0x00, 0x00, //dark red
        0x00, 0x84, 0x00, //dark green
        0x84, 0x84, 0x00, //dark yellow
        0x00, 0x00, 0x84, //dark blue
        0x84, 0x00, 0x84, //dark purple
        0x00, 0x84, 0x84, //dark light blue
        0x84, 0x84, 0x84  //dark gray
    };
    set_palette(0, 15, table_rgb);
    return;
}

void set_palette(int start, int end, unsigned char *rgb)
{
    int i, eflags;
    eflags = _io_load_eflags();
    _io_cli();
    _io_out8(0x03c8, start);
    for(i = start; i <= end; i++) {
        _io_out8(0x03c9, rgb[0] / 4);
        _io_out8(0x03c9, rgb[1] / 4);
        _io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    _io_store_eflags(eflags);
    return;
}

void boxfill(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
    int x, y;
    for(y = y0; y <= y1; y++) {
        for(x = x0; x <= x1; x++)
            vram[y * xsize + x] = c;
    }
    return;
}