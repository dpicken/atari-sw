#ifndef util_Array_h
#define util_Array_h

#include <cstddef>

namespace util {

template<typename T, std::size_t SizeV>
class Array {
public:
  using value_type = T;
  using size_type = decltype(SizeV);

  static constexpr auto size_v = SizeV;

  using const_iterator = const value_type*;
  using iterator = value_type*;

  Array() {
  }

  Array(const value_type* it, const value_type* end) {
    auto dst = m_data;
    while (it != end) {
      *dst++ = *it++;
    }
  }

  constexpr size_type size() const {
    return size_v;
  }

  const_iterator begin() const {
    return m_data;
  }

  const_iterator end() const {
    return m_data + size_v;
  }

  iterator begin() {
    return m_data;
  }

  iterator end() {
    return m_data + size_v;
  }

  value_type& operator[](size_type pos) {
    return m_data[pos];
  }

private:
  value_type m_data[size_v];
};

} // namespace util

#endif // ifndef util_Array_h
