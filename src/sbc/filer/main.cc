#include "Menu.h"

#include <cstdio>

int main(int argc, char** argv) {
  sbc::filer::Menu menu;
  for (;;) {
    menu.schedule();
  }
  return 0;
}
