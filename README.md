# Minimal-C Programming Language & Compiler
### Made by Chaidhat Chaimongkol on 27 August 2019
## About
This is a personal project of mine of trying to teach myself...\
a. how compilers work\
b. how to write assembly (for an x86_64 Intel architecture)\
c. modifying the C programming language\
a fun challenge.\
\
I aim to develop a very simple, easy to read, inefficient compiler which compiles my variant of C (called Minimal-C or Min-C)
into x86_64 assembly as a .o. No optimisations, as little confusion as possible (I hope) so that other people can look
at the source code and understand what on earth is going on. 
## Minimal-C
"*C is too powerful*" - no one ever.\
Minimal-C (Min-C) is my take on a simplified, derated version of the C Programming language.\
Mainly made because I neither got the skill or patience to fully follow the C ISOs to call it a 'C Compiler'. 
\
**What it introduces**
* nothing

**What it keeps from C**
* preprocessor statements and macros (#include, #define, #ifdef, etc.)
* standard Min-C library
* comments
* data types (char, short, int, double)
* pointers (*, &)
* *very* limited use of arrays
* *very* limited use of strings (char arrays)
* structs
* scope
* functions
* if statements
* control flow (for, while loops)
* running system commands
* user I/O

And some operators you might expect

* arthimetic (+, -, \*, /, %) with parentheses
* boolean logic (!, &&, ||)
* bitwise logic (~, &, |, ^)
* bitwise shift (<<, >>)
* assignment (=)
* equality testing (!=, ==)


## Min-C Compiler 
Compiles Min-C into x86_64 assembly .o. Then asks linker to link it into a binary
**What it does**
* reads input char by char, being lexed and parsed one by one into an AST
* generates IR based on that AST
* assemble code from the IR, as an .o file
* ask the system's linker (ld) to link the file. I ain't writing that.
