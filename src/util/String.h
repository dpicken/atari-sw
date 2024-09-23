#ifndef util_String_h
#define util_String_h

#include "Vector.h"

#include <cstdio>
#include <string.h>

namespace util {

template<std::size_t CapacityV>
class String {
public:
  static constexpr std::size_t max_size = CapacityV;

  static constexpr std::size_t terminator_size_v = 1;
  static constexpr char terminator_char = '\0';

  using buffer_type = Vector<char, CapacityV + terminator_size_v>;

  using value_type = typename buffer_type::value_type;
  using size_type = typename buffer_type::size_type;

  using const_iterator = typename buffer_type::const_iterator;
  using iterator = typename buffer_type::iterator;

  String() : String("") {
  }

  explicit String(const value_type* str) : String(str, strlen(str)) {
  }

  template<typename StringType>
  explicit String(const StringType& str) : String(str.data(), str.size()) {
  }

  String(const value_type* data, size_type count) {
    for (auto it = data, end = data + count; it != end && m_buffer.size() != max_size; ++it) {
      m_buffer.push_back(*it);
    }
    m_buffer.push_back(terminator_char);
  }

  bool empty() const {
    return m_buffer.size() == terminator_size_v;
  }

  size_type size() const {
    return m_buffer.size() - terminator_size_v;
  }

  const_iterator begin() const {
    return m_buffer.begin();
  }

  const_iterator end() const {
    return begin() + size();
  }

  iterator begin() {
    return m_buffer.begin();
  }

  iterator end() {
    return begin() + size();
  }

  void inverse() {
    for (auto& c : *this) {
      if ((c & 0x80) == 0) {
        c |= 0x80;
      } else {
        c &= 0x7F;
      }
    }
  }

  String& pad(char p = ' ') {
    m_buffer.pop_back();
    m_buffer.resize(max_size, p);
    m_buffer.push_back('\0');
    return *this;
  }

  void clear() {
    m_buffer.clear();
    m_buffer.push_back('\0');
  }

  const value_type* c_str() const {
    return &*m_buffer.begin();
  }

  void print(bool inverse = false) const {
    for (auto c : *this) {
      if (!inverse) {
        std::putchar(c);
      } else {
        std::putchar(c | 0x80);
      }
    }
  }

  void printLine(bool inverse = false) const {
    print(inverse);
    std::putchar('\n');
  }

private:
  buffer_type m_buffer;
};

} // namespace util

#endif // ifndef util_String_h
