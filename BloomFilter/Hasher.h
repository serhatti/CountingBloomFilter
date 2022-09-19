/* Default hasher for the counting bloom filter.
A desired number of  hash functions are obtained by
adding offsets to the items to be hashed.
Eg : h0(x) , h1(x)=h0(x(+)1) ... hi(x)=h1(x+(i))
even and odd indices are treated differently

author : Serhat Istin
       : istins@gmail.com
*/

#ifndef DEFAULT_HASHER_H
#define DEFAULT_HASHER_H

#include <cstdint>

template <class T>
class Hasher {
 public:
  uint64_t operator()(const T&, int offset = 0) const;
};

template <class T>
uint64_t Hasher<T>::operator()(const T& v, int offset) const {
  uint64_t init = v.size();
  int p = 0;
  for (auto el : v) {
    if (p % 2 == 0)
      el += 0x85ebca6b * (offset + 1);
    else
      el += 0xc2b2ae35 * (offset - 1);
    init ^= el + 0x9e3779b9 + (init << 6) + (init >> 2);
    p++;
  }
  return init;
}
#endif