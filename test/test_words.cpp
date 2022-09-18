/*
    author : Serhat Istin
           : istins@gmail.com
*/
#include <bits/stdc++.h>

#include <fstream>
#include <unordered_set>

#include "CountingBloomFilter.h"

int main() {
  const char *dict = std::getenv("BF_DICT");
  auto words_file = std::ifstream(dict);
  if (!words_file) {
    throw std::runtime_error("Dictionary file not found");
  }

  auto beginclk = std::chrono::high_resolution_clock::now();

  const int n_count_bits = 5;
  double n_expected = 4E5;
  CountingBloomFilter<std::string> bf(n_expected, n_count_bits);

  auto PrintQuery = [&bf](const std::string &s) {
    int count = bf.Count(s);
    if (count > 0) {
      std::cout << "\t--> \'" << s << "\': exists <= " << count << " times"
                << std::endl;
    } else {
      std::cout << "\t!->\'" << s << "\': doesn't exist" << std::endl;
    }
  };

  std::string buff;
  while (getline(words_file, buff)) {
    if (buff[0] == '#' || buff[0] == '/') continue;
    bf.Insert(buff);
  }

  /*
  std::unordered_set<std::string> check_list = {
      "Never", "gonna",  "give",  "you",   "up",      "Never", "gonna",
      "let",   "you",    "down",  "Never", "gonna",   "run",   "around",
      "and",   "desert", "you",   "Never", "gonna",   "make",  "you",
      "cry",   "Never",  "gonna", "say",   "goodbye", "Never", "gonna",
      "tell",  "a",      "lie",   "and",   "hurt",    "you"};
*/

  std::unordered_set<std::string> check_list = {
      "a",      "aa",   "aaa",       "aaaa",      "carrier", "scarab", "pursue",
      "Never",  "say",  "down",      "portal",    "portals", "portas", "verb",
      "matter", "done", "beginning", "Beginning", "located", "recall"};
  for (const auto &e : check_list) {
    PrintQuery(e);
  }
  std::cout << bf.Counter() << " items was inserted " << std::endl;

  auto endclk = std::chrono::high_resolution_clock::now();
  std::cout << "execution took "
            << std::chrono::duration_cast<std::chrono::microseconds>(endclk -
                                                                     beginclk)
                   .count()
            << " \xC2\xB5s" << std::endl;

  return 0;
}