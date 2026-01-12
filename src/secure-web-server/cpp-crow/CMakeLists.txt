cmake_minimum_required(VERSION 3.10)
project(SecureWebServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CROW REQUIRED crow)

add_executable(secure_web_server main.cpp)

target_link_libraries(secure_web_server ${CROW_LIBRARIES})
target_include_directories(secure_web_server PRIVATE ${CROW_INCLUDE_DIRS})
target_compile_options(secure_web_server PRIVATE ${CROW_CFLAGS_OTHER})
