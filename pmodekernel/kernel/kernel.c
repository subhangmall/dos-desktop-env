// main event loop

#include "../pmodedrv/vga.h"
#include "memmgt.h"
#include "idt.h"


// entry point for kernel, initialization
void kentry(void) {
    initMem();
    initIDT();
}