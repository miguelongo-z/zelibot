#include "../includes/config_manager.hpp"
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
ConfigManager::ConfigManager(const std::string &config_path)
    : path(config_path) {

  if (!config_file_exists())
    init();
  else
    load();
}

std::string ConfigManager::request_token() {
  const char *token_env = std::getenv("TOKEN");
  std::string bot_token;

  if (token_env != nullptr) {
    bot_token = token_env;
    return bot_token;
  }

  std::cout << "Enter BOT Token > ";

  std::cin >> bot_token;

  return bot_token;
}

bool ConfigManager::config_file_exists() const {
  std::ifstream file(path);
  return file.is_open();
}

void ConfigManager::init() {
  std::filesystem::create_directories(
      std::filesystem::path(path).parent_path());

  nlohmann::json data;

  const std::string tk = request_token();
  data["token"] = tk;

  data["chat_id"] = 0;

  token = tk;
  chat_id = 0;

  std::ofstream file(path);

  if (!file.is_open()) {
    throw std::runtime_error("[ConfigManager] Can't initialize config file");
  }

  file << data.dump(4);
}

void ConfigManager::save_chat_id(uint64_t chat_id_) {

  std::ifstream input(path);

  nlohmann::json data;

  input >> data;

  input.close();

  chat_id = chat_id_;

  data["chat_id"] = chat_id_;

  std::ofstream output(path);

  output << data.dump(4);
}

void ConfigManager::load() {

  std::ifstream input(path);

  nlohmann::json data;

  input >> data;

  token = data["token"].get<std::string>();
  chat_id = data["chat_id"].get<uint64_t>();
}

const std::string &ConfigManager::get_token() const { return token; }

bool ConfigManager::chat_id_is_setted() const { return chat_id != 0; }

uint64_t ConfigManager::get_chat_id() const { return chat_id; }
