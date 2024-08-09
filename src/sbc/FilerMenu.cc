#include "FilerMenu.h"

#include "sio/AtariControl.h"

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
  return false;
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
  return this;
}

menu::Menu* sbc::FilerMenu::onSelect() {
  auto itemIndex = getCurrentItemIndex();
  auto dirEntryIndex = (m_pageIndex * max_item_count) + itemIndex;
  sio::FileSystemSelectDirEntry::execute(dirEntryIndex);
  sio::AtariControlPowerCycle::execute();
  return this;
}

menu::Menu* sbc::FilerMenu::onAction0() {
  return this;
}

menu::Menu* sbc::FilerMenu::onAction1() {
  return this;
}

void sbc::FilerMenu::getCurrentDir() {
  if (!m_getCurrentDir.execute()) {
    setTitle(title_type(m_getCurrentDir.makeStatusMessage("(get current dir failed)")));
    return;
  }

  setTitle(title_type(m_getCurrentDir.data));
}

void sbc::FilerMenu::loadPage() {
  clearItems();

  auto dirEntryIndex = (m_pageIndex * max_item_count);

#define SBC_NON_SIO_TEST 0
#if !SBC_NON_SIO_TEST
  if (!m_readDir.execute(dirEntryIndex)) {
    appendItem(item_type(m_readDir.makeStatusMessage("(readdir failed)")));
    return;
  }

  static_assert (decltype(m_readDir.data)::max_entry_count == max_item_count);
  for (auto it = m_readDir.data.begin(), end = m_readDir.data.end(); it != end; ++it) {
    appendItem(item_type(it->name()));
  }
#else
  static constexpr const char* testDir[] = {
    "!sbc-menu.atr","air-strike-ii.atr","asteroids.atr","caverns-of-mars-2.atr","caverns-of-mars.atr","dark-chambers-cheat.atr","dogdaze-deluxe.atr","dos-2.0s.atr","dos-2.5.atr","dos-xe-1.0.atr","leapster.atr","mashed-turtles.atr","missile-command.atr","pitfall-i.atr","pole-position.atr","realm-of-impossibility.atr","savage-pond.atr","schreckenstein.atr","snowball-fight.atr","spelunker.atr","tekblast.atr","world-karate-championship.atr"
  };
  static constexpr auto testDirEntryCount = sizeof(testDir) / sizeof(*testDir);
  for (auto itemCount = 0; itemCount != max_item_count && dirEntryIndex != testDirEntryCount; ++itemCount, ++dirEntryIndex) {
    appendItem(item_type(testDir[dirEntryIndex]));
  }
  m_readDir.data.setEOS(dirEntryIndex == testDirEntryCount);
#endif
}
