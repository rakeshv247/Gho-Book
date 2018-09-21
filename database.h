#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <sqlite3.h>

typedef struct CallbackData_s
{
  void *dbObject;
  void *data;
} CallbackData;

class Database
{
private:
  Database();
  ~Database();
  bool Open();
  void Close();
  bool Exec(const char *query, void *data);
public:
  static bool Init();
  static void Destroy();
  static bool Update(const char *query, void *data);
private:
  static Database *self;
  sqlite3 *db;
  CallbackData *cbData;
};

#endif
