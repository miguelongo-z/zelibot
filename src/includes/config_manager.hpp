#ifndef SRC_INCLUDES_CONFIG_MANAGER_HPP_
#define SRC_INCLUDES_CONFIG_MANAGER_HPP_

#include <cstdint>
#include <string>
class ConfigManager {

private:
  const std::string path;
  std::string token;
  uint64_t chat_id;
  void init();

  bool config_file_exists() const;

  std::string request_token();

  void load();

public:
  explicit ConfigManager(const std::string &config_pat);
  void save_chat_id(uint64_t chat_id_);
  const std::string &get_token() const;
  uint64_t get_chat_id() const;

  bool chat_id_is_setted() const;
  ~ConfigManager() = default;
};

#endif // !SRC_INCLUDES_CONFIG_MANAGER_HPP_
