/* Atari 8-bit DOS boot loader linker script. */

ATR_BLOCK_SIZE = 16;

BOOT_SECTOR_SIZE = 128;
BOOT_SECTOR_COUNT = 4;

BOOT_LOAD_ADDRESS = 0x0700;
BOOT_LOAD_ADDRESS_END = BOOT_LOAD_ADDRESS + (BOOT_SECTOR_SIZE * BOOT_SECTOR_COUNT);

OPCODE_JMP = 0x4C;

DOS_BOOT_HEADER_SIZE = 9;

/* Provide 16 imaginary (zero page) registers located in 0xcb - 0xea range. */
__rc0 = 0x80;
INCLUDE imag-regs.ld
ASSERT(__rc31 == 0x9f, "Inconsistent zero page map.")

MEMORY {
    zp : ORIGIN = __rc31 + 1, LENGTH = 0x100 - (__rc31 + 1)
    ram (rw) : ORIGIN = BOOT_LOAD_ADDRESS + DOS_BOOT_HEADER_SIZE, LENGTH = ((BOOT_SECTOR_SIZE * BOOT_SECTOR_COUNT) - DOS_BOOT_HEADER_SIZE)
}

REGION_ALIAS("c_readonly", ram)
REGION_ALIAS("c_writeable", ram)

SECTIONS {
  INCLUDE c.ld
}

OUTPUT_FORMAT {
    /* ATR header. */
    SHORT(0x0296) /* Magic. */
    SHORT((BOOT_SECTOR_SIZE * BOOT_SECTOR_COUNT)  / ATR_BLOCK_SIZE)
    SHORT(BOOT_SECTOR_SIZE)
    SHORT(0x0000)
    SHORT(0x0000)
    SHORT(0x0000)
    SHORT(0x0000)
    SHORT(0x0000)

    /* DOS boot header. */
    BYTE(0x00) /* Flags. */
    BYTE(BOOT_SECTOR_COUNT)
    SHORT(BOOT_LOAD_ADDRESS)
    SHORT(_start)
    BYTE(OPCODE_JMP)
    SHORT(dos_boot_start)

    /** Application. */
    FULL(ram)
}
