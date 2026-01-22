#include "Menu.h"

#include "sbc/sio/AtariControl.h"

#define DEFINE_OPERATION_FAILED_ERROR_MESSAGE(operationDescription) "op failed: " operationDescription

sbc::filer::Menu::Menu()
  : m_pageIndex(0) {
  getCurrentDir();
  loadPage();
}

bool sbc::filer::Menu::hasPreviousPage() const {
  return m_pageIndex > 0;
}

bool sbc::filer::Menu::hasNextPage() const {
  return !m_readDir.data.eos();
}

bool sbc::filer::Menu::hasParentMenu() const {
  return true;
}

void sbc::filer::Menu::onPreviousPage() {
  --m_pageIndex;
  loadPage();
}

void sbc::filer::Menu::onNextPage() {
  ++m_pageIndex;
  loadPage();
}

sbc::filer::Menu* sbc::filer::Menu::onParentMenu() {
  if (!sio::FileSystemSelectParentDir::execute()) {
    onError(DEFINE_OPERATION_FAILED_ERROR_MESSAGE("select parent dir"));
    return this;
  }

  onDirectoryChange();
  return this;
}

sbc::filer::Menu* sbc::filer::Menu::onSelect() {
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

sbc::filer::Menu* sbc::filer::Menu::onAction0() {
  return this;
}

sbc::filer::Menu* sbc::filer::Menu::onAction1() {
  return this;
}

void sbc::filer::Menu::onDirectoryChange() {
  getCurrentDir();
  m_pageIndex = 0;
  loadPage();
}

void sbc::filer::Menu::onError(const char* message) {
  setMessage(message_type(message));
}

void sbc::filer::Menu::getCurrentDir() {
  if (!m_getCurrentDir.execute()) {
    onError(DEFINE_OPERATION_FAILED_ERROR_MESSAGE("get current dir"));
    return;
  }

  setTitle(title_type(m_getCurrentDir.data));
}

void sbc::filer::Menu::loadPage() {
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
