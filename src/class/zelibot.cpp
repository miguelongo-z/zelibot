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
  bot.getApi().deleteWebhook();
  std::cout << "[ZeliBOT] Bot username: " << bot.getApi().getMe()->username
            << std::endl;
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
      ss >> input_command;
      if ("/" + command == input_command) {

        std::string arg;
        ss >> arg;
        auto it = command_events_handlers.find(arg);
        if (it != command_events_handlers.end()) {
          std::vector<std::string> args;
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

  for (const auto &event : events) {
    bot.getApi().sendMessage(allowed_user, "[" + event.id + "] " + event.value +
                                               " | " + "Fecha: " + event.date);
  }
}

void ZeliBot::add_event(std::vector<std::string> &args) {

  if (args.empty() || args.size() < 2) {
    bot.getApi().sendMessage(allowed_user,
                             "Cantidad de argumentos incorrecto. uso /evento "
                             "add <fecha> <contenido>");

    return;
  }
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
    while (keep_running) {
      long_poll.start();
    }
  } catch (std::exception &e) {
    std::cout << "[ERROR] " << e.what() << std::endl;
    keep_running = false;
  }
}

bool ZeliBot::is_allowed_user(const uint64_t chat_id) const {
  return chat_id == allowed_user;
}
