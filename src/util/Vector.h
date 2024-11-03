#ifndef util_Vector_h
#define util_Vector_h

#include "Array.h"

namespace util {

template<typename T, std::size_t CapacityV>
class Vector {
public:
  using buffer_type = Array<T, CapacityV>;

  using value_type = typename buffer_type::value_type;
  using size_type = typename buffer_type::size_type;
  static constexpr auto capacity_v = buffer_type::size_v;

  using const_iterator = typename buffer_type::const_iterator;
  using iterator = typename buffer_type::iterator;

  Vector() : m_size(0) {
  }

  constexpr size_type capacity() const {
    return m_buffer.size();
  }

  bool empty() const {
    return m_size == 0;
  }

  size_type size() const {
    return m_size;
  }

  const_iterator begin() const {
    return m_buffer.begin();
  }

  const_iterator end() const {
    return begin() + m_size;
  }

  iterator begin() {
    return m_buffer.begin();
  }

  iterator end() {
    return begin() + m_size;
  }

  template<typename... Args>
  value_type& emplace_back(Args... args) {
    return m_buffer[m_size++] = value_type(args...);
  }

  void push_back(const value_type& v) {
    m_buffer[m_size++] = v;
  }

  void pop_back() {
    m_buffer[--m_size] = value_type();
  }

  void clear() {
    m_size = 0;
  }

  void resize(size_type size, const value_type v = value_type()) {
    while (m_size > size) {
      pop_back();
    }
    while (m_size < size) {
      push_back(v);
    }
  }

private:
  buffer_type m_buffer;

  size_type m_size;
};

} // namespace util

#endif // ifndef util_Vector_h
