/*
    author : Serhat Istin
           : istins@gmail.com
*/
#include <argp.h>
#include <bits/stdc++.h>

#include <fstream>
#include <iostream>
#include <unordered_set>

#include "Hasher.h"

int main(int argc, char **argv) {
  auto beginclk = std::chrono::high_resolution_clock::now();

  Hasher<std::string> hasher;

  std::unordered_set<uint64_t> hash_values;
  std::unordered_set<std::string> unique_words;
  auto words_file = std::ifstream("onemillionwords.txt");

  std::string line;
  int insert_count = 0;
  while (getline(words_file, line)) {
    if (line[0] == '#' || line[0] == '/') continue;
    unique_words.insert(line);
  }
  insert_count = unique_words.size();
  for (auto w : unique_words) {
    hash_values.insert(hasher(w));
  }

  std::cout << "#of inserts=" << insert_count
            << " # of unique hashes=" << hash_values.size() << std::endl;
  auto endclk = std::chrono::high_resolution_clock::now();
  std::cout << "execution took "
            << std::chrono::duration_cast<std::chrono::microseconds>(endclk -
                                                                     beginclk)
                   .count()
            << " \xC2\xB5s" << std::endl;

  return 0;
}