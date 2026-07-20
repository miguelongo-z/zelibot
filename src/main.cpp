#include "includes/zelibot.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>

int main() {
  try {
    // uint64_t chat_id = static_cast<uint64_t>(std::stoull(env));
    ZeliBot zelibot;
    zelibot.run();
  } catch (std::exception &e) {
    std::cout << "[ERROR]" << e.what() << std::endl;
  }
  return 0;
}
