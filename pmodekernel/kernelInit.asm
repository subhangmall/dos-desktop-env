[bits 32]               
global pModeMain     
extern kmain            

pModeMain:
    mov ax, 0x10        ; data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x90000    ; setup stack
    call kmain          ; kump to C

hang:
    hlt
    jmp hang