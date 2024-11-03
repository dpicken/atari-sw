#ifndef menu_Menu_h
#define menu_Menu_h

#include "util/Pair.h"
#include "util/String.h"
#include "util/Vector.h"

namespace menu {

class Menu {
public:
  using title_type = util::String<36>;

  using item_type = util::String<36>;
  using item_container_type = util::Vector<item_type, 18>;
  using item_index_type = item_container_type::size_type;
  static constexpr auto max_item_count = item_container_type::capacity_v;

  using action_name_type = util::String<6>;
  using action_type = util::Pair<char, action_name_type>;

  using message_type = util::String<36>;

  Menu();

  void setTitle(const title_type& title);

  void setAction0(char key, const action_name_type& name);
  void setAction1(char key, const action_name_type& name);

  bool canAppendItem() const;
  void appendItem(const item_type& item);
  void clearItems();

  void setMessage(const message_type& message);

  void schedule();

  void notifyRenderRequired();

  void render();

  void handleKeyPress(unsigned char keyCode);
  void handleUp();
  void handleDown();
  void handleLeft();
  void handleRight();
  void handleSelect();
  void handleBack();

  item_index_type getCurrentItemIndex() const;

  virtual bool hasPreviousPage() const = 0;
  virtual bool hasNextPage() const = 0;
  virtual bool hasParentMenu() const = 0;

  virtual void onPreviousPage() = 0;
  virtual void onNextPage() = 0;
  virtual Menu* onParentMenu() = 0;

  virtual Menu* onSelect() = 0;
  virtual Menu* onAction0() = 0;
  virtual Menu* onAction1() = 0;

private:
  void setCurrentItem(item_container_type::iterator it);
  void moveCursorToCurrentItem() const;

  title_type m_title;

  action_type m_action0;
  action_type m_action1;

  item_container_type m_items;
  item_container_type::iterator m_currentItem;

  message_type m_message;

  bool m_renderRequired;
};

} // namespace menu

#endif // ifndef menu_Menu_h
