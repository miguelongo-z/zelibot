#ifndef ZELIBOT_HPP
#define ZELIBOT_HPP
#include "db_manager.hpp"
#include <cstdint>
#include <functional>
#include <tgbot/tgbot.h>
#include <unordered_map>
#include <vector>

class ZeliBot {
private:
  TgBot::Bot bot;
  TgBot::TgLongPoll long_poll;
  DBManager db_manager;
  const std::vector<std::string> bot_commands = {"start", "test", "events"};
  bool test_text_state = false;
  const uint64_t allowed_user;
  bool is_allowed_user(const uint64_t chat_id) const;

  std::unordered_map<std::string,
                     std::function<void(std::vector<std::string> &args)>>
      command_events_handlers = {
          {"list",
           [this](std::vector<std::string> &args) { list_events(args); }},
          {"add", [this](std::vector<std::string> &args) { add_event(args); }}};

  void list_events(std::vector<std::string> &args);
  void add_event(std::vector<std::string> &args);
  void initCommands();

public:
  ZeliBot(const std::string &token, const uint64_t chat_id);

  void run();

  ~ZeliBot() = default;
};

#endif // !ZELIBOT_HPP
