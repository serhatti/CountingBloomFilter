/*
    author : Serhat Istin
           : istins@gmail.com
*/

#ifndef CUSTOM_HASHER
#define CUSTOM_HASHER

#include <vector>

#include "CustomObject.h"
#include "Hasher.h"

/* includes the specialisation for type: Custom
 */

template <>
uint64_t Hasher<Custom>::operator()(const Custom& itm, int offset) const {
  // Below is not the only possible hashing for <Custom> .
  auto hasher_vec_int = Hasher<std::vector<int>>();
  auto hasher_vec_int32 = Hasher<std::vector<uint64_t>>();
  auto hasher_str = Hasher<std::string>();
  auto hash1 = hasher_vec_int({itm.m_feature2, itm.m_feature3});
  auto hash2 = hasher_str(itm.m_feature1);
  return hasher_vec_int32({hash1 + offset, hash2 - offset});
}

#endif