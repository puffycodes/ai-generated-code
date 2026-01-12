cmake_minimum_required(VERSION 3.10)
project(WebServer)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CROW REQUIRED crow)

add_executable(webserver main.cpp)
target_link_libraries(webserver ${CROW_LIBRARIES})
target_include_directories(webserver PRIVATE ${CROW_INCLUDE_DIRS})
target_compile_options(webserver PRIVATE ${CROW_CFLAGS_OTHER})
