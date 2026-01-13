#include <malloc.h>
#include <stdlib.h>
#include <i86.h>
#include <stdio.h>
#include <stdint.h>


unsigned char __huge *vBuffer;

void initGraphics();
void drawToBuffer(unsigned int x, unsigned int y, unsigned char color) ;
void render();

void main() {
    int i, j;

    initGraphics();
    for (i = 0; i < 640; i++) {
        for (j = 0; j < 480; j++) {
            drawToBuffer(i, j, 3);
        }
    }

    render();
}

void initGraphics() {
    unsigned char r, g, b, colorPaletteIndex;

       unsigned char pal[16][3] = {
    {0x00, 0x00, 0x00}, // 0: Black
    {0x00, 0x00, 0x20}, // 1: Dark Blue (Windows Title Bar)
    {0x00, 0x20, 0x00}, // 2: Dark Green
    {0x01, 0x20, 0x20}, // 3: Windows Teal (The Desktop Background)
    {0x20, 0x00, 0x00}, // 4: Dark Red
    {0x20, 0x00, 0x20}, // 5: Dark Magenta
    {0x20, 0x20, 0x00}, // 6: Mustard/Brown
    {0x30, 0x30, 0x30}, // 7: Light Gray (The main Window/Button color)
    {0x20, 0x20, 0x20}, // 8: Dark Gray (Button Shadows)
    {0x00, 0x00, 0x3F}, // 9: Bright Blue
    {0x00, 0x3F, 0x00}, // 10: Bright Green
    {0x00, 0x3F, 0x3F}, // 11: Bright Cyan
    {0x3F, 0x00, 0x00}, // 12: Bright Red
    {0x3F, 0x00, 0x3F}, // 13: Bright Magenta
    {0x3F, 0x3F, 0x00}, // 14: Bright Yellow
    {0x3F, 0x3F, 0x3F}  // 15: Pure White (Button Highlights)
    };
    
    __asm {
       mov ah, 0x00 // Set video mode
       mov al, 0x12 // 640x200 pixel graphics, 16 colors
       int 0x10 // Call BIOS video interrupt
   }

    for (colorPaletteIndex = 0; colorPaletteIndex < 16; colorPaletteIndex++) {
        r = pal[colorPaletteIndex][0];
        g = pal[colorPaletteIndex][1];
        b = pal[colorPaletteIndex][2];
        
        __asm {
            mov dx, 0x3C8
            mov al, colorPaletteIndex
            out dx, al // set palette index
            mov dx, 0x3C9
            mov al, r  // Red
            out dx, al
            mov al, g  // Green
            out dx, al
            mov al, b  // Blue
            out dx, al
        }
    }

    vBuffer = (unsigned char __huge *)halloc(307200L, 1);
}

void drawToBuffer(unsigned int x, unsigned int y, unsigned char color) {
   unsigned long offset = (640UL * y) + (unsigned long)x;
   vBuffer[offset] = color & 0x0F; // to be safe, mask to 4 bits
}

void render() {
   unsigned int plane;
   unsigned long startingByteIdx;
   unsigned int byteIdx;
   unsigned char bitMask;
   unsigned char byte;
   volatile unsigned char __far *vga = (volatile unsigned char __far *)MK_FP(0xA000, 0);
   unsigned int vgaIdx;
   unsigned char __huge *p;


   __asm {
       mov dx, 0x3CE
       mov al, 0x05
       out dx, al
       inc dx
       mov al, 0x00
       out dx, al
   }

   __asm {
       mov dx, 0x3CE
       mov al, 0x08
       out dx, al
       inc dx
       mov al, 0xFF
       out dx, al
   }

   for (plane = 0; plane < 4; plane++) {
       // select plane to write to
       bitMask = 1 << plane;
       vgaIdx = 0;
       
       __asm {
           mov dx, 0x3C4
           mov al, 0x02
           out dx, al
           inc dx
           mov al, bitMask
           out dx, al
       }

       for(startingByteIdx = 0; startingByteIdx < 307200; startingByteIdx += 8) {
           byte = 0;
          
           // start at this specific byte index
           p = &vBuffer[startingByteIdx];

           if (p[0] & bitMask) byte |= 0x80;
           if (p[1] & bitMask) byte |= 0x40;
           if (p[2] & bitMask) byte |= 0x20;
           if (p[3] & bitMask) byte |= 0x10;
           if (p[4] & bitMask) byte |= 0x08;
           if (p[5] & bitMask) byte |= 0x04;
           if (p[6] & bitMask) byte |= 0x02;
           if (p[7] & bitMask) byte |= 0x01;

           vga[vgaIdx++] = byte;
       } 
   }

   __asm {
       mov dx, 0x3C4
       mov al, 0x02
       out dx, al
       inc dx
       mov al, 0xFF
       out dx, al
   }
}
