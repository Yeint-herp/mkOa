OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5);
    rodata PT_LOAD FLAGS(4);
    data PT_LOAD FLAGS(6);
}

SECTIONS
{
    . = 0xffffffff80000000;

    .text : {
        __mrks_text = .;
        *(.text .text.*)
        __mrke_text = .;
    } :text

    . = ALIGN(0x1000);

    .rodata : {
        __mrks_rodata = .;
        *(.rodata .rodata.*)
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
        __mrke_rodata = .;
    } :rodata

    . = ALIGN(0x1000);

    .data : {
        __mkrs_data = .;
        *(.data .data.*)
        _ mkre_data = .;
    } :data

    . = ALIGN(0x1000);

    .bss : {
        __mrks_bss = .;
        *(COMMON)
        *(.bss .bss.*)
        __mkre_bss = .;
    } :data

    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
        *(.comment)
    }
}

