/*A Counting Bloom Filter implementation
    author : Serhat Istin
           : istins@gmail.com
*/

#ifndef COUNTING_BLOOM_FILTER_H
#define COUNTING_BLOOM_FILTER_H

#include <math.h>

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

#include "Hasher.h"

/*Given a desired false positive rate and # of expected inserts along with a
counting theta : Get  # of  necessary memory bits and optimal # of hash
functions. If it turns out that the memory is not sufficient for the optimal
values then use the most available ram  (set to 6Gb TODO: make it dynamic )
------------------------------------------------------------------------------------------
1) Kappa star values taken from :
Kim K, Jeong Y, Lee Y, Lee S. Analysis of Counting Bloom Filters Used for Count
thetaing. Electronics. 2019; 8(7):779.
https://doi.org/10.3390/electronics8070779
------------------------------------------------------------------------------------------
2) False positives for regular conting Bloom Filters taken from :
https://arxiv.org/pdf/1804.04777.pdf :

Optimizing Bloom Filter: Challenges, Solutions,
and Comparisons
------------------------------------------------------------------------------------------
*/

/* By defalt this class works with std::string and std::vector<int>
To make it work with a custom type you need to specialize
Hasher<Custom>::operator(). For example see 'CustomHasher.h' and 'test_custom'.

For many custom types, combining  Hasher<std::string> and
Hasher<std::vector<int>> should suffice. But for performance or other reasons
you may want to specialize the Hasher for your custom type.
*/

/*
given a desired false probability rate and  number of items, first compute the
optimal values of width(m) and number of hashes(k) for a non-counting bloom
filter thus finding the number of total necessary bits.

If it turns out that the system has insufficient amount of memory then either
 quit immediately or:
 1) Set the total number of bits to something that is at an acceptable level
  (6Gb for eaxample)
 2) Re-calculate the safe number of items(n) given this amount of memory
  and number of hash functions(k) as calculated previously
 3) warn the client or stop execution if #of insert operations exceeds the #of
items set as in 2.
*/

// each bool in std::vector<bool> takes up 1 bit :
// see : https://en.cppreference.com/w/cpp/container/vector_bool
using Bits = std::vector<bool>;

template <typename T>
class CountingBloomFilter {
 public:
  CountingBloomFilter(uint64_t, int, double = 0.03);
  ~CountingBloomFilter();
  void Insert(const T& item);
  uint64_t Count(const T& item) const;
  uint64_t Counter() const;

 private:
  // m
  uint64_t m_width;
  // k
  int m_num_hashes;
  int m_n_counting_bits;
  uint64_t m_insert_counter;
  // safe #  max inserts.
  uint64_t m_n_max;

  std::unique_ptr<Bits> m_bits;
  Hasher<T> m_hasher;

  void AutoSetParameters(double n, double p = 0.03);
  std::vector<uint64_t> BloomHash(const T&) const;
  std::vector<uint64_t> CounterBitIndices(uint64_t) const;
  void IncrementCounter(const std::vector<uint64_t>&);
};

template <typename T>
CountingBloomFilter<T>::CountingBloomFilter(uint64_t n_expected,
                                            int ncount_bits, double p) {
  m_insert_counter = 0;
  m_n_counting_bits = ncount_bits;
  AutoSetParameters(n_expected, p);
  m_hasher = Hasher<T>();
}

template <typename T>
CountingBloomFilter<T>::~CountingBloomFilter() {}

template <typename T>
void CountingBloomFilter<T>::AutoSetParameters(double n, double p) {
  const double kappa_stars[30] = {
      0.6931, 0.9326, 1.1635, 1.3893, 1.6117, 1.8317, 2.0498, 2.2664,
      2.4818, 2.6963, 2.9099, 3.1228, 3.3351, 3.5469, 3.7582, 3.9690,
      4.1795, 4.3896, 4.5995, 4.8090, 5.0183, 5.2274, 5.4362, 5.6448,
      5.8533, 6.0616, 6.2697, 6.4776, 6.6854, 6.8931};

  const uint64_t max_allowed_nbits = 48E9;
  m_n_max = n;
  const int theta = (1 << m_n_counting_bits) - 1;
  double kappa = 0;
  if (theta >= 31) {
    kappa = 0.2037 * theta + 0.9176;
  } else {
    kappa = kappa_stars[theta - 1];
  }

  m_width = -n * log(p) / pow(log(2), 2);
  m_num_hashes = ceil((double(m_width) / n) * log(2));

  if (m_width * m_n_counting_bits > max_allowed_nbits) {
    std::cout << "WARNING : too many expected items and not enough memory \n";
    m_width = max_allowed_nbits / m_n_counting_bits;
    std::cout << "\t setting n from " << n << std::setprecision(2) << " to "
              << double(m_width) << "\n";
    n = m_width;
    // filter will be saturated beyond m_n_max inserts
    m_n_max = (m_width / m_num_hashes) * kappa;
    // use m/n =1
    m_num_hashes = ceil(kappa);
  }

  std::cout << "Bloom Filter  Parameter Summary :\n";
  std::cout << "\t ...  Allocated memory : "
            << m_width * m_n_counting_bits * 1.25E-10 << " [Gb]\n";
  std::cout << "\t ...  #of counting bits = " << double(m_n_counting_bits)
            << "\n";
  std::cout << "\t ...  m = " << double(m_width) << "\n";

  std::cout << "\t ...  k = " << m_num_hashes << "\n";
  std::cout << "\t ...  m/n = " << (double(m_width) / n) << "\n";
  std::cout << " \t ... n_saturated " << double(m_n_max) << "\n\n";

  m_bits = std::make_unique<Bits>(m_width * m_n_counting_bits);
  return;
}

template <typename T>
std::vector<uint64_t> CountingBloomFilter<T>::BloomHash(const T& item) const {
  std::vector<uint64_t> result(m_num_hashes);
  std::iota(result.begin(), result.end(), 0);
  std::for_each(result.begin(), result.end(),
                [&item, this](uint64_t& n) { n = m_hasher(item, n); });
  return result;
}

template <typename T>
uint64_t CountingBloomFilter<T>::Counter() const {
  return m_insert_counter;
}

template <typename T>
std::vector<uint64_t> CountingBloomFilter<T>::CounterBitIndices(
    uint64_t hashvalue) const {
  uint64_t counter_index = hashvalue % m_width;
  std::vector<uint64_t> indices(m_n_counting_bits);
  std::iota(indices.begin(), indices.end(), 0);
  std::for_each(
      indices.begin(), indices.end(),
      [&counter_index, this](uint64_t& n) { n = counter_index + n * m_width; });
  return indices;
}

template <typename T>
void CountingBloomFilter<T>::IncrementCounter(
    const std::vector<uint64_t>& counter_bit_indices) {
  // k is the position of the rightmost zero bit.Remember the bits are
  // traversed in reverse order.
  auto kpos =
      std::find_if(counter_bit_indices.rbegin(), counter_bit_indices.rend(),
                   [this](const auto& i) { return (*m_bits)[i] == 0; });
  if (kpos == counter_bit_indices.rend()) return;  // counter is saturated
  // set kth bit to one
  (*m_bits)[*kpos] = 1;
  // then flip any  bits beyond "k"
  std::for_each(counter_bit_indices.rbegin(), kpos,
                [this](const uint64_t& j) { (*m_bits)[j] = !(*m_bits)[j]; });
  return;
}

template <typename T>
void CountingBloomFilter<T>::Insert(const T& item) {
  for (const uint64_t& hash_value : BloomHash(item)) {
    IncrementCounter(CounterBitIndices(hash_value));
  }
  m_insert_counter++;
}

template <typename T>
uint64_t CountingBloomFilter<T>::Count(const T& item) const {
  int min_count = (1 << m_n_counting_bits) - 1;  // full counter value
  for (const uint64_t& hash_value : BloomHash(item)) {
    std::vector<uint64_t> indices = CounterBitIndices(hash_value);
    int curr_count = 0;
    int pow = 0;
    std::for_each(indices.rbegin(), indices.rend(),
                  [&curr_count, &pow, this](const uint64_t& i) {
                    curr_count += (*m_bits)[i] * (1 << pow);
                    pow++;
                  });
    if (curr_count < min_count) {
      min_count = curr_count;
    }
  }
  return min_count;
}

#endif