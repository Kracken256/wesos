ENTRY(_start)

/* Define Program Headers for memory protection */
PHDRS
{
    boot PT_LOAD FLAGS(5);   /* Read + Execute */
    text PT_LOAD FLAGS(5);   /* Read + Execute */
    rodata PT_LOAD FLAGS(4); /* Read-Only */
    data PT_LOAD FLAGS(6);   /* Read + Write */
}

SECTIONS
{
    /* The kernel starts at 1MB */
    . = 1M;
    __kernel_start = .;

    /* --- BOOT SECTION --- */
    /* Must be first and at a low file offset for Multiboot2 */
    .boot : ALIGN(4K)
    {
        __boot_start = .;
        KEEP(*(.multiboot2_header))
        __boot_end = .;
    } :boot

    /* --- TEXT SECTION (Code) --- */
    .text : ALIGN(4K)
    {
        __text_start = .;
        *(.text .text.*)
        __text_end = .;
    } :text

    /* --- RODATA SECTION (Constants) --- */
    .rodata : ALIGN(4K)
    {
        __rodata_start = .;
        *(.rodata .rodata.*)
        __rodata_end = .;
    } :rodata

    /* --- DATA SECTION (Initialized Variables) --- */
    .data : ALIGN(4K)
    {
        __data_start = .;
        *(.data .data.*)
        __data_end = .;
    } :data

    /* --- BSS SECTION (Uninitialized Variables) --- */
    .bss : ALIGN(4K)
    {
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;
    } :data

    /* --- STACK --- */
    /* You can define the stack here or in assembly. 
       If here, your bootloader or start code can find it. */
    .stack : ALIGN(4K)
    {
        __stack_bottom = .;
        . += 0x8000; /* 32KB stack */
        __stack_top = .;
    } :data

    __kernel_end = .;

    /* --- DEBUG & DISCARD --- */
    /* Keep debug symbols for addr2line/gdb, but discard trash */
    /DISCARD/ :
    {
        *(.comment)
        *(.note*)
        *(.eh_frame) /* Optional: discard if not doing panic unwinding */
    }
}