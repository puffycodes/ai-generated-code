cmake_minimum_required(VERSION 3.10)
project(SecureWebServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(MHD REQUIRED libmicrohttpd)

# Add executable
add_executable(secure_web_server main.cpp)

# Link libraries
target_link_libraries(secure_web_server ${MHD_LIBRARIES})
target_include_directories(secure_web_server PRIVATE ${MHD_INCLUDE_DIRS})
target_compile_options(secure_web_server PRIVATE ${MHD_CFLAGS_OTHER})

# Compiler flags for security
target_compile_options(secure_web_server PRIVATE 
    -Wall -Wextra -Werror 
    -fstack-protector-strong 
    -D_FORTIFY_SOURCE=2
)

# Linker flags for security
target_link_options(secure_web_server PRIVATE 
    -Wl,-z,relro 
    -Wl,-z,now
)
