# webserv

> A non-blocking HTTP/1.1 web server built in C++98, inspired by NGINX.

`webserv` is a 42 Common Core project that involves building a fully functional HTTP web server from scratch in C++, without using any external web frameworks. The server handles concurrent connections through a non-blocking I/O model using `select`/`poll`/`epoll`, parses HTTP/1.1 requests, and serves static content — all while respecting the RFC standards.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Requirements](#requirements)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
- [Supported HTTP Methods](#supported-http-methods)
- [References](#references)

---

## Overview

The server is configured via a custom configuration file inspired by NGINX syntax. It listens on one or more ports, routes requests to appropriate locations, and serves files from a specified root directory. CGI execution, file uploads, and custom error pages are also supported.

The project was developed as a group effort and is a major milestone in the 42 Common Core, covering advanced topics in networking, I/O multiplexing, and HTTP protocol implementation.

---

## Features

- Non-blocking I/O with `select` / `poll` / `epoll` (single-process, single-thread)
- Configurable virtual hosts and server blocks
- HTTP/1.1 request parsing (request line, headers, body)
- Static file serving with MIME type detection
- Directory listing (autoindex)
- Custom error pages (400, 403, 404, 405, 500, etc.)
- File upload support via POST
- CGI execution (Python, Shell scripts)
- HTTP methods: GET, POST, DELETE
- Keep-alive connections
- Chunked transfer encoding

---

## Project Structure

```
webserv/
├── main.cpp            # Entry point — config parsing and server startup
├── src/                # Server core: socket management, request/response logic
├── include/            # Header files
├── config/             # Example configuration files
├── images/             # Static assets for testing
├── Makefile
├── err.log             # Error log output
└── README.md
```

---

## Requirements

- GCC or Clang with C++98 support
- GNU Make
- Unix-based OS (Linux or macOS)
- Python 3 (optional — for CGI scripts)

---

## Installation

Clone the repository and build:

```bash
git clone https://github.com/NicoloRomito/webserv.git
cd webserv
make
```

Clean up:

```bash
make clean    # removes object files
make fclean   # removes binary and object files
make re       # full rebuild
```

---

## Configuration

The server is configured via a `.conf` file. Example configuration:

```nginx
server {
    listen          8080;
    server_name     localhost;
    root            ./www;
    index           index.html;
    client_max_body_size 10M;

    error_page 404  /errors/404.html;

    location / {
        methods     GET POST;
        autoindex   on;
    }

    location /upload {
        methods     POST;
        upload_dir  ./www/uploads;
    }

    location /cgi-bin {
        methods     GET POST;
        cgi_ext     .py /usr/bin/python3;
    }
}
```

Multiple server blocks can be defined to simulate virtual hosting.

---

## Usage

Start the server with a configuration file:

```bash
./webserv config/default.conf
```

If no config file is provided, the server uses a built-in default configuration.

**Test with curl:**

```bash
# GET request
curl http://localhost:8080/

# POST with a file upload
curl -X POST -F "file=@test_upload.txt" http://localhost:8080/upload

# DELETE request
curl -X DELETE http://localhost:8080/upload/test_upload.txt
```

**Test with a browser:**

Open `http://localhost:8080` in any browser to access the served content.

---

## Supported HTTP Methods

| Method | Description |
|---|---|
| `GET` | Retrieve a resource or serve a file |
| `POST` | Upload data or trigger CGI execution |
| `DELETE` | Delete a specified resource |

---

## References

- [RFC 9110 — HTTP Semantics](https://datatracker.ietf.org/doc/html/rfc9110)
- [RFC 9112 — HTTP/1.1 Message Syntax](https://datatracker.ietf.org/doc/html/rfc9112)
- [NGINX — Serving Static Content](https://docs.nginx.com/nginx/admin-guide/web-server/serving-static-content/)
- [Building a Non-Blocking Web Server in C++98 (Medium)](https://m4nnb3ll.medium.com/webserv-building-a-non-blocking-web-server-in-c-98-a-42-project-04c7365e4ec7)

---

*Project developed at 42 Firenze as part of the Common Core curriculum.*
