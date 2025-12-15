# Architecture Documentation

## Data Flow

## Structs

# Minishell Structures


For this project, we have decided not to use malloc, avoiding memory allocation on the heap, and moving the necessary buffers to the stack and, primarily, to .bss and .data. The reason is mostly personal: to learn practices more often required in embedded system software development. By avoiding the heap, we prevent one of the most common bugs in C programming—not freeing allocated memory.


Furthermore, it forces us to anticipate memory usage from the beginning of the program and to set limits on data structures that might otherwise grow dynamically without bounds. This allows us to know the system's worst-case memory performance from the start. The other advantage is execution speed, as we do not have to search the heap.

There are many disadvantages: more complexity in development of the project, the need for artificial bounds to the memory, an unnecessary heavier program on average… Still, it’s a choice of personal development and not of practicality.


## Memory Pools


We use two static memory pools, one for the input line and another one for the Abstract Syntax Tree, local and environment variables lists and command environment. They both have a semi-arbitrary size, given that there are no bottlenecks in other places of the program that limit the functionality of the shell. For example, there’s no limit for the number of redirections of a command, so in principle you can redirect until you fill the RAM.


There are, nonetheless, some bottlenecks that when considered, can give us an approximation of a size that makes sense. For example, execve just accepts a total of ~2MB of  arguments (args + envp), there’s a limit to the opened file descriptors at the same time (for the mandatory part, that implies a limit of commands), 


### Local and Environment Variable Lists


We use two separate arrays of structs, each containing two strings: one for key and one for value. The advantage over an array of strings is that we do not have to iterate through each entry repeatedly to find both. Since they are statically allocated in memory, the method for removing a variable is to copy the subsequent variables one position backward in the array, starting with the immediately following one, and then reset the last initialized position to 0.


### Tokens and Expanded Tokens


For tokenization we create an Abstract syntax tree in the next form:

![AST](./Screenshot_20251215_120314.png)


### Command Environment


A struct containing all the information each command needs for execution:
The command to execute (with or without a path, string).
The command arguments, the names of the files for input and output redirection (if any, strings), and their mode (in case it is appended instead of truncate).
Heredoc if present (we have decided to simplify and pass a string directly instead of writing to temporary files).
Pointers to the aforementioned variable lists.
### Global Variable


A single global variable is used to capture signal delivery and handle them differently within a heredoc, open quotes or operators, and in parent and child processes. The type is volatile sigatomic int.



## Signal Handling

