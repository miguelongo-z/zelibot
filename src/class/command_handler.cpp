#include "command_handler.hpp"
CommandHandler::CommandHandler(IMessageSender &_client, DBManager &db_manager)
    : client(_client), event_service(_client, db_manager) {}

void CommandHandler::handleEvent(const std::string &action,
                                 std::vector<std::string> &args) {

  auto it = command_events_handlers.find(action);

  if (it == command_events_handlers.end()) {
    client.send_message("Mal uso de /event :(");
    return;
  }
  it->second(args);
}

void CommandHandler::handle(const std::string &msg_text) {

  std::stringstream ss(msg_text);
  std::string input_command;
  ss >> input_command;

  auto it = command_handler_map.find(input_command);

  if (it != command_handler_map.end()) {
    client.send_message("[ERROR] Comando inválido");
    return;
  }
  std::string arg;
  std::string action;
  ss >> action;
  std::vector<std::string> args;
  while (ss >> arg) {
    args.push_back(arg);
  }

  it->second(action, args);
}
