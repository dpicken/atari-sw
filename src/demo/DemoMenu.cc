#include "DemoMenu.h"

demo::MainMenu::MainMenu()
  : m_currentDepth(0)
  , m_currentPage(0) {
  setAction0('R', action_name_type("Root"));
  setAction1('L', action_name_type("Leaf"));
  reload();
}

bool demo::MainMenu::hasPreviousPage() const {
  return m_currentPage > 0;
}

bool demo::MainMenu::hasNextPage() const {
  return m_currentPage < 2;
}

bool demo::MainMenu::hasParentMenu() const {
  return m_currentDepth > 0;
}

void demo::MainMenu::onPreviousPage() {
  --m_currentPage;
  reload();
}

void demo::MainMenu::onNextPage() {
  ++m_currentPage;
  reload();
}

menu::Menu* demo::MainMenu::onParentMenu() {
  --m_currentDepth;
  m_currentPage = 0;
  reload();
  return this;
}

menu::Menu* demo::MainMenu::onSelect() {
  if (m_currentDepth < 5) {
    ++m_currentDepth;
    m_currentPage = 0;
    reload();
  }
  return this;
}

menu::Menu* demo::MainMenu::onAction0() {
  m_currentDepth = 0;
  m_currentPage = 0;
  reload();
  return this;
}

menu::Menu* demo::MainMenu::onAction1() {
  m_currentDepth = 5;
  m_currentPage = 0;
  reload();
  return this;
}

void demo::MainMenu::reload() {
  setTitle(title_type("DemoMenu").pad('0' + m_currentDepth));
  clearItems();
  auto maxItemCount = max_item_count / (m_currentDepth + 1);
  for (std::uint8_t i = 0; i != maxItemCount; ++i) {
    appendItem(item_type("MenuItem").pad('A' + (m_currentPage * max_item_count) + i));
  }
  setMessage(message_type("Message").pad('0' + m_currentDepth));
}
