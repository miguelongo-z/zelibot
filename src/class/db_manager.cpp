#include "../includes/db_manager.hpp"
#include <iostream>
#include <ostream>
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

    int nb = db.exec(
        "INSERT INTO events VALUES (NULL, \"Hello World!\",\"14-07-2026\" )");
    std::cout << "INSERT INTO events VALUES (NULL, \"Hello World!\", "
                 "\"14-07-2026\")\", returned "
              << nb << std::endl;

    SQLite::Statement query(db, "SELECT * FROM events");
    std::cout << "SELECT * FROM events :\n";
    while (query.executeStep()) {
      std::cout << "row (" << query.getColumn(0) << ", \"" << query.getColumn(1)
                << ", \"" << query.getColumn(2) << "\")\n";
    }

  } catch (std::exception &e) {
    std::cout << "[SQLite] " << e.what() << std::endl;
    return;
  }
  db.exec("DROP TABLE events");
  std::cout << "[SQLite] OK " << std::endl;
};
