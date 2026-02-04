ENTRY(main)
OUTPUT_FORMAT(binary)

SECTIONS
{
    /* This tells the linker the code 'lives' at 0x1000, 
       but doesn't necessarily pad the file from 0x0. */
    . = 0x1000;

    .text : {
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
        *(COMMON)
    }
}