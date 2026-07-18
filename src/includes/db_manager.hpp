#ifndef DB_MANAGER
#define DB_MANAGER
#include <SQLiteCpp/SQLiteCpp.h>

class DBManager {
private:
  SQLite::Database db;
  const std::string &name;
  void init();

public:
  DBManager(const std::string &db_name);
  ~DBManager() = default;
};

#endif // !DB_MANAGER
