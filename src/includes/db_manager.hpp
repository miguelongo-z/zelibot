#ifndef DB_MANAGER
#define DB_MANAGER
#include "types.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include <vector>
class DBManager {
private:
  SQLite::Database db;
  const std::string &name;
  void init();

public:
  DBManager(const std::string &db_name);
  std::vector<Event> get_events();
  bool has_pending_events() const;
  ~DBManager() = default;
};

#endif // !DB_MANAGER
