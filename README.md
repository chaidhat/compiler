# MinimalistiC by Chaidhat Chaimongkol
### Programming Language & Compiler started 27.08.19
![MinimalistiC Programming Language & Compiler](https://github.com/Chai112/MinC-Compiler/blob/master/doc/thumb.png)

## About
This is a personal project of mine of trying to teach myself...
1. how compilers work
2. how to write assembly (for an x86_64 Intel architecture)
3. modifying the C programming language
4. (teach myself how to use Vim)

it's a fun challenge.\
\
I aim to develop a ultra-simple, easy to read, inefficient compiler which compiles my variant of C (called MinimalistiC or MinC)
into x86_64 assembly as a *.o* file. No optimisations, as little confusion as possible (I hope) so that other people can look
at the source code and understand what on earth is going on. 

## Install
Mac & Windows are OK, tested on Mac\
Please download the repository and cd into it via console prompt or terminal.\
Make sure [GNU Make is installed](http://gnuwin32.sourceforge.net/packages/make.htm), do
```
make
```
The source code will be compiled by GNU Make into an executable, mcc in the bin file.\
Run the executable with -h flag for instructions.
```
./mcc -h
```
doing `make all` will auto build, self-run an test program and clean the directory.
## MinimalistiC
"*C is too powerful*" - no one ever.\
MinimalistiC (MinC) is my take on a ultra-simplified, derated version of the C Programming language.\
Mainly made because I neither got the skill or patience to fully follow the C ISOs to call it a 'C Compiler'. Note that there are no additional changes to C, only removals.

**Capabilities**
* preprocessor directives (`#include`, `#define`, `#ifdef`, `#endif`)
* preprocessor macros (`__FILE__`, `__LINE___`, `__TIME__`, `__ASM`, etc.)
* comments (`//`, `/*`, `*/`)
* data types (`char`, `int`)
* pointers (`*`, `&`)
* *very* limited use of arrays (`[`, `]`)
* string literals (`"`) to char arrays
* code structures & scope (`{`, `}`)
* basic data structures (`struct`, `union`, `.`, `,`, `;`)
* functions (`void`, `return`)
* conditionals (`if`)
* control flow (`while` loops)
* arithmetic (`+`, `-`, `*`, `/`) with (`(`, `)`) (with bidmas)
* binary logic (`!`, `&&`, `||`)
* equality testing (`==`, `>`)
* user I/O

That's it. 8 keywords, 4 preprocessor directives,\
and a charset of `a..z`, `0..9` with 20 symbols `. , ; + - * / = # ! & | " > ( ) [ ] { }`

## MinimalistiC Compiler (MCC)
Compiles MinC into x86_64 assembly *.o* files then asks linker to link into binaries\
**What it does**
1. reads input char by char, being lexed into tokens
   * `file.c` and `io.c` take in the source code as a stream
   * `pp.c` for preprocesses input char stream.
   * `lex.c` tokenise into one of `NULL`, `LITERAL`, `IDENTIFIER`, `KEYWORD`, `SEPARATOR`, `OPERATOR`, or `END OF FILE`
   * `pp.c` parses preprocessor directives `#` and macros
2. parses tokens in `parse.c` one by one into an Abstract Syntax Tree using a top-down recursive descent algorithm
3. generate an Intermediate Representation based on that AST
4. assemble code from the IR, as an *.o* file
5. ask the system's linker (`ld`) to link the file. I ain't writing that.

## Thank you to 
### compiler 
* http://www.cs.man.ac.uk/~pjj/farrell/compmain.html
* http://lisperator.net/pltut/
### parser  
* http://lisperator.net/pltut/parser/
* https://stackoverflow.com/questions/2245962
### lexer
* http://www.cse.chalmers.se/edu/year/2015/course/DAT150/lectures/proglang-04.html
