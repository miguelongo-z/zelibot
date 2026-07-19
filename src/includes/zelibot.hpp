#ifndef SRC_INCLUDES_ZELIBOT_HPP_
#define SRC_INCLUDES_ZELIBOT_HPP_
#include "db_manager.hpp"
#include <cstdint>
#include <functional>
#include <string>
#include <tgbot/tgbot.h>
#include <unordered_map>
#include <vector>
class ZeliBot {
private:
  TgBot::Bot bot;
  TgBot::TgLongPoll long_poll;
  DBManager db_manager;
  const std::vector<std::string> bot_commands = {"start", "test", "event"};
  bool test_text_state = false;
  bool keep_running = true;
  const uint64_t allowed_user;

  std::unordered_map<std::string,
                     std::function<void(std::vector<std::string> &args)>>
      command_events_handlers = {
          {"list",
           [this](std::vector<std::string> &args) { list_event(args); }},
          {"add", [this](std::vector<std::string> &args) { add_event(args); }},
          {"del", [this](std::vector<std::string> &args) { del_event(args); }}};

  void del_event(const std::vector<std::string> &args);
  void list_event(const std::vector<std::string> &args);
  void add_event(const std::vector<std::string> &args);
  void initCommands();
  bool is_allowed_user(const uint64_t chat_id) const;

public:
  ZeliBot(const std::string &token, const uint64_t chat_id);

  void run();

  ~ZeliBot() = default;
};

#endif // !SRC_INCLUDES_ZELIBOT_HPP_
