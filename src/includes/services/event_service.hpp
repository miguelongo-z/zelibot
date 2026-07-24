#ifndef SRC_INCLUDES_SERVICES_EVENT_SERVICE_HPP_
#define SRC_INCLUDES_SERVICES_EVENT_SERVICE_HPP_

#include "database/db_manager.hpp"
#include "imessage_sender.hpp"
#include "types.hpp"
#include <string>
#include <vector>
class EventService {
private:
  IMessageSender &client;
  DBManager &db;

public:
  explicit EventService(IMessageSender &_client, DBManager &db_manager);

  void add(const std::vector<std::string> &args);

  void remove(const std::vector<std::string> &args);

  void list(const std::vector<std::string> &args);

  std::vector<Event> pending();
  ~EventService() = default;
};
#endif //  SRC_INCLUDES_SERVICES_EVENT_SERVICE_HPP_
