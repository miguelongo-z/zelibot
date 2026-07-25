#ifndef SRC_INCLUDES_COMMAND_HANDLER_HPP_
#define SRC_INCLUDES_COMMAND_HANDLER_HPP_

#include "database/db_manager.hpp"
#include "imessage_sender.hpp"
#include "services/event_service.hpp"
#include <functional>
#include <string>
#include <tgbot/tgbot.h>
#include <unordered_map>
#include <vector>
class CommandHandler {

private:
  IMessageSender &client;
  EventService event_service;

  std::unordered_map<std::string,
                     std::function<void(std::vector<std::string> &args)>>
      command_events_handlers = {
          {"list",
           [this](std::vector<std::string> &args) {
             event_service.list(args);
           }},
          {"add",
           [this](std::vector<std::string> &args) { event_service.add(args); }},
          {"del", [this](std::vector<std::string> &args) {
             event_service.remove(args);
           }}};

  std::unordered_map<std::string,
                     std::function<void(const std::string &action,
                                        std::vector<std::string> &args)>>
      command_handler_map = {{"/event", [this](const std::string &action,
                                               std::vector<std::string> &args) {
                                handleEvent(action, args);
                              }}};

  void handleEvent(const std::string &action, std::vector<std::string> &args);

public:
  CommandHandler(IMessageSender &_client, DBManager &db_manager);

  void handle(const std::string &msg_text);

  ~CommandHandler() = default;
};
#endif // SRC_INCLUDES_COMMAND_HANDLER_HPP_
