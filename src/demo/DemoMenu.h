#ifndef demo_DemoMenu_h
#define demo_DemoMenu_h

#include "menu/Menu.h"

#include <cstdint>

namespace demo {

class MainMenu : public menu::Menu {
public:
  MainMenu();

  bool hasPreviousPage() const override;
  bool hasNextPage() const override;
  bool hasParentMenu() const override;

  void onPreviousPage() override;
  void onNextPage() override;
  Menu* onParentMenu() override;

  Menu* onSelect() override;
  Menu* onAction0() override;
  Menu* onAction1() override;

private:
  void reload();

  std::uint8_t m_currentDepth;
  std::uint8_t m_currentPage;
};

} // namespace demo

#endif // ifndef demo_DemoMenu_h
