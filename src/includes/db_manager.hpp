
#ifndef SRC_INCLUDES_DB_MANAGER_HPP_
#define SRC_INCLUDES_DB_MANAGER_HPP_

#include "types.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include <mutex>
#include <string>
#include <vector>
class DBManager {
private:
  SQLite::Database db;
  const std::string &name;

  mutable std::mutex mutex;

  void init();

public:
  explicit DBManager(const std::string &db_name);
  std::vector<Event> get_events();
  void create_event(const std::string &value, const std::string &date);
  bool delete_event(const int id);
  bool has_events() const;
  std::vector<Event> pop_pending_events();
  ~DBManager() = default;
};
#endif // SRC_INCLUDES_DB_MANAGER_HPP_
