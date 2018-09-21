#ifndef G_BOOK_H
#define G_BOOK_H

#include <iostream>
#include "sqlite3.h"

class GhoBook
{
public:
  GhoBook();
  ~GhoBook();
  bool Init(int argc, char **argv);
  void Destroy();
};

#endif
