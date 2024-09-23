#ifndef util_Pair_h
#define util_Pair_h

namespace util {

template<typename T1, typename T2>
struct Pair {
  using first_type = T1;
  using second_type = T2;

  first_type first;
  second_type second;
};

} // namespace util

#endif // ifndef util_Pair_h
