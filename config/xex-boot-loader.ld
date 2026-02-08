/** Atari 8-bit DOS boot loader linker script. */
ATR_BLOCK_SIZE = 16;

BOOT_SECTOR_SIZE = 128;
BOOT_SECTOR_COUNT = 5;

BOOT_LOAD_ADDRESS = 0x0700;
BOOT_LOAD_ADDRESS_END = BOOT_LOAD_ADDRESS + (BOOT_SECTOR_SIZE * BOOT_SECTOR_COUNT);

OPCODE_JMP = 0x4C;

DOS_BOOT_HEADER_SIZE = 9;

/** Provide minimal imaginary registers and additional zero page space. */
ZP_CONTEXT = zp_context;
__rc0 = ZP_CONTEXT + 0;
__rc1 = ZP_CONTEXT + 1;
__rc18 = ZP_CONTEXT + 2;
__rc19 = ZP_CONTEXT + 3;
ZP_CONTEXT_VAR_BEGIN = ZP_CONTEXT + 4;
ZP_CONTEXT_VAR_SIZE = 10;
ZP_CONTEXT_SIZE = (ZP_CONTEXT_VAR_BEGIN + ZP_CONTEXT_VAR_SIZE) - ZP_CONTEXT;
ASSERT(ZP_CONTEXT_SIZE == zp_context_size, "ZP_CONTEXT_SIZE inconsistent with zp_context_size")

INCLUDE imag-regs.ld

MEMORY {
    zp : ORIGIN = ZP_CONTEXT_VAR_BEGIN, LENGTH = ZP_CONTEXT_VAR_SIZE
    ram (rw) : ORIGIN = BOOT_LOAD_ADDRESS + DOS_BOOT_HEADER_SIZE, LENGTH = ((BOOT_SECTOR_SIZE * BOOT_SECTOR_COUNT) - DOS_BOOT_HEADER_SIZE)
}

REGION_ALIAS("c_readonly", ram)
REGION_ALIAS("c_writeable", ram)

SECTIONS {
  INCLUDE c.ld
}

OUTPUT_FORMAT {
    /** ATR header. */
    SHORT(0x0296) /** Magic. */
    SHORT((BOOT_SECTOR_SIZE * BOOT_SECTOR_COUNT)  / ATR_BLOCK_SIZE)
    SHORT(BOOT_SECTOR_SIZE)
    SHORT(0x0000)
    SHORT(0x0000)
    SHORT(0x0000)
    SHORT(0x0000)
    SHORT(0x0000)

    /** DOS boot header. */
    BYTE(0x00) /* Flags. */
    BYTE(BOOT_SECTOR_COUNT)
    SHORT(BOOT_LOAD_ADDRESS)
    SHORT(_start)
    BYTE(OPCODE_JMP)
    SHORT(dos_boot_start)

    /** Application. */
    FULL(ram)
}
