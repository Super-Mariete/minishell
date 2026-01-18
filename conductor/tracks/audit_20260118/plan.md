# Implementation Plan - Stability & Compliance Audit

## Phase 1: Environment & Static Analysis
Goal: Ensure the codebase builds cleanly and meets style standards.

- [x] Task: Environment Health Check
    - [x] Verify `make` compiles the project without errors or warnings.
    - [x] Run `make clean`, `make fclean`, and `make re` to ensure build targets work as expected.
- [x] Task: Norminette Compliance Audit
    - [x] Run `norminette` on `libft/`.
    - [x] Fix any style errors in `libft/`.
    - [x] Run `norminette` on `parsing/` and `exec/`.
    - [x] Fix any style errors in `parsing/` and `exec/`.
    - [x] Run `norminette` on `minishell.c` and `minishell.h`.
    - [x] Fix any style errors in core files.

## Phase 2: Memory Safety & Leak Detection
Goal: Eliminate memory leaks and invalid accesses.

- [ ] Task: Valgrind Baseline
    - [ ] Run a simple command (e.g., `echo hello`) under Valgrind to establish a baseline.
    - [ ] Analyze reports for unclosed file descriptors.
- [ ] Task: Fix Memory Leaks in Parsing
    - [ ] Analyze AST destruction logic (`t_cli` freeing).
    - [ ] Verify environment (`t_shenv`) cleanup on exit.
    - [ ] Run parser-heavy tests under Valgrind and fix identified leaks.
- [ ] Task: Fix Memory Leaks in Execution
    - [ ] Check for unclosed pipes in `exec_pipe.c`.
    - [ ] Verify child process cleanup.
    - [ ] Run execution-heavy tests (pipes, redirections) under Valgrind and fix leaks.

## Phase 3: Functional Verification
Goal: Ensure no regressions and full functional correctness.

- [ ] Task: Integration Test Suite
    - [ ] Run `tests/tests.sh` (or equivalent provided test script).
    - [ ] Triage any failing tests.
    - [ ] Fix bugs for confirmed failures.
- [ ] Task: Edge Case Validation
    - [ ] Manual test: Empty command.
    - [ ] Manual test: Spaces only.
    - [ ] Manual test: Unset variables.
    - [ ] Manual test: Signal handling (Ctrl-C, Ctrl-D, Ctrl-\) in blocking and non-blocking modes.

## Phase 4: Final Polish
Goal: Prepare the project for "release" or further feature development.

- [ ] Task: Documentation Update
    - [ ] Update `README.md` with build instructions and current status.
    - [ ] Ensure `conductor/index.md` and architecture docs are aligned with any findings.
