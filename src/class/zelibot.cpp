#include "../includes/zelibot.hpp"
#include "tgbot/tgbot.h"
#include <exception>
#include <string>
ZeliBot::ZeliBot(const std::string &token) : bot(token), long_poll(bot) {
  initCommands();
};

void ZeliBot::initCommands() {

  bot.getEvents().onCommand("start", [this](TgBot::Message::Ptr message) {
    bot.getApi().sendMessage(message->chat->id, "Hi!");
  });

  bot.getEvents().onCommand("test", [this](TgBot::Message::Ptr message) {
    bot.getApi().sendMessage(message->chat->id, "Enter text");
    test_text_state = true;
  });

  bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
    if (test_text_state) {
      bot.getApi().sendMessage(message->chat->id, message->text);
      test_text_state = false;
      return;
    }

    for (const auto &command : bot_commands) {
      if ("/" + command == message->text) {
        return;
      }
    }

    bot.getApi().sendMessage(message->chat->id, "[ERROR] unknown command");
  });
}

void ZeliBot::run() {
  try {
    printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
    bot.getApi().deleteWebhook();

    while (true) {
      printf("Long poll started\n");
      long_poll.start();
    }
  } catch (std::exception &e) {
    printf("error: %s\n", e.what());
  }
}
