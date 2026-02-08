os_initad = 0x2E2
os_runad = 0x2E0

.global zp_context
zp_context = 0x80
.global zp_context_size
zp_context_size = 14

.data

zp_context_loader: .space zp_context_size
zp_context_app: .space zp_context_size

fn_pointer: .space 2

.text

; Switch the app into zp_context.
zp_context_switch_app:
  ldx zp_context_size - 1
zp_context_switch_app_loop:
  lda zp_context,x
  sta zp_context_loader,x
  lda zp_context_app,x
  sta zp_context,x
  dex
  bpl zp_context_switch_app_loop
  rts

; Switch the loader into zp_context.
zp_context_switch_loader:
  ldx zp_context_size - 1
zp_context_switch_loader_loop:
  lda zp_context,x
  sta zp_context_app,x
  lda zp_context_loader,x
  sta zp_context,x
  dex
  bpl zp_context_switch_loader_loop
  rts

call_fn_pointer:
  jmp (fn_pointer)
; rts performed by (fn_pointer).

; Call fn_pointer (function with an undefined calling convention).
call_fn:
  jsr zp_context_switch_app
  jsr call_fn_pointer
  jsr zp_context_switch_loader
  rts

; Call INITAD.
.global call_initad
call_initad:
  lda os_initad
  sta fn_pointer
  lda os_initad + 1
  sta fn_pointer + 1
  jsr call_fn
  rts

; Call RUNAD.
.global call_runad
call_runad:
  lda os_runad
  sta fn_pointer
  lda os_runad + 1
  sta fn_pointer+1
  jsr call_fn
  rts
