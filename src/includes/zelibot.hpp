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

  std::unordered_map<std::string, std::function<void()>>
      command_events_handlers = {{"list", [this]() { list_events(); }}};

  void list_events();
  void initCommands();

public:
  ZeliBot(const std::string &token, const uint64_t allowed_user);

  void run();

  ~ZeliBot() = default;
};

#endif // !ZELIBOT_HPP
