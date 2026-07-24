#ifndef SRC_INCLUDES_ZELIBOT_HPP_
#define SRC_INCLUDES_ZELIBOT_HPP_
#include "telegram_client.hpp"
#include <tgbot/tgbot.h>

class ZeliBot {
private:
public:
  ZeliBot();

  TelegramClient client;
  void run();

  ~ZeliBot() = default;
};

#endif // !SRC_INCLUDES_ZELIBOT_HPP_
