
#ifndef SRC_INCLUDES_DB_MANAGER_HPP_
#define SRC_INCLUDES_DB_MANAGER_HPP_

#include "types.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <vector>
class DBManager {
private:
  SQLite::Database db;
  const std::string &name;
  void init();

public:
  explicit DBManager(const std::string &db_name);
  std::vector<Event> get_events();
  void create_event(const std::string &value, const std::string &date);
  bool has_pending_events() const;
  ~DBManager() = default;
};
#endif // SRC_INCLUDES_DB_MANAGER_HPP_
