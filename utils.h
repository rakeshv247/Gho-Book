#ifndef UTILS_H
#define UTILS_H

#include <iostream>

class Utils
{
public:
  Utils();
  ~Utils();

  static void GetCurrDateTimeMs(std::string& str);
};

#endif
