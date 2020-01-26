 # MinimalistiC by Chaidhat Chaimongkol
### Programming Language & Compiler started 27.08.19
![MinimalistiC Programming Language & Compiler](https://github.com/Chai112/MinC-Compiler/blob/master/doc/thumb.png)

## About
This is a personal project of mine to...
1. learn how compilers work, writing low level languages, CPU architecture.
2. offer this as a learning experience by publically sharing my ideas and documented src code
3. create a super-simple C-like programming language as a good starting step for LL programming
4. (learn to use VIm)

it's a fun challenge.\
\
I aim to develop a basic compiler which compiles my variant of C (called MinimalistiC or MinC)
into x86 assembly as a *.s* file in the most simple way so others can learn from the source code.

## Table of Contents
[MinimalistiC Programming Language](#minimalistiC-programming-language)\
[MinimalistiC Compiler (MCC)](#minimalistic-compiler-(mcc))\
[Installation](#installation)\
[Thanks & Bibliography](#thank-you-to)

### To learn more, please read the sections below on the programming language and its compiler. Installation instructions are at the bottom 

## MinimalistiC Programming Language
MinimalistiC (MinC) is my take on a ultra-simplified, ultra-lightweight, derated version of the C Programming language.\
MinC's aims to be...
* **minimalised**, very easy to teach/learn as it lacks niches and only offers essential programming concepts
* **consistent**, pragmatic use of syntax to easily understand & adapt
* **forward compatible** with C - it can be (almost) be treated as C code and be compiled & optimised with GCC/[TCC](https://bellard.org/tcc/)
* **lightweight**, requires a smaller library and compiler than C/C++, faster compilation, smaller memory footprint
* **lower-level**, more finely controlled optimisations such as memory management without relying on an assembler
**Most importantly, it aims to serve as a good starting step for begginner programmers & learning experience for me**\

**All Features**
* preprocessor directives (`#include`, `#define`, `#ifdef`, `#endif`)
* preprocessor macros (`__FILE__`, `__LINE___`, `__TIME__`, `__ASM`, etc.)
* comments (`//`, `/*`, `*/`)
* data types (`byte`, `int`)
* static declarations (`static`) 
* pointers (`*`, `&`)
* *very* limited use of arrays (a lá ptr math, declare with `[`, `]`)
* string literals (`"`) to char arrays
* code structures & scope (`{`, `}`, `.`)
* basic data structures (`struct`, `union`, `.`, `,`, `;`)
* functions (`return`)
* conditionals (`if`)
* control flow (`while` loops)
* arithmetic (`+`, `-`, `*`, `/`) with (`(`, `)`) (with bidmas)
* binary logic (`!`, `&&`, `||`)
* equality testing (`==`, `!=`, `>`)
* linked with the C standard library for portability

That's all. 8 keywords, 4 preprocessor directives,\
and a charset of `a..z`, `0..9` with 20 symbols `. , ; + - * / = # ! & | " > ( ) [ ] { }`\

## MinimalistiC Compiler (MCC)
Compiles MinC into 32-bit x86 assembly *.s* files then asks linker to link into binaries\
**What it does**
1. reads input char by char, being lexed into tokens
   * `file.c` and `io.c` take in the source code as a stream
   * `pp.c` for preprocesses input char stream.
   * `lex.c` tokenise into one of `NULL`, `LITERAL`, `IDENTIFIER`, `KEYWORD`, `SEPARATOR`, `OPERATOR`, or `END OF FILE`
   * `pp.c` parses preprocessor directives `#` and macros
   * if `-E` flag, `dump.c` prints preprocessed code
2. parses token stream in `parse.c` (handwritten parser!) into an Abstract Syntax Tree
   * `parse.c` parses tokens into abstract instructions
   * parser is a top-down recursive decent parser, a *headache* to write
   * parser is hand-written by me specifically for MinC as opposed to FLEX/YACC
   * if `-fd` flag, `dump.c` pretty-prints the AST in readable form
3. generate an Intermediate Representation based on that AST
   * `gen_ir.c` generates linear IR with infinite registers from AST
   * `gen_ir.c` preforms a postal-order traversal of the AST
   * SSA or DAG optimisation is not used, to keep it simple 
   * `memalloc.c` allocates space on stack from infinite registers
   * `gen_x86.c` formats linear IR as actual x86 32-bit code in AT&T formatting
   * if `-S` flag, `dump.c` outputs the human-readble IR as an `.s`
5. assembly and linking are done externally
   * GNU's assembler (`as`) converts the human-readable x86 `.s` to `.o` files
   * if `-c` flag, `dump.c` outputs the object file as an `.o`
   * GNU's linker (`ld`) links all `.o` files together and with the C standard library
   * code is linked with the C standard library for portability across OSes

Notes
   * MCC is so small it can fit into a 1980s floppy disk (>160KB)
   * parser rearranges binary expressions to respect precedence such as BIDMAS (insanely confusing to write)
   * generator is non-optimising to keep it simple and easy to understand
   * since mingw has no r10-16 registers, memalloc pushes and pops the stack instead.
   * memory management is absent (declares variables on heap but never frees it) 
   * semantic analysis is non-existent and allows implicit type casting (odd quirk)
   * error-checking is minimal too, so errors may be uncaught

## Install
The compiler generates x86 32-bit assembly code for Intel CPUs.
\
Should work for 32-bit Unix-like OSes and 32-bit Windows. \
Tested on Windows 10 and Mac OS X Mojave. \
Will not work on Catalina as it has no legacy support for 32-bit applications.\
\
[MinGW](https://osdn.net/projects/mingw/releases/) is required for assembly and linking.\
Uses GNU's assembler (GAS) and linker.
Links with C Standard library for portability across all OSes.
\
Please download the repository and cd into it using command prompt or terminal.\
Make sure [GNU Make is installed](http://gnuwin32.sourceforge.net/packages/make.htm), do
```
make all
```
The source code will be compiled by GNU Make into an executable, mcc in the bin file.\
Compilation tests will be perfomed automatically after successful compilation by Make.\
\
Run the executable with -h flag for instructions. *(Unix terminal)*
```
cd bin ; ./mcc -h
```
or give it a MinC file, dump parser's AST `-fd` and verbosely compile `-v` into executable
```
./mcc -fd -v test/file.mc
```

## Thank you to 
### compiler 
* Introduction to Compilers and Language Designs by Prof. Douglas Thain (book)
* http://www.cs.man.ac.uk/~pjj/farrell/compmain.html
* http://lisperator.net/pltut/
* https://github.com/rui314/8cc for inspiration
* https://github.com/rui314/9cc for inspiration
* https://github.com/nlsandler/nqcc for inspiration
### parser  
* http://lisperator.net/pltut/parser/
* https://stackoverflow.com/questions/2245962
### lexer
* http://www.cse.chalmers.se/edu/year/2015/course/DAT150/lectures/proglang-04.html
### code generator
* https://en.wikibooks.org/wiki/X86_Assembly/GAS_Syntax
* https://www.chocolatesparalucia.com/2010/09/hello-world-c-and-gnu-as/
### x86
* https://cs.lmu.edu/~ray/notes/gasexamples/
* https://www.cs.virginia.edu/~evans/cs216/guides/x86.html
* https://pages.hep.wisc.edu/~pinghc/x86AssmTutorial.htm
* https://www.youtube.com/watch?v=wLXIWKUWpSs
