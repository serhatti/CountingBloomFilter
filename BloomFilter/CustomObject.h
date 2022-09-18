/*
    author : Serhat Istin
           : istins@gmail.com
*/
#ifndef CUSTOM_OBJECT_H
#define CUSTOM_OBJECT_H

#include <string>

struct Custom {
  Custom(std::string, int, int);
  std::string m_feature1;
  int m_feature2;
  int m_feature3;
};

Custom::Custom(std::string f1, int f2, int f3)
    : m_feature1(f1), m_feature2(f2), m_feature3(f3) {}

#endif