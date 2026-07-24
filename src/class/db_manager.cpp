#include "db_manager.hpp"
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

bool DBManager::has_events() const {
  std::lock_guard lock(mutex);
  SQLite::Statement query(db, "SELECT EXISTS(SELECT 1 FROM events);");

  query.executeStep();

  return query.getColumn(0).getInt() == 1;
}

std::vector<Event> DBManager::get_events() {
  std::lock_guard lock(mutex);
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
  std::lock_guard lock(mutex);
  SQLite::Statement query(db, "INSERT INTO events VALUES(NULL, ?, ?)");

  query.bind(1, value);
  query.bind(2, date);

  query.exec();

  std::cout << "[DBManager] Event saved" << std::endl;
}

bool DBManager::delete_event(const int id) {
  std::lock_guard lock(mutex);
  int row = 0;
  SQLite::Statement query(db, "DELETE FROM events WHERE id = ?");
  query.bind(1, id);
  row = query.exec();

  if (row > 0)
    std::cout << "[DBManager] Event deleted" << std::endl;
  return row > 0;
}

std::vector<Event> DBManager::get_pending_events() {
  std::lock_guard lock(mutex);

  std::vector<Event> events;
  SQLite::Statement query(db, "SELECT * FROM events WHERE datetime(event_date "
                              "|| ':00') <= datetime('now','localtime');");
  while (query.executeStep()) {
    Event event;

    event.id = query.getColumn(0).getString();
    event.value = query.getColumn(1).getString();
    event.date = query.getColumn(2).getString();

    events.push_back(event);
  }
  db.exec("DELETE FROM events WHERE datetime(event_date || ':00') <= "
          "datetime('now','localtime');");

  return events;
}
