OUTPUT_ARCH("riscv")
ENTRY(rvtest_entry_point)

MEMORY {               
  sram (rwx) : ORIGIN = 0x001F8000, LENGTH = 0x4000
  ddr (rwx) : ORIGIN = 0x88020000000000, LENGTH = 0x10000000  
}

SECTIONS
{
 
  PROVIDE(_ram = ORIGIN(ddr));
  PROVIDE(_ram_end = _ram + LENGTH(ddr));

  .text.init ALIGN((ORIGIN(ddr) + 0x0), 64) : AT(ALIGN((ORIGIN(ddr) + 0x0), 64)) {
    PROVIDE(_ftext = .);
    *(.text.init)
    PROVIDE(_etext = .);
  }

  .text ALIGN((ADDR(.text.init) + SIZEOF(.text.init)), 64) : AT(ALIGN((LOADADDR(.text.init) + SIZEOF(.text.init)), 64)) {
    *(.text)
  } 

  .tohost ALIGN((ADDR(.text) + SIZEOF(.text)), 64) : AT(ALIGN((LOADADDR(.text) + SIZEOF(.text)), 64)) {
    *(.tohost)
  }

  .data ALIGN((ADDR(.tohost) + SIZEOF(.tohost)), 64) : AT(ALIGN((LOADADDR(.tohost) + SIZEOF(.tohost)), 64)) {
    *(.data)
  } 

  PROVIDE(_data = ADDR(.data));
  PROVIDE(_data_lma = LOADADDR(.data));
  PROVIDE(_edata = ADDR(.data) + SIZEOF(.data));

  .data.string ALIGN((ADDR(.data) + SIZEOF(.data)), 64) : AT(ALIGN((LOADADDR(.data) + SIZEOF(.data)), 64)) {
    *(.data.string)
  }  

  PROVIDE(_fstext = ADDR(.data.string));
  PROVIDE(_data_strings = LOADADDR(.data.string));
  PROVIDE(_estext = ADDR(.data.string) + SIZEOF(.data.string));

  .bss ALIGN((ADDR(.data.string) + SIZEOF(.data.string)), 64) : AT(ALIGN((LOADADDR(.data.string) + SIZEOF(.data.string)), 64)) ALIGN(8) {
    PROVIDE(_fbss = .);
    *(.bss)
    PROVIDE(_ebss = .);
  } 

  PROVIDE(_end = .);

}