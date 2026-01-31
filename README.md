*This project has been created as part of the 42 curriculum by rafael-m and made-ped.*

# Minishell

## Description
Minishell is a comprehensive, simplified shell implementation inspired by **Bash** (Bourne Again SHell). The primary objective of this project is to deepen the understanding of process creation, synchronization, file descriptors, and the overall architecture of a shell.

This program parses user input, handles variable expansion, and executes commands by creating child processes. It supports a wide range of features including pipes, logical operators, redirections, signals, and environment management.

## Architecture & Design Decisions

### Data Structures
Instead of the traditional Abstract Syntax Tree (AST), this project utilizes a **doubly linked list** (`t_cli`) to manage command execution flow.
*   **Reasoning:** The decision to implement bonus features (logical operators `&&`, `||` and parentheses `()`) was made later in the development cycle. A linear linked list structure was adapted to handle these hierarchical operations by managing grouping and operator precedence logic within the execution phase (`exec_prnts.c`), rather than restructuring the entire parser into a tree.

### Environment Management
The environment is managed via a linked list (`t_shenv`), allowing for modifications of environment variables.
*   **Scope:** The shell implements a single, global environment scope. Local variables (e.g., `KEY=VALUE` without `export`) are not supported as persistent local variables, complying with the functional requirements of the subject without adding the complexity of varying variable scopes.

### Modular Parsing
The interpretation of input is divided into distinct phases:
1.  **Lexing & Tokenization:** Input is split into tokens, handling quotes and special separators.
2.  **Expansion:** Environment variables (`$VAR`) and wildcards (`*`) are expanded.
3.  **Parsing:** Tokens are organized into the `t_cli` execution list, associating arguments and redirections with their respective commands.

### Expansion Strategy
Variable expansion is performed globally after the lexing phase instead of within each command's individual execution environment.
*   **Reasoning:** This decision was made to better divide the development work between the two project members. By decoupling expansion from execution, one member could focus on the lexical analysis and variable resolution while the other focused on process management and execution.

### Decision Making

To implement built-in commands, the project uses a function pointer table instead of multiple if-else statements.
This approach improves code readability, maintainability, and follows structured and clean programming principles by centralizing decision logic and reducing conditional complexity.

## Instructions

### Compilation
To compile the project, run the following command in the root directory:
```bash
make
```
This will build the `libft` dependency and generate the `minishell` executable.

### Execution
Start the shell by running:
```bash
./minishell
```
You will be greeted with a prompt where you can type standard bash commands.

### Installation
```bash
git clone <git@github.com:Super-Mariete/minishell.git minishell
cd minishell
make
```

## Resources

### References
*   **GNU Bash Manual:** [https://www.gnu.org/software/bash/manual/](https://www.gnu.org/software/bash/manual/) - The primary reference for behavior compliance.
*   **Bash Source Code:** [http://git.savannah.gnu.org/cgit/bash.git/](http://git.savannah.gnu.org/cgit/bash.git/) - Used to verify specific implementation details of the original shell.

### AI Usage
Artificial Intelligence tools were utilized during the development of this project strictly for educational and clarification purposes:
*   **Code Understanding:** To clarify the behavior of complex C syntax or library functions.
*   **Logic Verification:** To ask questions about how specific shell features (like signal handling in child processes or pipe synchronization) interact in edge cases.
*   **Project Structure:** To discuss organizational strategies, such as the viability of using linked lists for the bonus part.
