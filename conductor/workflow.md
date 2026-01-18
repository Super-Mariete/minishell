# Workflow & Development Standards

## Development Lifecycle

### 1. Branching Strategy
- **Main Branch:** `master` (or `main`) is the source of truth. It must always be compilable and pass all critical tests.
- **Feature Branches:** Create short-lived branches for new features or bug fixes (e.g., `feat/parser-expansion`, `fix/pipe-fd-leak`).
- **Merge Criteria:** Code must pass `norminette` and the integration test suite before merging.

### 2. Implementation Loop
1.  **Plan:** Identify the module (Lexer, Parser, Exec) and specific function requirements.
2.  **Code:** Implement strict C code adhering to 42 Norminette.
3.  **Style Check:** Run `norminette` frequently on modified files.
4.  **Unit Test:** If applicable, create small reproduction cases or unit tests in `tests/`.
5.  **Build:** Verify compilation with `make` (ensure no warnings with `-Wall -Wextra -Werror`).

### 3. Verification & Quality Assurance
- **Integration Testing:** Run the main test suite:
  ```bash
  ./tests/tests.sh
  ```
- **Memory Analysis:** Critical for this project.
  - Run with Valgrind to ensure no leaks:
    ```bash
    valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell
    ```
  - Verify all file descriptors are closed properly.

### 4. Commits
- **Format:** Use conventional commit style (e.g., `feat(parser): add wildcard expansion`, `fix(exec): close unused pipe fds`).
- **Granularity:** Keep commits atomic. One logical change per commit.

## Tooling Configuration
- **Build System:** `Makefile` is the single source of build truth.
  - `make`: Compiles the release binary.
  - `make re`: Rebuilds from scratch.
  - `make clean/fclean`: Cleans artifacts.
- **Static Analysis:** `norminette` must return `OK` for all source files.
