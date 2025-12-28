# Minishell Project Context

## Project Overview
Minishell is a custom shell implementation written in C, designed to replicate the functionality of a minimal Bash-like shell. This project is part of the 42 curriculum.

**Key Architectural Decision:**
A significant deviation from standard practices in this specific implementation is the **avoidance of `malloc` (dynamic memory allocation on the heap)**. Instead, the project utilizes **static memory pools** (in `.bss` and `.data` segments) and the stack. This is done to simulate embedded systems constraints and to rigorously manage memory bounds and performance.

## Architecture & Memory Management
The project uses three main static memory pools:
1.  **Input Line Pool:** Buffer for the raw input line.
2.  **AST & Environment Pool:** Stores the Abstract Syntax Tree and command environment data.
3.  **Variables Pool:** Stores local and environment variable linked lists and command structures.

**Key Structures:**
*   `t_env`: Linked list node for environment and local variables.
*   `t_term`: Terminal data for readline functionality.
*   **Global Variable:** `extern sig_atomic_t g_signal` is the *only* allowed global variable, used strictly for signal handling.

## Building and Running
The project uses a `Makefile` for compilation.

*   **Build:** `make`
    *   Compiles `libft` (recursive make).
    *   Compiles source files.
    *   Links with `readline`.
    *   Produces the `minishell` executable.
*   **Run:** `./minishell`
*   **Clean Objects:** `make clean`
*   **Full Clean:** `make fclean`
*   **Rebuild:** `make re`

## Development Conventions
*   **Coding Standard:** STRICT adherence to the **42 Norm**.
    *   Max 25 lines per function.
    *   Max 5 functions per file.
    *   Max 80 columns per line.
    *   Specific variable naming and file headers.
    *   Forbidden functions must be avoided (except those explicitly allowed).
*   **Commit Messages:** Follow **Conventional Commits** (e.g., `feat:`, `fix:`, `docs:`, `refactor:`).
*   **Forbidden Functions:** `malloc`, `free` (mostly avoided due to static allocation strategy, though standard 42 projects usually allow them). *Note: `libft` might contain mallocs, but the main shell logic aims to avoid them per `ARCHITECTURE.md`.*

## Directory Structure
*   `main.c`: Entry point.
*   `includes/`: Header files (`minishell.h`).
*   `libft/`: Custom standard library implementation (42 Libft).
*   `init/`: Initialization logic (e.g., environment setup).
*   `signals/`: Signal handling logic (`signals.c`).
*   `docs/`: Documentation (`ARCHITECTURE.md`, `README.md`, `CONTRIBUTING.md`).
*   `parse/`: (Placeholder) Parsing logic.
*   `readline/`: (Placeholder) Custom readline implementation or integration.


## Expected answering style
*	Don't edit the code unless specified
*	Be detailed in the explanations