#include "../includes/zelibot.hpp"
#include "../includes/defs.hpp"
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <numeric>
#include <regex>
#include <string>
#include <thread>
#include <vector>

ZeliBot::ZeliBot(const std::string &token, const uint64_t chat_id)
    : bot(token), notifier_bot(token), long_poll(bot), db_manager(DB_NAME),
      allowed_user(chat_id) {
  initCommands();
  bot.getApi().deleteWebhook();
  std::cout << "[ZeliBOT] Bot username: " << bot.getApi().getMe()->username
            << std::endl;
}

void ZeliBot::initCommands() {

  bot.getEvents().onCommand("test", [this](TgBot::Message::Ptr message) {
    if (!is_allowed_user(static_cast<uint64_t>(message->chat->id))) {
      send_message(message->chat->id,
                   "[ERROR] You are not allowed for use this bot.");
      return;
    }
    send_message("Enter text");
    test_text_state = true;
  });

  bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
    if (!is_allowed_user(static_cast<uint64_t>(message->chat->id))) {
      send_message(message->chat->id,
                   "[ERROR] You are not allowed for use this bot.");
      return;
    }
    if (test_text_state) {
      send_message(message->chat->id, message->text);
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

    send_message(message->chat->id, "[ERROR] Unknown Command");
  });
}

void ZeliBot::list_event(const std::vector<std::string> &) {

  if (!db_manager.has_events()) {
    send_message("No hay eventos programados :(");
    return;
  }
  auto events = db_manager.get_events();

  for (const auto &event : events) {
    send_message("[" + event.id + "] " + event.value + " | " +
                 "Fecha: " + event.date);
  }
}

void ZeliBot::del_event(const std::vector<std::string> &args) {
  if (args.empty()) {
    send_message("Cantidad de argumentos incorrecto. uso /event "
                 "del <eventoID>");
    return;
  }
  std::string number_str = args.front();

  bool is_a_number =
      std::all_of(number_str.begin(), number_str.end(),
                  [](unsigned char c) { return std::isdigit(c); });

  if (!is_a_number) {
    send_message("ID inválido.");
    return;
  }

  bool success = db_manager.delete_event(std::stoi(number_str));

  if (success) {
    send_message("Evento eliminado :)");
  } else {
    send_message("Error al borrar el evento. ID no existe o error interno");
  }
}

bool ZeliBot::check_hour(const std::string &hour) {

  int h, m;
  char sep;

  std::stringstream ss(hour);

  if (!(ss >> h >> sep >> m))
    return false;

  if (sep != ':')
    return false;

  return h >= 0 && h <= 23 && m >= 0 && m <= 59;
}

bool ZeliBot::check_date(const std::string &date) {
  int year, month, day;
  char sep1, sep2;

  std::stringstream ss(date);

  if (!(ss >> year >> sep1 >> month >> sep2 >> day))
    return false;

  if (sep1 != '-' || sep2 != '-')
    return false;

  std::chrono::year_month_day ymd{
      std::chrono::year{year}, std::chrono::month{static_cast<unsigned>(month)},
      std::chrono::day{static_cast<unsigned>(day)}};

  return ymd.ok();
}

bool ZeliBot::is_valid_format_date(const std::string &date,
                                   const std::string &hour) {

  std::regex date_regex(R"(^\d{4}-\d{2}-\d{2}$)");
  std::regex hour_regex(R"(^\d{2}:\d{2}$)");

  return std::regex_match(date, date_regex) &&
         std::regex_match(hour, hour_regex) && check_date(date) &&
         check_hour(hour);
}

void ZeliBot::send_message(const std::string &message) {
  bot.getApi().sendMessage(allowed_user, message);
}

void ZeliBot::send_message(int64_t chat_id, const std::string &message) {

  bot.getApi().sendMessage(chat_id, message);
}

void ZeliBot::add_event(const std::vector<std::string> &args) {

  if (args.size() < 2) {
    send_message("Cantidad de argumentos incorrecto. uso /event add "
                 "<fecha> <hora> <contenido>");
    return;
  }

  std::string date = args[0];

  std::string hour = args[1];

  if (!is_valid_format_date(date, hour)) {
    send_message("No es una fecha válida, revisa que cumpla este formato "
                 "<año-mes-día> <hora:minutos>");
    return;
  }

  std::string content =
      std::accumulate(args.begin() + 2, args.end(), std::string(),
                      [](const std::string &a, const std::string &b) {
                        return a.empty() ? b : a + " " + b;
                      });

  if (content.empty()) {
    send_message("Sin contenido :(");
    return;
  }

  db_manager.create_event(content, date + " " + hour);
  send_message("Evento agregado :)");
}

void ZeliBot::run() {
  notification_thread = std::jthread(&ZeliBot::notification_loop, this);
  try {
    std::cout << "[ZeliBOt] long_poll" << std::endl;
    while (keep_running) {
      long_poll.start();
    }
  } catch (const std::exception &e) {
    std::cerr << "[ZeliBOT] FATAL: " << e.what() << std::endl;
    keep_running = false;
  } catch (...) {
    std::cerr << "[ZeliBOT] FATAL: unknown exception" << std::endl;
    keep_running = false;
  }
}
bool ZeliBot::is_allowed_user(const uint64_t chat_id) const {
  return chat_id == allowed_user;
}

void ZeliBot::notify_pending_events() {
  auto events = db_manager.pop_pending_events();
  if (events.empty())
    return;

  for (const auto &event : events) {
    notifier_bot.getApi().sendMessage(allowed_user,
                                      "[NOTIFICACIÓN] " + event.value);
  }
}

void ZeliBot::notification_loop() {

  while (keep_running) {
    std::cout << "[ZeliBOT] Checking for pending events..." << std::endl;
    notify_pending_events();

    std::this_thread::sleep_for(std::chrono::seconds(15));
  }
}
