#include <stdbool.h>

#define KEYBOARD_BUFFER_SIZE 32

volatile unsigned char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
volatile int keyIdx = 0;

unsigned char scrollNumberCapsLockStatusByte;

unsigned char oldOffset, oldSegment;
unsigned int curDS;

void hookInterrupt();
void disableKeyboardSet2to1Translation();

void keyboardDriver() {
    
    // enable keyboard scanning
    __asm {
        mov al, 0xF4
        mov dl, 0x64
        out dl, al
    }

    disableKeyboardSet2to1Translation();
    hookInterrupt();
}

void disableKeyboardSet2to1Translation() {
    unsigned char status, config;

    // wait till keyboard ready to read
    do {
        __asm {
            mov dl, 0x64
            in al, dl
            mov status, al
        } 
    } while (status & 0x02);

    // read current config byte; start by requesting it
    __asm {
        mov al, 0x20
        mov dl, 0x64
        out dl, al
    }

    // wait till keyboard has data ready
    do {
        __asm {
            mov dl, 0x64
            in al, dl
            mov status, al
        } 
    } while (!(status & 0x01));

    // actually read the byte once ready
    __asm {
        mov dl, 0x60
        in al, dl
        mov config, al
    }

    config &= 0b10111111;

    // wait till keyboard ready to read
    do {
        __asm {
            mov dl, 0x64
            in al, dl
            mov status, al
        } 
    } while (status & 0x02);

    // write command byte
    __asm {
        mov dl, 0x64
        mov al, 0x60
        out dl, al
    }

    // wait for keyboard to be ready to read
    do {
        __asm {
            mov dl, 0x64
            in al, dl
            mov status, al
        } 
    } while (status & 0x02);

    // write new config byte
    __asm {
        mov al, config
        mov dl, 0x60
        out dl, al
    } 
}

void updateKeyboardStatus(unsigned char scancode) {
    if (keyIdx >= KEYBOARD_BUFFER_SIZE) return;

    keyboard_buffer[keyIdx] = scancode;
    keyIdx++;
}

void keyboardInterrupt() {
    __asm {
        cli

        push ax
        push bx
        push cx
        push dx
        push si
        push di
        push bp
        push ds
        push es


        push ds
        mov ax, seg curDS
        mov ds, ax
        mov ax, curDS
        mov ds, ax

        mov dx, 0x60
        in al, dx
        
        push ax

        extern _updateKeyboardStatus:near
        call _updateKeyboardStatus

        add sp, 2

        // say interrupt comlete
        mov dl, 0x20
        mov al, 0x20
        out dl, al
        
        // notify keyboard that read interrupt
        mov dl, 0x61
        in al, dl
        mov ah, al
        or al, 0x80
        out dl, al
        mov al, ah
        out dl, al
        
        pop ds

        pop es
        pop ds
        pop bp
        pop di
        pop si
        pop dx
        pop cx
        pop bx
        pop ax


        sti
        iret
    }
}

void hookInterrupt() {
    void (far *func_ptr)(void) = keyboardInterrupt;
    unsigned int segment = FP_SEG(func_ptr);
    unsigned int offset = FP_OFF(func_ptr);
    struct SREGS sregs;
    segread(&sregs);
    curDS = sregs.ds;

    // store old interrupt address
    __asm {
        mov ah, 0x35
        mov al, 0x09
        int 0x21
        mov oldSegment, es
        mov oldOffset, bx
    }

    // set new interrupt address
    __asm {
        push ds
        mov al, 0x09
        mov dx, offset
        mov ds, segment
        mov ah, 0x25
        int 0x21
        pop ds
    }
}

void sendLightStatusByte() {
    unsigned char status;

    do {
        __asm {
            mov dl, 0x64
            in al, dl
            mov status, al
        }
    } while (status & 0x02);

    __asm {
        mov dl, 0x60
        mov al, 0xED
        out dl, al
    }

    do {
        __asm {
            mov dl, 0x64
            in al, dl
            mov statusByte, al
        }
    } while (status & 0x02);

    __asm {
        mov dl, 0x60
        mov al, scrollNumberCapsLockStatusByte
        out dl, al
    }
}

void setCapsLockLightStatus(bool val) {
    if (val) {
        scrollNumberCapsLockStatusByte |= 0b00000100;
    } else {
        scrollNumberCapsLockStatusByte &= 0b00000011;
    }
    sendLightStatusByte();
}

void setScrollLockLightStatus(bool val) {
    if (val) {
        scrollNumberCapsLockStatusByte |= 0b00000100;
    } else {
        scrollNumberCapsLockStatusByte &= 0b00000011;
    }
    sendLightStatusByte();
}

void setNumLockLightStatus(bool val) {
    if (val) {
        scrollNumberCapsLockStatusByte |= 0b00000100;
    } else {
        scrollNumberCapsLockStatusByte &= 0b00000011;
    }
    sendLightStatusByte();
}