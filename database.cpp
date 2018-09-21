#include "database.h"
#include "logger.h"
#include "utils.h"
#include "config.h"

#include <sys/stat.h>
#include <sys/types.h>

Database* Database::self = NULL;

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   
   for(i = 0; i<argc; i++){
      LOG(LOG_INFO, "%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }

   return 0;
}

Database::Database() : db(NULL), cbData(NULL)
{
}

Database::~Database()
{
  if (cbData) {
    delete cbData;
  }
}

bool Database::Init()
{
  if (!self) {
    self = new Database();
    self->cbData = new CallbackData();
  }

  if (!self || !self->cbData) {
    LOG(LOG_ERROR, "Initializing Database\n");
    return false;
  }

  return self->Open();
}

void Database::Destroy()
{
  if (self) {
    self->Close();
    delete self;
  }
}

bool Database::Open()
{
  int rc = SQLITE_ERROR;
	std::string db_file_name;
	std::string _db_file_path;

	Utils::GetCurrDateTimeMs(db_file_name);
	db_file_name += ".db";
	_db_file_path = config.db_file_path;

	if (mkdir(_db_file_path.c_str(), 0755)) {
		LOG(LOG_ERROR, "Opening DB\n");
		return false;
	}

	if (_db_file_path[_db_file_path.length()-1] != '/') {
		_db_file_path += "/";
	}

	_db_file_path += db_file_name;

  rc = sqlite3_open(_db_file_path.c_str(), &db);

  if (rc != SQLITE_OK) {
    LOG(LOG_ERROR, "%s\n", sqlite3_errmsg(db));
    return false;
  }

  return true;
}

void Database::Close()
{
  int rc = SQLITE_ERROR;

  rc = sqlite3_close(db);

  if (rc != SQLITE_OK) {
    LOG(LOG_ERROR, "%s\n", sqlite3_errmsg(db));
  }
}

bool Database::Exec(const char *query, void *data)
{
  int rc = SQLITE_ERROR;
  char *zErrMsg = NULL;

  cbData->dbObject = this;
  cbData->data = data;
  rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);

  if (rc != SQLITE_OK) {
    LOG(LOG_ERROR, "%s\n", sqlite3_errmsg(db));
    return false;
  }

  return true;
}

bool Database::Update(const char *query, void *data)
{
  if (!self) {
    LOG(LOG_ERROR, "Database Object Missing\n");
    return false;
  }

  return self->Exec(query, data);
}
