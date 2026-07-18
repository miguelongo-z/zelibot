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

public:
  ZeliBot(const std::string &token);

  void initCommands();

  void run();

  ~ZeliBot() = default;
};

#endif // !ZELIBOT_HPP
