#include "Menu.h"

#include <cstdint>
#include <cstdio>

#include <atari.h>

namespace {

menu::Menu::action_type makeAction(char key, const menu::Menu::action_name_type& name) {
  menu::Menu::action_type action;
  action.first = key;
  (action.second = name).pad();
  return action;
}

} // namespace

menu::Menu::Menu()
  : m_action0(makeAction('\0', action_name_type("")))
  , m_action1(makeAction('\0', action_name_type("")))
  , m_currentItem(m_items.begin())
  , m_renderRequired(true) {
}

void menu::Menu::setTitle(const title_type& title) {
  (m_title = title).pad();
}

void menu::Menu::setAction0(char key, const action_name_type& name) {
  m_action0 = makeAction(key, name);
}

void menu::Menu::setAction1(char key, const action_name_type& name) {
  m_action1 = makeAction(key, name);
}

bool menu::Menu::canAppendItem() const {
  return m_items.size() < m_items.capacity();
}

void menu::Menu::appendItem(const item_type& item) {
  m_items.emplace_back(item).pad();
  notifyRenderRequired();
}

void menu::Menu::clearItems() {
  m_items.clear();
  notifyRenderRequired();
}

void printBlankLine() {
  static const auto line = util::String<36>().pad();
  line.printLine();
}

void printPageNavigationLine(bool hasPreviousPage, bool hasNextPage) {
  if (hasPreviousPage && hasNextPage) {
    static const auto line = util::String<36>(" < Previous                  Next > ").pad();
    line.printLine();
  } else if (hasPreviousPage) {
    static const auto line = util::String<36>(" < Previous").pad();
    line.printLine();
  } else if (hasNextPage) {
    static const auto line = util::String<36>("                             Next > ").pad();
    line.printLine();
  } else {
    printBlankLine();
  }
}

void printAction(const menu::Menu::action_type& action) {
  std::putchar(action.first);
  std::putchar(':');
  action.second.print();
}

void printActionLine(const menu::Menu::action_type& action0, const menu::Menu::action_type& action1) {
  static const menu::Menu::action_type actionBack = makeAction('E', menu::Menu::action_name_type("Back"));
  static const menu::Menu::action_type actionSelect = makeAction('R', menu::Menu::action_name_type("Select"));
  printAction(actionBack);
  printAction(action0);
  std::putchar(' ');
  std::putchar(' ');
  printAction(action1);
  std::putchar(' ');
  std::putchar(' ');
  printAction(actionSelect);
}

void menu::Menu::render() const {
  OS.rowcrs = 0;
  OS.colcrs = 2;

  m_title.printLine();
  printBlankLine();
  for (auto it = m_items.begin(), end = m_items.end(); it != end; ++it) {
    it->printLine(it == m_currentItem);
  }
  printPageNavigationLine(hasPreviousPage(), hasNextPage());
  for (auto it = m_items.size(), end = m_items.capacity(); it != end; ++it) {
    printBlankLine();
  }
  printBlankLine();
  printBlankLine();
  printActionLine(m_action0, m_action1);
  moveCursorToCurrentItem();
}

void menu::Menu::schedule() {
  auto c = OS.ch;
  OS.ch = KEY_NONE;
  handleKeyPress(c);
  if (m_renderRequired) {
    render();
    m_renderRequired = false;
  }
}

void menu::Menu::notifyRenderRequired() {
  m_renderRequired = true;
}

void menu::Menu::handleKeyPress(char c) {
  switch (c) {
    case KEY_UP:
      handleUp();
      return;

    case KEY_DOWN:
      handleDown();
      return;

    case KEY_LEFT:
      handleLeft();
      return;

    case KEY_RIGHT:
      handleRight();
      return;

    case KEY_RETURN:
      onSelect();
      return;

    case KEY_ESC:
      handleBack();
      return;
  }

  if (c == m_action0.first) {
    onAction0();
    return;
  }

  if (c == m_action1.first) {
    onAction1();
    return;
  }
}

void menu::Menu::handleUp() {
  if (m_currentItem > m_items.begin()) {
    setCurrentItem(m_currentItem - 1);
    return;
  }

  handleLeft();
}

void menu::Menu::handleDown() {
  if (m_items.empty()) {
    return;
  }

  auto lastItem = m_items.end() - 1;
  if (m_currentItem < lastItem) {
    setCurrentItem(m_currentItem + 1);
    return;
  }

  handleRight();
}

void menu::Menu::handleLeft() {
  if (!hasPreviousPage()) {
    return;
  }

  setCurrentItem(m_items.end());
  onPreviousPage();
  if (!m_items.empty()) {
    setCurrentItem(m_items.end() - 1);
  } else {
    setCurrentItem(m_items.end());
  }
}

void menu::Menu::handleRight() {
  if (!hasNextPage()) {
    return;
  }

  setCurrentItem(m_items.end());
  onNextPage();
  setCurrentItem(m_items.begin());
}

void menu::Menu::handleBack() {
  if (!hasParentMenu()) {
    return;
  }

  onParentMenu();
  notifyRenderRequired();
}

menu::Menu::item_index_type menu::Menu::getCurrentItemIndex() const {
  return m_currentItem - m_items.begin();
}

void menu::Menu::setCurrentItem(item_container_type::iterator it) {
  if (m_currentItem != m_items.end()) {
    OS.rowcrs = 2 + (m_currentItem - m_items.begin());
    OS.colcrs = 2;
    m_currentItem->printLine();
  }

  m_currentItem = it;

  if (m_currentItem != m_items.end()) {
    OS.rowcrs = 2 + (m_currentItem - m_items.begin());
    OS.colcrs = 2;
    m_currentItem->print(true);
    moveCursorToCurrentItem();
  }
}

void menu::Menu::moveCursorToCurrentItem() const {
  if (m_currentItem == m_items.end()) {
    return;
  }

  OS.rowcrs = 2 + (m_currentItem - m_items.begin());
  OS.colcrs = 2 + m_currentItem->size() - 1;
  std::putchar(*(m_currentItem->end() - 1));
  std::putchar('\b');
}
