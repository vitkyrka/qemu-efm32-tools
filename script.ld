OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SEARCH_DIR("/usr/lib/arm-none-eabi/lib");
SECTIONS
{
  . = 0x00000000;
  .text :
  {
    *(.text.*)
    *(.dtb.*)
  }

  . = 0x8C000000;
  .kernel : {
    *(.kernel.*)
  }
}
