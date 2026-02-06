#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 24

bool loadMemMapAtAddress(uint16_t seg, uint16_t off) {
    // EDX is signature; FIX
    __asm {
        mov edx, seg
        mov es, edx
        mov di, off
        mov ebx, 0x0000

        

        getMMap:
            mov eax, 0xE820
            mov edx, 0x534D4150
            mov ecx, BUFFER_SIZE

            int 15h

            // one way a BIOS signals the end
            jc end

            // check for error by seeing if the signature is still correct
            cmp eax, 0x534D4150
            jne end
            
            // add 32 bytes to the offset (for speed its not 24)
            add di, 32
            jnc noOverflow

            add es, 0x1000 // only called if overflow
            
            noOverflow:

            
            // if bx == 0, end
            test ebx, ebx
            je end
            

            // if bx not 0, restart
            jmp getMMap

        end:
    }
}
