# C++ Crow Web Server

A minimal web server implementation using the Crow C++ framework.

## Features

- Home page with welcome message
- Message submission form with email and message fields
- Form processing and response display
- CSS styling
- Static file serving

## Prerequisites

- C++ compiler with C++14 support
- CMake 3.10+
- Crow framework

## Installation

### Install Crow Framework

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install libcrow-dev
```

**macOS (with Homebrew):**
```bash
brew install crow
```

**From source:**
```bash
git clone https://github.com/CrowCpp/Crow.git
cd Crow
mkdir build
cd build
cmake .. -DCROW_BUILD_EXAMPLES=OFF
make -j4
sudo make install
```

## Build and Run

```bash
mkdir build
cd build
cmake ..
make
./webserver
```

The server will start on http://localhost:8080

## Usage

1. Navigate to http://localhost:8080 for the home page
2. Click "Submit Message" to access the submission form
3. Fill in email and message, then click Submit
4. View the confirmation page with your submitted data
