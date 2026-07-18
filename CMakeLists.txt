cmake_minimum_required(VERSION 3.10.2)

project(zelibot)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall)

include(FetchContent)

FetchContent_Declare(
    tgbot-cpp
    GIT_REPOSITORY https://github.com/reo7sp/tgbot-cpp.git
    GIT_TAG v1.10
)

FetchContent_MakeAvailable(tgbot-cpp)

FetchContent_Declare(
  sqlitecpp
    GIT_REPOSITORY https://github.com/srombauts/sqlitecpp.git
    GIT_TAG 3.3.3
)
FetchContent_MakeAvailable(sqlitecpp)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL REQUIRED)


add_executable(zelibot
    src/main.cpp
    src/class/zelibot.cpp
    src/class/db_manager.cpp
)


target_link_libraries(zelibot
    TgBot
    SQLiteCpp
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
    Boost::system
    CURL::libcurl
)
