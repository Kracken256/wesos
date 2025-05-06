ENTRY(_start)

SECTIONS
{
  . = 0x100000; /* Load at 1MB */

  wesos_kern_text_start = .;
  .text : {
    KEEP(*(.text._start))   /* ensure it's not discarded */
    *(.text)
  }
  wesos_kern_text_end = .;

  wesos_kern_rodata_start = .;
  .rodata : {
    BEG_CXX_CTORS_GLOBAL = .;
    KEEP(*( .init_array ));
    KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
    END_CXX_CTORS_GLOBAL = .;

    *(.rodata)
  }
  wesos_kern_rodata_end = .;
  
  wesos_kern_data_start = .;
  .data : {
    *(.data)
  }
  wesos_kern_data_end = .;

  wesos_kern_bss_start = .;
  .bss : {
    *(.bss)
    *(COMMON)
  }
  wesos_kern_bss_end = .;

  /DISCARD/ : {
    *(.comment)
    *(.note*)
    *(.got*)
    *(.igot*)
    *(.iplt*)
    *(.rela*)
  }
}