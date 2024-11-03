#include "FilerMenu.h"

#include "sio/AtariControl.h"

#define DEFINE_OPERATION_FAILED_ERROR_MESSAGE(operationDescription) "op failed: " operationDescription

sbc::FilerMenu::FilerMenu()
  : m_pageIndex(0) {
  getCurrentDir();
  loadPage();
}

bool sbc::FilerMenu::hasPreviousPage() const {
  return m_pageIndex > 0;
}

bool sbc::FilerMenu::hasNextPage() const {
  return !m_readDir.data.eos();
}

bool sbc::FilerMenu::hasParentMenu() const {
  return true;
}

void sbc::FilerMenu::onPreviousPage() {
  --m_pageIndex;
  loadPage();
}

void sbc::FilerMenu::onNextPage() {
  ++m_pageIndex;
  loadPage();
}

menu::Menu* sbc::FilerMenu::onParentMenu() {
  if (!sio::FileSystemSelectParentDir::execute()) {
    onError(DEFINE_OPERATION_FAILED_ERROR_MESSAGE("select parent dir"));
    return this;
  }

  onDirectoryChange();
  return this;
}

menu::Menu* sbc::FilerMenu::onSelect() {
  auto itemIndex = getCurrentItemIndex();
  auto dirEntryIndex = (m_pageIndex * max_item_count) + itemIndex;

  if (!sio::FileSystemSelectDirEntry::execute(dirEntryIndex)) {
    onError(DEFINE_OPERATION_FAILED_ERROR_MESSAGE("select dir entry"));
    return this;
  }

  if (m_readDir.data[itemIndex].isDirectory()) {
    onDirectoryChange();
  } else {
    if (!sio::AtariControlPowerCycle::execute()) {
      onError(DEFINE_OPERATION_FAILED_ERROR_MESSAGE("power cycle"));
      return this;
    }
  }

  return this;
}

menu::Menu* sbc::FilerMenu::onAction0() {
  return this;
}

menu::Menu* sbc::FilerMenu::onAction1() {
  return this;
}

void sbc::FilerMenu::onDirectoryChange() {
  getCurrentDir();
  m_pageIndex = 0;
  loadPage();
}

void sbc::FilerMenu::onError(const char* message) {
  setMessage(message_type(message));
}

void sbc::FilerMenu::getCurrentDir() {
  if (!m_getCurrentDir.execute()) {
    onError(DEFINE_OPERATION_FAILED_ERROR_MESSAGE("get current dir"));
    return;
  }

  setTitle(title_type(m_getCurrentDir.data));
}

void sbc::FilerMenu::loadPage() {
  clearItems();

  auto dirEntryIndex = (m_pageIndex * max_item_count);

  if (!m_readDir.execute(dirEntryIndex)) {
    onError(DEFINE_OPERATION_FAILED_ERROR_MESSAGE("read dir"));
    return;
  }

  if (m_readDir.data.empty()) {
    setMessage(message_type("(empty directory)"));
  }

  static_assert (decltype(m_readDir.data)::max_entry_count == max_item_count);
  for (auto it = m_readDir.data.begin(), end = m_readDir.data.end(); it != end; ++it) {
    appendItem(item_type(it->name()));
  }
}
