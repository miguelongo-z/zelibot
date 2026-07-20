#ifndef SRC_INCLUDES_ZELIBOT_HPP_
#define SRC_INCLUDES_ZELIBOT_HPP_
#include "db_manager.hpp"
#include <cstdint>
#include <functional>
#include <string>
#include <tgbot/tgbot.h>
#include <thread>
#include <unordered_map>
#include <vector>
class ZeliBot {
private:
  TgBot::Bot bot;
  TgBot::Bot notifier_bot;
  TgBot::TgLongPoll long_poll;
  DBManager db_manager;
  bool test_text_state = false;
  std::atomic<bool> keep_running{true};
  const uint64_t allowed_user;

  std::jthread notification_thread;

  const std::vector<std::string> bot_commands = {"start", "test", "event"};

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
  void send_message(const std::string &message);
  void send_message(int64_t chat_id, const std::string &message);
  static bool is_valid_format_date(const std::string &date,
                                   const std::string &hour);
  static bool check_date(const std::string &date);
  static bool check_hour(const std::string &hour);

  static std::string get_db_path();

  void notify_pending_events();

  void notification_loop();

public:
  ZeliBot(const std::string &token, const uint64_t chat_id);

  void run();

  ~ZeliBot() = default;
};

#endif // !SRC_INCLUDES_ZELIBOT_HPP_
