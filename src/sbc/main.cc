#include "FilerMenu.h"

#include <cstdio>

int main(int argc, char** argv) {
  sbc::FilerMenu mainMenu;
  for (;;) {
    mainMenu.schedule();
  }
  return 0;
}
