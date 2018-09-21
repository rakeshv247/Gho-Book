#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <stdio.h>

#define LOG(MODE, FMT, ...) Logger::LogWrite(MODE, FMT, ##__VA_ARGS__)

typedef enum LogMode_
{
  LOG_INFO = 0,
  LOG_ERROR,
  LOG_DEBUG
} LogMode;

class Logger
{
private:
  Logger();
  ~Logger();
  bool Open();
public:
  static bool Init();
  static void LogWrite(LogMode mode, const char *fmt, ...);
  static void Destroy();
private:
  static Logger *self;
  FILE* logfile;
};

#endif
