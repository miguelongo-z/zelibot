#ifndef IMESSAGE_SENDER_HPP
#define IMESSAGE_SENDER_HPP

#include <string>

class IMessageSender {

public:
  virtual void send_message(const std::string &message) = 0;

  virtual void send_message(int64_t chat_id, const std::string &message) = 0;

  virtual ~IMessageSender() = default;
};

#endif
