/* Default hasher for the counting bloom filter.
author : Serhat Istin
       : istins@gmail.com
*/

#ifndef DEFAULT_HASHER_H
#define DEFAULT_HASHER_H

#include <cstdint>
template <class T> class Hasher {
public:
  uint64_t operator()(const T &, uint64_t offset = 0) const;
};

template <class T>
uint64_t Hasher<T>::operator()(const T &v, uint64_t offset) const {
  uint64_t init = v.size();
  for (uint64_t el : v) {
    if (el & 1) {
      el += offset;
      el ^= el << (offset + 1);
    } else {
      el += 3 * offset;
      el ^= el >> (offset + 1);
    }
    init ^= el + 0x9e3779b9 + (init << 11) + (init >> 4);
  }
  return init;
}
#endif