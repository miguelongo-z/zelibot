
#ifndef SRC_INCLUDES_TELEGRAM_CLIENT_HPP_
#define SRC_INCLUDES_TELEGRAM_CLIENT_HPP_

#include "command_handler.hpp"
#include "config_manager.hpp"
#include "database/db_manager.hpp"
#include "imessage_sender.hpp"
#include <cstdint>
#include <mutex>
#include <string>
#include <tgbot/tgbot.h>
#include <thread>

class TelegramClient : public IMessageSender {
private:
  ConfigManager config_manager;
  TgBot::Bot bot;
  TgBot::TgLongPoll long_poll;
  DBManager db_manager;
  CommandHandler command_handler;
  std::atomic<bool> keep_running{true};
  std::jthread notification_thread;

  mutable std::mutex bot_mtx;
  void init();
  bool is_allowed_user(const int64_t chat_id) const;

  void notify_pending_events();

  void notification_loop();

public:
  TelegramClient();
  void run();
  void send_message(const std::string &message) override;
  void send_message(int64_t chat_id, const std::string &message) override;
  ~TelegramClient() override;
};
#endif // SRC_INCLUDES_TELEGRAM_CLIENT_HPP_
