#include <stdint.h>
#include "memmgt.h"

#define NULL ((void*)0)

#pragma pack(push, 1)
struct InterruptDescriptor {
   uint16_t offset_1;        // offset bits 0..15
   uint16_t selector;        // a code segment selector in GDT or LDT
   uint8_t  zero;            // unused, set to 0
   uint8_t  type_attributes; // gate type, dpl, and p fields
   uint16_t offset_2;        // offset bits 16..31
};

#pragma pack(pop)

struct InterruptDescriptor* idt;

// initialize idt variable and set cpu's idt register to point to it
void initIDT() {
    idt = kcalloc(sizeof(struct InterruptDescriptor)*256);
    asm volatile (
        "lidt %0"
        : 
        : "m" (idt)
        : 
    );
}

void setIDTValue(uint8_t intNum, uint32_t offsetAddress) {
    if (idt==NULL) initIDT();
    
    struct InterruptDescriptor new =  
    idt[intNum] 
}