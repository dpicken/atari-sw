.global dos_boot_init
dos_boot_init:
; Update MEMLO and APPMHI
  lda #<[BOOT_LOAD_ADDRESS_END]
  sta 0x2E7
  sta 0x14
  lda #>[BOOT_LOAD_ADDRESS_END]
  sta 0x2E8
  sta 0x15
; Set DOSVEC
  lda #<_start
  sta 0x0A
  lda #>_start
  sta 0x0B
; Success
  clc
  rts
