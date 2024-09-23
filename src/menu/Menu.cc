#include "Menu.h"

#include <ctype.h>
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

const auto null_action = makeAction(' ', menu::Menu::action_name_type(""));

menu::Menu::Menu()
  : m_action0(null_action)
  , m_action1(null_action)
  , m_currentItem(m_items.begin())
  , m_renderRequired(true) {
}

void menu::Menu::setTitle(const title_type& title) {
  (m_title = title).pad();
  notifyRenderRequired();
}

void menu::Menu::setAction0(char key, const action_name_type& name) {
  m_action0 = makeAction(key, name);
  notifyRenderRequired();
}

void menu::Menu::setAction1(char key, const action_name_type& name) {
  m_action1 = makeAction(key, name);
  notifyRenderRequired();
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
  m_currentItem = m_items.begin();
  notifyRenderRequired();
}

void menu::Menu::setMessage(const message_type& message) {
  (m_message = message).pad();
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
  std::putchar(action.first != null_action.first ? ':' : ' ');
  action.second.print();
}

void printMessageLine(const menu::Menu::message_type& message) {
  message.printLine();
}

void printActionLine(bool hasParentMenu, const menu::Menu::action_type& action0, const menu::Menu::action_type& action1, bool hasItems) {
  static const menu::Menu::action_type actionBack = makeAction(0x1B, menu::Menu::action_name_type("Back"));
  static const menu::Menu::action_type actionSelect = makeAction(0x14, menu::Menu::action_name_type("Select"));
  if (hasParentMenu) {
    std::putchar(0x1B);
    printAction(actionBack);
  } else {
    printAction(null_action);
  }
  printAction(action0);
  std::putchar(' ');
  std::putchar(' ');
  printAction(action1);
  std::putchar(' ');
  std::putchar(' ');
  if (hasItems) {
    printAction(actionSelect);
  } else {
    printAction(null_action);
  }
}

void menu::Menu::render() {
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
  printMessageLine(m_message);
  printActionLine(hasParentMenu(), m_action0, m_action1, !m_items.empty());
  moveCursorToCurrentItem();

  m_message.clear();
  m_message.pad();
}

void menu::Menu::schedule() {
  if (OS.keydel == 3) {
    auto keyCode = OS.ch;
    OS.ch = KEY_NONE;
    handleKeyPress(keyCode);
  }
  if (m_renderRequired) {
    render();
    m_renderRequired = false;
  }
}

void menu::Menu::notifyRenderRequired() {
  m_renderRequired = true;
}

void menu::Menu::handleKeyPress(unsigned char keyCode) {
  switch (keyCode) {
    case KEY_UP:
    case KEY_DASH:
      handleUp();
      return;

    case KEY_DOWN:
    case KEY_EQUALS:
      handleDown();
      return;

    case KEY_LEFT:
    case KEY_PLUS:
    case KEY_LESSTHAN:
      handleLeft();
      return;

    case KEY_RIGHT:
    case KEY_ASTERISK:
    case KEY_GREATERTHAN:
      handleRight();
      return;

    case KEY_RETURN:
      handleSelect();
      return;

    case KEY_ESC:
      handleBack();
      return;
  }

  auto c = toupper(OS.keydef[keyCode]);

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

  onNextPage();
  setCurrentItem(m_items.begin());
}

void menu::Menu::handleSelect() {
  if (m_items.empty()) {
    return;
  }

  onSelect();
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
