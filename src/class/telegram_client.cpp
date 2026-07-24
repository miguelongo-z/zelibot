#include "telegram_client.hpp"
#include "defs.hpp"
#include <thread>
TelegramClient::TelegramClient()
    : config_manager(CONFIG_PATH), bot(config_manager.get_token()),
      long_poll(bot), command_handler(*this, db_manager) {

  std::cout << "[ZeliBOT] Bot username: " << bot.getApi().getMe()->username
            << std::endl;

  if (!config_manager.chat_id_is_setted()) {
    std::cout << "[ZeliBOT] Chat id is not setted. Send /start to init"
              << std::endl;
  }

  bot.getApi().deleteWebhook();
}
void TelegramClient::init() {

  bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
    if (!config_manager.chat_id_is_setted()) {
      if ("/start" == message->text) {
        config_manager.save_chat_id(message->chat->id);
        send_message(message->chat->id,
                     "El bot ha sido configurado con exito :)");

        std::cout << "[ZeliBOT] Setup success" << std::endl;
      } else {

        send_message(
            message->chat->id,
            "El bot no ha sido configurado, escribe /start para empezar");
      }
      return;
    }

    if (!is_allowed_user(message->chat->id)) {
      send_message(message->chat->id,
                   "[ERROR] You are not allowed for use this bot.");
      return;
    }

    send_message(message->chat->id, "[ERROR] Comando desconocido");
  });
}

bool TelegramClient::is_allowed_user(const int64_t chat_id) const {
  return chat_id == config_manager.get_chat_id();
}

void TelegramClient::send_message(const std::string &message) {
  std::lock_guard<std::mutex> guard(bot_mtx);
  bot.getApi().sendMessage(config_manager.get_chat_id(), message);
}

void TelegramClient::send_message(int64_t chat_id, const std::string &message) {
  std::lock_guard<std::mutex> guard(bot_mtx);
  bot.getApi().sendMessage(chat_id, message);
}

void TelegramClient::run() {

  notification_thread = std::jthread(&TelegramClient::notification_loop, this);
  while (keep_running) {

    try {
      std::cout << "[ZeliBOt] long_poll" << std::endl;
      long_poll.start();
    } catch (const std::exception &e) {
      std::cerr << "[ZeliBOT] FATAL: " << e.what() << std::endl;
      std::string error = e.what();
      if (error.find("Bad Gateway") != std::string::npos) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        continue;
      }
      keep_running = false;
    } catch (...) {
      std::cerr << "[ZeliBOT] FATAL: unknown exception" << std::endl;
      keep_running = false;
    }
  }
}

void TelegramClient::notify_pending_events() {
  auto events = db_manager.get_pending_events();
  if (events.empty())
    return;

  for (const auto &event : events) {
    send_message("[NOTIFICACIÓN] " + event.value);
  }
}

void TelegramClient::notification_loop() {

  while (keep_running) {
    if (!config_manager.chat_id_is_setted()) {
      continue;
    }

    try {

      std::cout << "[ZeliBOT] Checking for pending events..." << std::endl;
      notify_pending_events();

      std::this_thread::sleep_for(std::chrono::seconds(15));
    } catch (const std::exception &e) {
      std::cerr << "[ZeliBOT - Notifier] " << e.what() << std::endl;

      std::this_thread::sleep_for(std::chrono::seconds(5));
    } catch (...) {
      std::cerr << "[ZeliBOT - Notifier] Unknown exception" << std::endl;
      keep_running = false;
    }
  }
}
