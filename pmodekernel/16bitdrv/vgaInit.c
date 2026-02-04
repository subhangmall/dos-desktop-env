// real mode graphicsInit.c

#include <stdint.h>
#include <stdbool.h>
#include <i86.h>
#include <malloc.h>

void setPalette(unsigned char colorPalette[16][3]);

bool checkVGACompatibility() {
    uint16_t axAfter;

    __asm {
        mov ax, 0x4F00
        int 0x10
        mov axAfter, ax
    }

    return (axAfter == 0x004F);
}


void initVideoMode(unsigned char pal[16][3]) {
    __asm {
        mov ah, 0x00 // Set video mode
        mov al, 0x12 // 640x200 pixel graphics, 16 colors
        int 0x10 // Call BIOS video interrupt
    }
    
    setPalette(pal);
}