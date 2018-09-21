#include "utils.h"

#include <sys/time.h>
#include <sys/resource.h>

static const unsigned int MAX_DATE_TIME_LEN = 256;

void Utils::GetCurrDateTimeMs(std::string& str)
{
  char c_dt_1[MAX_DATE_TIME_LEN] = {0};
  char c_dt_2[MAX_DATE_TIME_LEN] = {0};

  struct timeval tv_info;
  gettimeofday(&tv_info, NULL);

  struct tm tm_info;
  localtime_r(&(tv_info.tv_sec), &tm_info);

  strftime(c_dt_1, MAX_DATE_TIME_LEN, "%Y-%m-%d-%H:%M:%S", &tm_info);

  unsigned int micro_secs = 0;
  if ( tv_info.tv_usec > 0 ) {
    micro_secs = tv_info.tv_usec;
  }

  snprintf(c_dt_2, MAX_DATE_TIME_LEN, "%s:%06d", c_dt_1, micro_secs);

  str.assign(c_dt_2);
}
