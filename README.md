# POSIX Shell Implementation

This project implements a POSIX-compliant shell with various built-in commands and functionalities, including support for piping, input/output redirection, and tab-based autocomplete. Below is a breakdown of the core components and their respective functionalities:

## Features and Components

### 1. `cd.cpp` - Change Directory
This file handles the `cd` (change directory) command using the `chdir` system call to change the working directory of the process. It implements error handling to manage invalid directory changes.

### 2. `ls.cpp` - List Files and Directories
The `ls` command is implemented using the `opendir` and `readdir` functions from the `<dirent.h>` library. It supports the following flags:
- `-a`: Lists all files, including hidden files.
- `-l`: Provides detailed file information, such as permissions, ownership, and size.

### 3. `echo.cpp` - Echo Command
This module implements the `echo` command, which outputs any text following the command. It handles cases like printing multiple arguments and escaping special characters.

### 4. `search.cpp` - File and Directory Search
A breadth-first search (BFS) is used to search through files and directories in the current working directory. If a directory is encountered, it is added to the queue, and the search continues within it.

### 5. `process_info.cpp` - Process Information
The `pinfo` command is implemented here, providing information about a process by reading from the `/proc/<pid>/stat` and `/proc/<pid>/cmdline` files. This allows users to view process status and command line arguments of a specific PID.

### 6. `syscomm.cpp` - System Command Execution
This file handles the execution of external system commands using the `execvp` function. It takes care of launching system programs that are not built into the shell.

### 7. `cmdparser.cpp` - Command Parsing
The command parsing logic resides here, categorizing input into:
- Piped commands
- Input/output redirection commands
- Simple commands

It breaks down commands for further processing by other modules.

### 8. `inout.cpp` - Input and Output Redirection
This file implements input and output redirection using the `dup2` system call. It handles cases where the user redirects the input or output of a command to or from a file.

### 9. `pipe.cpp` - Pipe Handling
The pipe functionality is implemented here using the `pipe` system call. This enables inter-process communication by creating pipes for commands connected with the pipe (`|`) symbol.

### 10. `execute.cpp` - Command Execution
This module is responsible for segregating simple commands (those without pipes or redirection) and mapping them to their respective built-in commands (`cd`, `pwd`, `echo`, `ls`, `search`) or system commands.

### 11. `prompt.cpp` - Main Shell Loop and Autocomplete
This file contains the main shell loop that takes user input and manages the shell's core functionality. It also includes the tab-based autocomplete feature for commands and filenames, enhancing user experience.

### 12. `header.h` - Header File
The `header.h` file is used to declare and share functions across different source files, facilitating modularity and better organization of the shell's code.

## How to Run

1. Clone the repository.
2. Compile the source files using `make`.
3. Run the shell by executing the compiled binary.

## Future Enhancements
- Adding support for more built-in commands.
- Improving error handling for various edge cases.
- Enhancing performance for complex commands with large directories.



