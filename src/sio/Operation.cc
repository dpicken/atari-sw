#include "Operation.h"

asm (
    ".text            \n"
    ".global sys_sio  \n"
    "sys_sio:         \n"
    "  jsr $E459      \n"
    "  rts            \n");
