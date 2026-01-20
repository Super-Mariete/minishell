# Specification: Stability & Compliance Audit

## Context
The project is a "brownfield" Minishell implementation. While the core functionality exists, the "Product Vision" demands uncompromising stability (100% leak-free, robust error handling) and professional presentation (Norminette compliance). This track serves as the baseline assessment and remediation phase.

## Goals
1.  **Static Analysis:** Ensure the entire codebase (`.c` and `.h` files) passes `norminette` v3 without errors.
2.  **Memory Safety:** Verify zero memory leaks and invalid reads/writes using Valgrind on the provided test suite.
3.  **Functionality:** Confirm that existing integration tests (`tests/tests.sh`) pass.
4.  **Documentation:** Update README to reflect current build/test status.

## Scope
- **In Scope:**
    - `minishell.c` (Core)
    - `parsing/` (Lexer, Parser, Expansion)
    - `exec/` (Execution, Builtins)
    - `libft/` (Utils)
    - `tests/` (Test scripts)
- **Out of Scope:**
    - Adding new features (e.g., bonus features like wildcards if not already present, though code analysis suggests they are).
    - Major architectural refactoring (unless required to fix critical bugs).

## Success Criteria
- `norminette` returns `OK` for all source files.
- `make` compiles with `-Wall -Wextra -Werror` and no warnings.
- `tests/tests.sh` executes with 100% pass rate (or documented known failures).
- Valgrind report shows "0 errors from 0 contexts" (using `readline.supp` to suppress external library leaks).
