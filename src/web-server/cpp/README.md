# C++ Web Server

A minimal web server implementation in C++ that provides a simple message submission interface.

## Features

- Home page with welcome message
- Message submission form with email and message fields
- Result page displaying submitted data
- CSS styling for all pages
- Multi-threaded request handling

## Build and Run

```bash
make
make run
```

Or manually:
```bash
g++ -std=c++11 -pthread -Wall -o webserver main.cpp
./webserver
```

The server will start on http://localhost:8080

## Clean

```bash
make clean
```
