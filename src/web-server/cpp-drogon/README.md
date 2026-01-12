# C++ Drogon Web Server

A simple web server built with C++ and the Drogon framework.

## Features

- Home page with welcome message
- Message submission form
- Form processing and display
- CSS styling
- Navigation between pages

## Build Requirements

- C++17 compiler
- CMake 3.5+
- Drogon framework

## Installation

1. Install Drogon framework:
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libdrogon-dev
   
   # Or build from source
   git clone https://github.com/drogonframework/drogon
   cd drogon
   git submodule update --init
   mkdir build && cd build
   cmake ..
   make && sudo make install
   ```

2. Build the project:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

3. Run the server:
   ```bash
   ./WebServer
   ```

4. Open browser to `http://localhost:8080`

## Usage

- Visit the home page at `/`
- Click "Submit Message" to go to the submission form
- Fill out email and message, then submit
- View the confirmation page with your submitted data
- Navigate back using the provided links
