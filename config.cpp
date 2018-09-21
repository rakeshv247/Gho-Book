#include "config.h"

#include <string.h>
#include <stdlib.h>

struct myoption {
  const char *name;     // name of long option
  int val;              // if flag is NULL, return enum value value
};

enum config_param_type {
  LOG_FILE_SIZE = 256,
  LOG_FILE_PATH,
  DB_FILE_PATH,
  LOG_TO_CONSOLE
};

static const myoption long_options[] = {
  { "log_file_size", LOG_FILE_SIZE },
  { "log_file_path", LOG_FILE_PATH },
  { "db_file_path",  DB_FILE_PATH },
  { "log_to_console", LOG_TO_CONSOLE }
};

Config* Config::self = NULL;
Config_t config;

Config::Config()
{
}

Config::~Config()
{
}

bool Config::Init(const std::string& config_file)
{
  if (!self) {
    self = new Config();
  }

  if (!self) {
    fprintf(stderr, "Error: Initializing Config\n");
    return false;
  }

  return self->ReadConfigFile(config_file);
}

void Config::Destroy()
{
  if (self) {
    delete self;
  }
}

char* Config::skip_blanks(char* s)
{
  while(*s==' ' || *s=='\t' || *s=='\n')
    ++s;

  return s;
}

bool Config::parse_arg_string(char *sarg, int *c, char **value)
{
  int i = 0;
  char *name = sarg;
  while(*sarg) {
    if((*sarg==' ') || (*sarg=='=') || (*sarg=='\t')) {
      *sarg=0;
      do {
        ++sarg;
      } while((*sarg==' ') || (*sarg=='=') || (*sarg=='\t'));
      *value = sarg;
      break;
    }
    ++sarg;
    *value=sarg;
  }

  if(value && *value && **value=='\"') {
    *value += 1;
    size_t len = strlen(*value);
    while(len>0 && (
          ((*value)[len-1]=='\n') ||
          ((*value)[len-1]=='\r') ||
          ((*value)[len-1]==' ') ||
          ((*value)[len-1]=='\t')
          ) ) {
      (*value)[--len]=0;
    }
    if(len>0 && (*value)[len-1]=='\"') {
      (*value)[--len]=0;
    }
  }

  while(long_options[i].name) {
    if(strcmp(long_options[i].name,name)) {
      ++i;
      continue;
    }
    *c=long_options[i].val;
    return true;
  }

  return false;
}

bool Config::ReadConfigFile(const std::string& config_file)
{
  FILE *f = NULL;
  f = fopen(config_file.c_str(), "r");
  if (f) {

    char sbuf[1025] = {0};
    char sarg[1035] = {0};

    for (;;) {
      char *s = fgets(sbuf, sizeof(sbuf) - 1, f);
      if (!s)
        break;
      s = skip_blanks(s);
      if (s[0] == '#')
        continue;
      if (!s[0])
        continue;
      size_t slen = strlen(s);
      while (slen && ((s[slen - 1] == 10) || (s[slen - 1] == 13))) //new line carriage return
        s[--slen] = 0;
      if (slen) {
        int c = 0;
        char *value = NULL;
        strcpy(sarg, s);
        if (!parse_arg_string(sarg, &c, &value)) {
          fprintf(stderr, "Bad configuration format: %s\n",sarg);
          return false;
        } else if(c == LOG_FILE_PATH) {
          config.log_file_path = value;
        } else if(c == LOG_FILE_SIZE) {
          uint32_t size = atoi(value);
          if (size > 0) {
            config.log_file_size = size;
          }
        } else if (c == DB_FILE_PATH) {
          config.db_file_path = value;
        } else if (c == LOG_TO_CONSOLE) {
          uint32_t val = atoi(value);
          if (!val) {
            config.log_to_console = false;
          }
        }
      }
    }
    fclose(f);
  } else {
    fprintf(stderr, "WARNING: Cannot find config file: %s. Default and command-line settings will be used.\n", config_file.c_str());
    return false;
  }

  return true;
}

