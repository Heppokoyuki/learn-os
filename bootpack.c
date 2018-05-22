#include "haribote.h"
#include "sprintf.c"

extern void sprintf(char *str, char *fmt, ...);

void HariMain(void)
{
    struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
    char *mcursor;
    int mx, my;

    mx = (binfo->scrnx - 16) / 2;
    my = (binfo->scrny - 28 - 16) / 2;

    init_gdtidt();
    init_pic();

    init_palette();
    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
    putfonts8_asc(binfo->vram, binfo->scrnx, 8, 8, COL8_FFFFFF, "LAN GA SASA LAN");
    init_mouse_cursor8(mcursor, COL8_008484);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

    for(;;) {
        _io_hlt();
    }
}
