#include "../includes/db_manager.hpp"
#include <exception>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

DBManager::DBManager(const std::string &db_name)
    : db(db_name, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE), name(db_name) {

  std::cout << "[DBManager] SQLite3 version " << SQLite::VERSION << " ("
            << SQLite::getLibVersion() << ")" << std::endl;
  std::cout << "[DBManager] SQLiteC++ version " << SQLITECPP_VERSION
            << std::endl;
  init();
}

void DBManager::init() {

  try {

    std::cout << "[DBManager] SQLite database file"
              << " opened successfully\n";

    db.exec("CREATE TABLE IF NOT EXISTS events ("
            "id INTEGER PRIMARY KEY, "
            "event_name TEXT, event_date TEXT)");

  } catch (std::exception &e) {
    std::cout << "[DBManager] " << e.what() << std::endl;
    return;
  }
  std::cout << "[DBManager] OK " << std::endl;
}

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
  db.exec("INSERT INTO events VALUES (NULL, \"" + value + "\",\" " + date +
          "\" )");

  std::cout << "[DBManager] Event saved" << std::endl;
}

bool DBManager::delete_event(const int id) {
  int ret = -1;
  try {
    ret = db.exec("DELETE FROM events WHERE id = " + std::to_string(id) + ";");
  } catch (std::exception &e) {
    std::cout << "[DBManager] " << e.what() << std::endl;
    return ret;
  }

  std::cout << "[DBManager] Event deleted" << std::endl;
  return ret;
}
