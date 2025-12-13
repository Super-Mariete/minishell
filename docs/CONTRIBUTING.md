# Contributing Guidelines

## Coding Standards
The project adheres to the 42 Norm, a strict set of coding standards for C projects. Key rules include:

*   **File and Function Headers:** Specific comment blocks are required at the top of each file and before each function.
*   **Function Length:** Functions should not exceed 25 lines.
*   **File Function Count:** A single `.c` file should contain a maximum of 5 functions.
*   **Line Length:** Lines must not exceed 80 columns.
*   **Naming Conventions:** Adhere to `snake_case` for variables and `ft_` prefix for library functions.
*   **Global Variables:** Generally forbidden, with rare exceptions (e.g., for signal handling, limited to one global for signal number).
*   **Memory Management:** All dynamically allocated memory must be properly freed to prevent leaks.
*   **Forbidden Functions:** A specific list of forbidden functions must not be used (check `es.subject.pdf` for allowed functions).

Failure to comply with these standards will result in a penalty during evaluation.

## Git Workflow
We follow the Conventional Commits specification for commit messages. This helps in generating changelogs and understanding the purpose of each commit. Please use the following types:

*   **feat:** A new feature (e.g., `feat: implement pipes`)
*   **fix:** A bug fix (e.g., `fix: correct redirection parsing`)
*   **refactor:** A code change that neither fixes a bug nor adds a feature (e.g., `refactor: restructure lexer`)
*   **docs:** Documentation only changes (e.g., `docs: update README`)
*   **test:** Adding missing tests or correcting existing tests (e.g., `test: add unit tests for builtins`)
*   **chore:** Other changes that don't modify src or test files (e.g., `chore: update Makefile`)

Each commit message should start with a type, followed by a scope (optional, in parentheses), a colon, and a subject. For example: `feat(parser): add support for single quotes`.

## Error Handling

