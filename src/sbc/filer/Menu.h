#ifndef sbc_filer_Menu_h
#define sbc_filer_Menu_h

#include "sbc/sio/FileSystem.h"

#include "menu/Menu.h"

namespace sbc { namespace filer {

class Menu : public ::menu::Menu {
public:
  Menu();

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

} } // namespace sbc::filer

#endif // ifndef sbc_filer_Menu_h
