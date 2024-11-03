#ifndef sbc_FilerMenu_h
#define sbc_FilerMenu_h

#include "sio/FileSystem.h"

#include "menu/Menu.h"

namespace sbc {

class FilerMenu : public menu::Menu {
public:
  FilerMenu();

  bool hasParentMenu() const override;
  bool hasPreviousPage() const override;
  bool hasNextPage() const override;

  void onPreviousPage() override;
  void onNextPage() override;
  Menu* onParentMenu() override;

  Menu* onSelect() override;
  Menu* onAction0() override;
  Menu* onAction1() override;

private:
  void onDirectoryChange();
  void onError(const char* message);

  void getCurrentDir();
  void loadPage();

  unsigned int m_pageIndex;

  sio::FileSystemGetCurrentDir m_getCurrentDir;
  sio::FileSystemReadDir m_readDir;
};

} // namespace sbc

#endif // ifndef sbc_FilerMenu_h
