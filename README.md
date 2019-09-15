# Minimal-C Programming Language & Compiler
### Made by Chaidhat Chaimongkol on 27 August 2019
## Minimal-C
"*C is too powerful*" - no one ever.\
Minimal-C (Min-C) is my take on a simplifed, derated version of the C Programming language.\
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
**What it does**
* reads input char by char, being lexed and parsed one by one into an AST
