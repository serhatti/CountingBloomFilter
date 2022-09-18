/*
    author : Serhat Istin
           : istins@gmail.com
*/
#include <iostream>

#include "CountingBloomFilter.h"
#include "CustomHasher.h"

int main() {
  uint64_t n_expected = 1E4;
  double p_false_positive = 0.001;
  int n_counter_bits = 4;
  CountingBloomFilter<Custom> bf2(n_expected, n_counter_bits);

  auto PrintQuery = [&bf2](const Custom& inp) {
    int count = bf2.Count(inp);
    std::cout << "\t--> item with <feature1=" << inp.m_feature1
              << "> <feature2=" << inp.m_feature2
              << "> <feature3=" << inp.m_feature3 << ">\n";
    if (count > 0) {
      std::cout << "exists " << count << " times" << std::endl;
    } else {
      std::cout << " does not exist" << std::endl;
    }
  };

  bf2.Insert(Custom("proton", 201, 12));
  bf2.Insert(Custom("proton", 201, 12));
  bf2.Insert(Custom("proton", 201, 12));
  bf2.Insert(Custom("proton", 201, 12));

  PrintQuery(Custom("proton", 201, 12));
  PrintQuery(Custom("proton", 202, 12));
  PrintQuery(Custom("lepton", 201, 11));
  return 0;
}