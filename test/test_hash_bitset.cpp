#include "Hasher.h"
#include <bitset>
#include <cstdint>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

using Bits = std::vector<bool>;

// generates a 64bit random integer
std::uint64_t random_integer(uint64_t min, uint64_t max) {
  static thread_local std::mt19937_64 generator{std::random_device{}()};
  std::uniform_int_distribution<std::uint64_t> distribution(min, max);
  return distribution(generator);
}

std::vector<bool> rand_bitvec() {
  uint64_t num = random_integer(0, 0xFFFFFFFFFFFFFFFF);
  auto k = std::bitset<64>(num);
  std::vector<bool> bit_vec(64);
  for (int i = 0; i < 64; ++i) {
    bit_vec[i] = k.test(i);
  }
  return bit_vec;
}

const auto bitvec_hasher = Hasher<std::vector<bool>>();

int main() {

  int offset = 0;
  const uint64_t Nitems = 1E6;

  std::unordered_set<uint64_t> hash_values;
  std::unordered_set<Bits> unique_items;
  for (uint64_t i = 0; i < Nitems; i++) {
    auto v = rand_bitvec();
    unique_items.insert(rand_bitvec());
  }

  for (const auto &el : unique_items) {
    hash_values.insert(bitvec_hasher(el, offset));
  }

  std::cout << "# of unique bitsets = " << unique_items.size()
            << " #of unique hash values = " << hash_values.size() << "\n";
  std::cout << "Ratio : "
            << double(unique_items.size()) / double(hash_values.size()) << "\n";
  return 0;
}