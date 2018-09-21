#include "logger.h"
#include "config.h"
#include "utils.h"

#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define WRITE_INFO_LOG(FMT) fprintf(stderr, "INFO: %s", FMT)
#define WRITE_ERROR_LOG(FMT) fprintf(stderr, "ERROR: %s", FMT)
#define WRITE_DEBUG_LOG(FMT) fprintf(stderr, "DEBUG: %s", FMT)

Logger* Logger::self = NULL;

Logger::Logger() : logfile(NULL)
{
}

Logger::~Logger()
{
}

bool Logger::Init()
{
  if (!self) {
    self = new Logger();
  }

  if (!self) {
    fprintf(stderr, "Error: Initializing Logger\n");
    return false;
  }

  return self->Open();
}

bool Logger::Open()
{
  std::string log_file_name;
  std::string _log_file_path = config.log_file_path;

  Utils::GetCurrDateTimeMs(log_file_name);
  log_file_name += ".log";

  if (mkdir(_log_file_path.c_str(), 0755) == -1) {
    if (errno != EEXIST) {
      fprintf(stderr, "ERROR: Creating/Opening LOG Directory\n");
      return false;
    }
  }

  if (_log_file_path[_log_file_path.length()-1] != '/') {
    _log_file_path += "/";
  }

  _log_file_path += log_file_name;

  logfile = fopen(_log_file_path.c_str(), "w");
  if (!logfile) {
    fprintf(stderr, "Error: Opening Log File\n");
    return false;
  }

  return true;
}

void Logger::LogWrite(LogMode mode, const char *fmt, ...)
{
  if (!self) {
    fprintf(stderr, "Error: Logger Not Inited\n");
    return;
  }

  va_list vl1, vl2;
  va_start(vl1, fmt);
  va_copy(vl2, vl1);

  const int num = vsnprintf(0, 0, fmt, vl1);
  char temp[num+1];
  memset(temp, 0, sizeof(temp));
  vsnprintf(temp, (num+1), fmt, vl2);

  switch(mode) {
    case LOG_INFO:
      {
        WRITE_INFO_LOG(temp);
        break;
      }
    case LOG_ERROR:
      {
        WRITE_ERROR_LOG(temp);
        break;
      }
    case LOG_DEBUG:
      {
		    WRITE_DEBUG_LOG(temp);
        break;
      }
    default:
      {
        WRITE_DEBUG_LOG(temp);
        break;
      }
  }

  va_end(vl1);
  va_end(vl2);
}

void Logger::Destroy()
{
  if (self) {
    delete self;
  }
}
