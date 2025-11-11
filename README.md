# Simple Shell Project

This project is a simplified version of a UNIX command interpreter.  
It reads a command from the user, interprets it, and executes it just like the sh shell.

## What this shell will eventually do:
- Read user input
- Find the correct command using PATH
- Execute commands using fork and execve
- Support built-in commands such as exit and env
- Work in both interactive and non-interactive modes

## How to compile:
The project will be compiled using the following command:
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

## Team Members:
Nouf Saud Almutairi  
Sara Ahmed AlMawkil
