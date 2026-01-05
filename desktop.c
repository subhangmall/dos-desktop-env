#include <conio.h>
#include <stdint.h>
#include "mouse.h"
#include "graphics.h"
#include "window.h"



void main() {
    int dx, dy;
    struct Window progMgr;
    // uint8_t majorVersion, minorVersion;

    initVideoMode();
    dispCursor();
    drawBackground();

    
    // __asm {
    //     mov ah, 0x30
    //     int 0x21
    //     mov majorVersion, al
    //     mov minorVersion, ah
    //     mov dl, majorVersion
    //     add dl, 0x30
    //     mov ah, 0x02
    //     int 0x21
    //     mov dl, minorVersion
    //     add dl, 0x30
    //     mov ah, 0x02
    //     int 0x21
    // }

    progMgr.x = 100;
    progMgr.y = 50;
    progMgr.width = 400;
    progMgr.height = 300;
    progMgr.priority = 1;
    progMgr.isFocused = true;
    strcpy(progMgr.title, "Program Manager");
    

    renderWindow(&progMgr);
    
    
    getch(); // Wait for a key press
    
}