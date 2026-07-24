cmake_minimum_required(VERSION 3.16)

project(zelibot)

set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ON CACHE BOOL TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    tgbot-cpp
    GIT_REPOSITORY https://github.com/reo7sp/tgbot-cpp.git
    GIT_TAG v1.10
)

FetchContent_MakeAvailable(tgbot-cpp)

# Json parser 

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

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
    src/class/config_manager.cpp  
)

target_compile_options(zelibot PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -Wconversion
    -Wsign-conversion
    -Wshadow
    -Wcast-qual
    -Wold-style-cast
    -Wnon-virtual-dtor
    -Woverloaded-virtual
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2
)

# Para que no marque los warns de la libreria tgbot-cpp
target_include_directories(zelibot
    SYSTEM PRIVATE
    ${tgbot-cpp_SOURCE_DIR}/include
)
target_include_directories(zelibot PRIVATE
    ${CMAKE_SOURCE_DIR}/src/includes
)

# Oculta el warn de bind.h
target_compile_definitions(zelibot PRIVATE BOOST_BIND_GLOBAL_PLACEHOLDERS)

target_link_libraries(zelibot PRIVATE
    TgBot
    SQLiteCpp
    nlohmann_json::nlohmann_json
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
    Boost::system
    CURL::libcurl
    nlohmann_json::nlohmann_json
)
