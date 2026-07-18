#ifndef ZELIBOT_HPP
#define ZELIBOT_HPP
#include <tgbot/tgbot.h>
#include <vector>

class ZeliBot {
private:
  TgBot::Bot bot;
  TgBot::TgLongPoll long_poll;
  const std::vector<std::string> bot_commands = {"start", "test"};
  bool test_text_state = false;
  const int allowed_user;

  bool is_allowed_user(const int chat_id) const;

public:
  ZeliBot(const std::string &token, const int allowed_user);

  void initCommands();

  void run();

  ~ZeliBot() = default;
};

#endif // !ZELIBOT_HPP
