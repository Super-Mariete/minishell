# Technology Stack

## Core Language & Runtime
- **Language:** C (POSIX standard compliant)
- **Runtime Environment:** Linux/Unix (using POSIX APIs for process management, file I/O, and signals)

## Key Libraries
- **GNU Readline:** Used for input handling, prompt management, and command history.
- **Custom Libft:** A comprehensive internal library of C utility functions (string manipulation, linked lists, memory utilities).

## Build & Project Management
- **Build System:** GNU Make (centralized `Makefile` managing compilation and dependencies).
- **Version Control:** Git.

## Testing & Quality Assurance
- **Verification Suite:** Custom integration test framework built in Bash (`tests/tests.sh`).
- **Memory Analysis:** Valgrind (used for leak detection and memory error verification, with a dedicated suppression file `readline.supp`).
- **Static Analysis:** Norminette (ensuring adherence to 42 School coding standards).
