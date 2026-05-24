# History of Algeria Database using Dynamic Data Structures

A hybrid C/Python project that models a historical database for Algeria using dynamic data structures and exposes the functionality through a Flask web bridge.

## Project Overview

This repository combines:

- C implementations of dynamic data structures:
  - Binary Search Tree (BST)
  - Linked List
  - Stack
  - Recursion-based helpers
- A C API wrapper that returns JSON responses
- A Python `Flask` web server that loads the shared C library and exposes API endpoints
- A minimal web interface (`index.html` + `style.css` + `script.js`) to call C-backed functions from the browser
- Sample data files in `data/`

## Key Files

- `main_api.c` — Core C API wrapper and JSON serialization layer
- `src/linked_list.c`, `src/bst.c`, `src/stack.c`, `src/recursion.c` — Data structure implementations
- `include/` — Data structure headers and shared type definitions
- `bridge.py` — Python bridge to the C shared library
- `app.py` — Flask web server for the browser-based front end
- `index.html`, `style.css`, `script.js` — Web UI for selecting functions and viewing results
- `Makefile` — Build targets for the console and GUI C binaries
- `data/history.txt`, `data/events.txt`, `data/persons.txt` — Example history database input files

## Features

- Search and manipulate historical records via BST operations
- Manage historical personalities, dates, and events using linked lists
- Recursion utilities for string matching, replacement, permutations, and palindrome checks
- Stack-based queries and transformations
- Web-based interface for exploring the C API functions interactively

## Prerequisites

- `gcc` or compatible C compiler
- Python 3
- `Flask` Python package

## Setup

1. Install Python dependencies:

```bash
python3 -m pip install flask
```

2. Build the shared C library used by the Python bridge:

```bash
gcc -shared -fPIC -Iinclude main_api.c src/linked_list.c src/stack.c src/bst.c src/recursion.c -o /tmp/history_bridge.so -lm
```

3. Configure the environment variable so the Python bridge can find the library:

```bash
export HISTORY_BRIDGE_SO=/tmp/history_bridge.so
```

4. Start the Flask web server:

```bash
python3 app.py
```

5. Open the web interface in a browser:

```
http://127.0.0.1:5000
```

## Using the Web Interface

The browser UI lets you select one of the grouped functions and send payloads to the backend API.

- `BST` group: tree insertions, traversal, search, update, delete
- `Linked List` group: record retrieval, sorting, palindrome checks, merges, and date operations
- `Recursion` group: occurrence counting, replacement, permutations, subsequences, and range searching
- `Stack` group: stack search, sorting, updates, and utilities

Results are returned as JSON and displayed in the response pane.

## Command-Line Build Targets

The included `Makefile` supports building standalone C programs:

- `make console` — build a console program using `src/main.c`
- `make gui` — build a GUI program using `src/gui.c` and a local `libraylib.a`
- `make clean` — remove generated binaries

## GUI with Raylib

A native graphical interface is available through the `src/gui.c` source file. It uses the Raylib game development library to render a windowed UI for:

- browsing records
- running data structure operations
- displaying output logs

The GUI build depends on Raylib and the local `libraylib.a` library file referenced by the `Makefile`. If you do not already have Raylib installed, build or provide the static library before running `make gui`.

## Notes

- The Python bridge is implemented in `bridge.py` and uses `ctypes` to call exported C functions.
- `python/c_bridge.py` contains a similar interface for loading the shared library and marshaling call arguments.
- The web server exposes the API under `/api/<function_name>`.
- The project is primarily a demonstration of C data structures combined with a Python/JavaScript frontend layer.

## Troubleshooting

- If the shared library cannot be loaded, verify `HISTORY_BRIDGE_SO` points to a valid `.so` file.
- If the web app fails to start, confirm `Flask` is installed.
- If API calls return invalid JSON, check the C wrapper output and ensure the C functions are available in the shared library.
