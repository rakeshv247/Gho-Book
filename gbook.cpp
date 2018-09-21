#include "gbook.h"
#include "logger.h"
#include "config.h"
#include "database.h"

#include <string.h>

GhoBook::GhoBook()
{
}

GhoBook::~GhoBook()
{
}

bool GhoBook::Init(int argc, char **argv)
{
  static char config_file[1025] = DEFAULT_CONFIG_FILE;

  if (argv) {
    int i = 0;
    for (i = 0; i < argc; i++) {
      if (!strcmp(argv[i], "-f")) {
        if (i < argc - 1) {
          strcpy(config_file, argv[i + 1]);
        } else {
          fprintf(stderr, "Wrong usage of -f option\n");
        }
      }
    }
  }

  if (!Config::Init(config_file) ||
      !Logger::Init() ||
      !Database::Init()) {
    return false;
  }

  return true;
}

void GhoBook::Destroy()
{
  Database::Destroy();
  Logger::Destroy();
  Config::Destroy();
}

int main(int argc, char **argv)
{
  GhoBook gb;

  gb.Init(argc, argv);
  gb.Destroy();

  return 0;
}
