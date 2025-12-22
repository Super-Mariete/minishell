# Minishell Testing Guide

This document outlines the testing strategy, tools, and procedures for the Minishell project. Given our constraint of avoiding dynamic memory allocation (`malloc`), testing focuses heavily on boundary conditions and state management within static memory pools.

## 1. Testing Philosophy
- **Memory Safety:** Every execution is monitored with Valgrind to ensure zero leaks and no invalid memory accesses within our static pools.
- **Boundary Testing:** We test the limits of `ARG_MAX` and other buffers defined in `includes/msh_limits.h`.
- **Regression Testing:** Automated scripts ensure that refactors (like moving initialization logic) do not break existing functionality.

## 2. Prerequisites
The following tools are required for the full test suite:
- `valgrind`: For memory leak and error detection.
- `cppcheck`: For static code analysis.
- `norminette`: To ensure 42 Norm compliance.
- `bash`: Used as the reference shell for integration tests.

## 3. Directory Structure
- `tests/`: Contains all testing-related files.
    - `tests.sh`: The main test runner script.
    - `test_*.c`: Unit test implementations (e.g., `test_init_env.c`).
    - `to_be_tested/`: Input files containing test cases and command strings.
    - `old_tests/`: Legacy scripts kept for reference.

## 4. How to Run Tests

### 4.1. Full Automated Suite
To run all unit tests and check memory safety:
```bash
bash tests/tests.sh
```
This script will:
1. Compile the unit tests.
2. Run them normally to check for logic errors.
3. Run them through Valgrind to check for memory errors.
4. Generate timestamped logs in the `tests/` directory.

### 4.2. Static Analysis
To run `cppcheck` across the source code:
```bash
make check
```

### 4.3. Norm Compliance
To check if the source code adheres to the 42 Norm:
```bash
norminette main.c includes/ init/ signals/
```

## 5. Adding New Tests

### 5.1. Adding a Unit Test
1. Create a new file in `tests/test_<feature>.c`.
2. Include the test header: `#include "test.h"`.
3. Implement a `main` function that exercises the specific logic.
4. Update `tests/tests.sh` to include your new test file in the `ft_test_<feature>` loop.

### 5.2. Adding Integration Cases
Add command strings or environment variable sets to the relevant text file in `tests/to_be_tested/`. The runner script will iterate through these inputs.

## 6. Debugging & Logs
When a test fails, check the generated log directories:
- **Normal Log:** Standard output and error of the test.
- **Valgrind Log:** Detailed memory reports.
- **Debug Log:** Internal state prints if the test was compiled with debug flags.
