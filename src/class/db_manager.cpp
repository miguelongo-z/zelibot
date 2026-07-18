#include "../includes/db_manager.hpp"
#include <iostream>
#include <ostream>
#include <vector>

DBManager::DBManager(const std::string &name)
    : db(name, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE), name(name) {

  std::cout << "[SQLite] SQLite3 version " << SQLite::VERSION << " ("
            << SQLite::getLibVersion() << ")" << std::endl;
  std::cout << "[SQLite] SQLiteC++ version " << SQLITECPP_VERSION << std::endl;
  init();
}

void DBManager::init() {

  try {

    std::cout << "[SQLite] SQLite database file" << " opened successfully\n";

    db.exec("CREATE TABLE IF NOT EXISTS events ("
            "id INTEGER PRIMARY KEY, "
            "event_name TEXT, event_date TEXT)");

    SQLite::Statement query(db, "SELECT * FROM events");
    while (query.executeStep()) {
      std::cout << "row (" << query.getColumn(0) << ", \"" << query.getColumn(1)
                << ", \"" << query.getColumn(2) << "\")\n";
    }

  } catch (std::exception &e) {
    std::cout << "[SQLite] " << e.what() << std::endl;
    return;
  }
  std::cout << "[SQLite] OK " << std::endl;
};

bool DBManager::has_pending_events() const {
  SQLite::Statement query(db, "SELECT * FROM events");
  return query.executeStep();
}

std::vector<Event> DBManager::get_events() {
  std::vector<Event> events;
  SQLite::Statement query(db, "SELECT * FROM events");
  while (query.executeStep()) {
    Event event;

    event.id = query.getColumn(0).getString();
    event.value = query.getColumn(1).getString();
    event.date = query.getColumn(2).getString();

    events.push_back(event);
  }
  return events;
}

void DBManager::create_event(const std::string &value,
                             const std::string &date) {

  std::cout << "[DBManager] event saved" << std::endl;

  db.exec("INSERT INTO events VALUES (NULL, \"" + value + "\",\" " + date +
          "\" )");
}
