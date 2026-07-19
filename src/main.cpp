#include "includes/zelibot.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

int main() {
  try {
    std::string token(getenv("TOKEN"));
    printf("[ZeliBOT] Token: %s\n", token.c_str());

    const char *env = std::getenv("ALLOWED_USER");

    if (!env) {
      throw std::runtime_error("ALLOWED_USER not defined");
    }

    uint64_t chat_id = static_cast<uint64_t>(std::stoull(env));
    ZeliBot zelibot(token, chat_id);
    zelibot.run();
  } catch (std::exception &e) {
    std::cout << "[ERROR]" << e.what() << std::endl;
  }
  return 0;
}
