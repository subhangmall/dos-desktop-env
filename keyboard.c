#include <stdbool.h>

unsigned char scrollNumberCapsLockStatusByte;

unsigned char oldOffset, oldSegment;

void keyboardDriver() {
    __asm {
        mov al, 0xF4
        mov dl, 0x64
        out dl, al
    }
}

void updatekeyboardStatus(unsigned char interrupt) {

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

        mov dx, 0x60
        in al, dx
        
        push ax

        extern _updateKeyboardStatus:near
        call _updateKeyboardStatus

        add sp, 2

        mov dl, 0x20
        mov al, 0x20
        out dl, al

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
            mov statusByte, al
        }
    } while (status & 0x02);

    __asm {
        mov dl, 0x64
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

bool setCapsLockLightStatus(bool val) {
    if (val) {
        scrollNumberCapsLockStatusByte |= 0b00000100;
    } else {
        scrollNumberCapsLockStatusByte &= 0b00000011;
    }
    sendLightStatusByte();
}

bool setScrollLockLightStatus(bool val) {
    if (val) {
        scrollNumberCapsLockStatusByte |= 0b00000100;
    } else {
        scrollNumberCapsLockStatusByte &= 0b00000011;
    }
    sendLightStatusByte();
}

bool setNumLockLightStatus(bool val) {
    if (val) {
        scrollNumberCapsLockStatusByte |= 0b00000100;
    } else {
        scrollNumberCapsLockStatusByte &= 0b00000011;
    }
    sendLightStatusByte();
}