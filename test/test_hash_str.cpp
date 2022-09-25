/*
    author : Serhat Istin
           : istins@gmail.com
*/
#include <bits/stdc++.h>

#include <fstream>
#include <iostream>
#include <unordered_set>

#include "Hasher.h"

int main() {
  const char *dict = std::getenv("BF_DICT");
  auto words_file = std::ifstream(dict);
  if (!words_file) {
    throw std::runtime_error("Dictionary file not found");
  }
  auto beginclk = std::chrono::high_resolution_clock::now();

  Hasher<std::string> hasher;

  std::unordered_set<uint64_t> hash_values;
  std::unordered_set<std::string> unique_words;

  std::string line;
  while (getline(words_file, line)) {
    if (line[0] == '#' || line[0] == '/')
      continue;
    unique_words.emplace(line);
  }
  for (auto w : unique_words) {
    hash_values.emplace(hasher(w));
  }

  std::cout << "#of inserts=" << unique_words.size()
            << " # of unique hashes=" << hash_values.size() << std::endl;
  auto endclk = std::chrono::high_resolution_clock::now();
  std::cout << "execution took "
            << std::chrono::duration_cast<std::chrono::microseconds>(endclk -
                                                                     beginclk)
                   .count()
            << " \xC2\xB5s" << std::endl;

  return 0;
}