cmake_minimum_required(VERSION 3.10)
project(SecureWebServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Drogon CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

# Create directories
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/static)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)

# Copy static files
configure_file(${CMAKE_SOURCE_DIR}/static/style.css ${CMAKE_BINARY_DIR}/static/style.css COPYONLY)
