// 16 bit real mode

#include <stdint.h>
#include "../16bitdrv/vgaInit.h"
#include "./memmap.h"

extern void PModeMain(void);

#pragma pack(push, 1)
typedef struct {
    uint16_t segmentLimitFirst16Bits;
    uint16_t segmentBaseAddressFirst16Bits;
    uint8_t middleBaseBytes;
    uint8_t accessByte;
    uint8_t highLimitAndFlags; // flag first, limit second
    uint8_t baseAddressFinal8Bits;
} gdtEntry;

typedef struct {
    uint16_t limit;
    uint32_t base;
} gdtr_t;
#pragma pack(pop)

gdtr_t gdtr;
gdtEntry gdt[5];

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

int main (void) {
    gdtEntry nullDesc = {0, 0, 0, 0, 0, 0};
    gdtEntry kernelModeCodeSegment = {0xFFFF, 0, 0, 0x9A, 0xCF, 0};
    gdtEntry kernelModeDataSegment = {0xFFFF, 0, 0, 0x92, 0xCF, 0};
    gdtEntry userModeCodeSegment = {0xFFFF, 0, 0, 0xFA, 0xCF, 0};
    gdtEntry userModeDataSegment = {0xFFFF, 0, 0, 0xF2, 0xCF, 0};

    
    gdt[0] = nullDesc;
    gdt[1] = kernelModeCodeSegment;
    gdt[2] = kernelModeDataSegment;
    gdt[3] = userModeCodeSegment;
    gdt[4] = userModeDataSegment;

    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = (uint32_t)&gdt;   
    
    initVideoMode(pal);
    
    asm volatile (
        "cli\n\t"                  // clear interrupt vector table
        "lgdt %0\n\t"              // load gdt (%0 -> gdtr)
        "mov %%cr0, %%eax\n\t"     
        "or $1, %%eax\n\t"         
        "mov %%eax, %%cr0\n\t"     // enable protected mode
        "ljmp $0x08, $1f\n\t"      // far jump to clear pipeline, set cs
        "1:\n\t"                   // label for the jump destination
        "jmp pModeMain"            // jmp to ASM setup (global pModeMain)
        : 
        : "m" (gdtr)               // the gdtr structure in memory
        : "eax", "memory"          // clobbers
    );

    return 0;
}