#include "../includes/zelibot.hpp"
#include "../includes/defs.hpp"
#include "tgbot/tgbot.h"
#include <cstdint>
#include <exception>
#include <numeric>
#include <string>
#include <vector>

ZeliBot::ZeliBot(const std::string &token, const uint64_t chat_id)
    : bot(token), long_poll(bot), db_manager(DB_NAME), allowed_user(chat_id) {
  initCommands();
}

void ZeliBot::initCommands() {

  bot.getEvents().onCommand("test", [this](TgBot::Message::Ptr message) {
    if (!is_allowed_user(static_cast<uint64_t>(message->chat->id))) {
      bot.getApi().sendMessage(message->chat->id,
                               "[ERROR] You are not allowed for use this bot.");
      return;
    }
    bot.getApi().sendMessage(message->chat->id, "Enter text");
    test_text_state = true;
  });

  bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
    if (!is_allowed_user(static_cast<uint64_t>(message->chat->id))) {
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
      std::vector<std::string> args;
      ss >> input_command;
      if ("/" + command == input_command) {
        std::string arg;
        ss >> arg;
        auto it = command_events_handlers.find(arg);
        if (it != command_events_handlers.end()) {
          while (ss >> arg) {
            args.push_back(arg);
          }
          it->second(args);
          return;
        }
      }
    }

    bot.getApi().sendMessage(message->chat->id, "[ERROR] Unknown Command");
  });
}

void ZeliBot::list_events(std::vector<std::string> &) {

  if (!db_manager.has_pending_events()) {
    bot.getApi().sendMessage(allowed_user, "No hay eventos programados :(");
    return;
  }
  auto events = db_manager.get_events();

  for (auto event : events) {
    bot.getApi().sendMessage(allowed_user, "[" + event.id + "] " + event.value +
                                               " " + "Fecha: " + event.date);
  }
}

void ZeliBot::add_event(std::vector<std::string> &args) {

  std::string date = args.front();
  args.erase(args.begin());

  std::string content =
      std::accumulate(args.begin(), args.end(), std::string(),
                      [](const std::string &a, const std::string &b) {
                        return a.empty() ? b : a + " " + b;
                      });
  db_manager.create_event(content, date);
  bot.getApi().sendMessage(allowed_user, "Evento agregado :)");
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
