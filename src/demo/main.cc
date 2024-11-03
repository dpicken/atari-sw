#include "DemoMenu.h"

#include <cstdio>

int main(int argc, char** argv) {
  demo::MainMenu mainMenu;
  for (;;) {
    mainMenu.schedule();
  }
  return 0;
}
