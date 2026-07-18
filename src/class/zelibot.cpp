#include "../includes/zelibot.hpp"
#include "../includes/defs.hpp"
#include "tgbot/tgbot.h"
#include <cstdint>
#include <exception>
#include <string>

ZeliBot::ZeliBot(const std::string &token, const uint64_t allowed_user)
    : bot(token), long_poll(bot), db_manager(DB_NAME),
      allowed_user(allowed_user) {
  initCommands();
};

void ZeliBot::initCommands() {

  bot.getEvents().onCommand("test", [this](TgBot::Message::Ptr message) {
    if (!is_allowed_user(message->chat->id)) {
      bot.getApi().sendMessage(message->chat->id,
                               "[ERROR] You are not allowed for use this bot.");
      return;
    }
    bot.getApi().sendMessage(message->chat->id, "Enter text");
    test_text_state = true;
  });

  bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
    if (!is_allowed_user(message->chat->id)) {
      bot.getApi().sendMessage(message->chat->id,
                               "[ERROR] You are not allowed for use this bot.");
      return;
    }
    if (test_text_state) {
      bot.getApi().sendMessage(message->chat->id, message->text);
      test_text_state = false;
      return;
    }

    for (const auto &command : bot_commands) {
      std::stringstream ss(message->text);
      std::string input_command;
      ss >> input_command;
      if ("/" + command == input_command) {
        std::string arg;
        ss >> arg;
        auto it = command_events_handlers.find(arg);
        if (it != command_events_handlers.end()) {
          it->second();
          return;
        }
      }
    }

    bot.getApi().sendMessage(message->chat->id, "[ERROR] unknown command");
  });
}

void ZeliBot::list_events() {

  auto events = db_manager.get_events();

  for (auto event : events) {
    bot.getApi().sendMessage(allowed_user, "[" + event.id + "] " + event.value +
                                               "Fecha: " + event.date);
  }
}

void ZeliBot::run() {
  try {
    printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
    bot.getApi().deleteWebhook();

    while (true) {
      long_poll.start();
    }
  } catch (std::exception &e) {
    printf("error: %s\n", e.what());
  }
}

bool ZeliBot::is_allowed_user(const uint64_t chat_id) const {
  return chat_id == allowed_user;
}
