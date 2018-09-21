#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <stdint.h>
#include <stdio.h>

#define DEFAULT_CONFIG_FILE "./gho_book.cfg"

typedef struct Config_s {

  uint32_t log_file_size;
  std::string log_file_path;
  std::string db_file_path;
  bool log_to_console;

  Config_s() {
    log_file_size = 1000000;
    log_file_path = "/tmp";
    db_file_path = "/tmp";
    log_to_console = true;
  }

} Config_t;

class Config
{
private:
  Config();
  ~Config();
  bool ReadConfigFile(const std::string& config_file);
  char* skip_blanks(char* s);
  bool parse_arg_string(char *sarg, int *c, char **value);
public:
  static bool Init(const std::string& config_file);
  static void Destroy();
private:
  static Config *self;
};

extern Config_t config;

#endif
