 # MinimalistiC by Chaidhat Chaimongkol
### Programming Language & Compiler started 27.08.19
![MinimalistiC Programming Language & Compiler](https://github.com/Chai112/MinC-Compiler/blob/master/doc/thumb.png)

## About
This is a personal project of mine of trying to teach myself...
1. how compilers work
2. how to write assembly (for an x86_64 Intel architecture)
3. the C programming language
4. (teach myself how to use Vim)

it's a fun challenge.\
\
I aim to develop a ultra-simple, easy to read, compiler which compiles my variant of C (named MinimalistiC or MinC)
into x86_64 assembly as a *.o* file in as little confusion as possible (I hope) so others make sense of the source code. 

## Install
The compiler generates x86 assembly code for Mac OS X and Ubuntu.\
[NASM](https://www.nasm.us/pub/nasm/releasebuilds/?C=M;O=D) is required for assembly.\
[MinGW](https://osdn.net/projects/mingw/releases/) is required for linking.\
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
or give it a MinC file, dump parser's AST `-fdast` and verbosely compile `-v` into executable
```
./mcc -fdast -v test/file.mc
```

## MinimalistiC Programming Language
"*C is too powerful*" - no one ever.\
MinimalistiC (MinC) is my take on a ultra-simplified, ultra-lightweight, derated version of the C Programming language.\
MinC's aims to be...
* consistent with C so it does not add anything new, only removals and a few changes 
* very easy to learn and memorise due to it's lack of niches and feature
* heavily-simplifed and contains only the bare-minimum of C

**All Capabilities**
* preprocessor directives (`#include`, `#define`, `#ifdef`, `#endif`)
* preprocessor macros (`__FILE__`, `__LINE___`, `__TIME__`, `__ASM`, etc.)
* comments (`//`, `/*`, `*/`)
* data types (`byte`, `int (4 bytes)`)
* hex, octal, binary representations
* static declarations (`static`) 
* pointers (`*`, `&`)
* *very* limited use of arrays (a lá ptr math, declare with `[`, `]`)
* string literals (`"`)
* code structures & scope (`{`, `}`, `.`)
* basic data structures (`struct`, `union`, `.`, `,`, `;`)
* functions (`return`)
* conditionals (`if`)
* control flow (`while` loops)
* unary operations (`+`, `-`, `*`, `/`) with (`(`, `)`) (with bidmas/precedence)
* bitwise operations (`~`, `&`, `<<`, `>>`)
* binary logic (`!`, `&&`)
* equality testing (`==`, `!=`, `<`, `>`)
* standard library

That's all. 8 keywords, 4 preprocessor directives,\
and a charset of `a..z`, `0..9` with 21 symbols `. , ; + - * / = # ! & ~ " < > ( ) [ ] { }`\

## MinimalistiC Compiler (MCC)
Compiles MinC into x86_64 assembly *.o* files then asks linker to link into binaries\
**What it does**
1. reads input char by char, being lexed into tokens
   * `file.c` and `io.c` take in the source code as a stream
   * `pp.c` for preprocesses input char stream.
   * `lex.c` tokenise into one of `NULL`, `LITERAL`, `IDENTIFIER`, `KEYWORD`, `SEPARATOR`, `OPERATOR`, or `END OF FILE`
   * `pp.c` parses preprocessor directives `#` and macros
2. parses token stream in (handwritten!) `parse.c` into an Abstract Syntax Tree using a top-down recursive descent algorithm
   * `parse.c` parses tokens into abstract instructions recursively
   * parser is handwritten for better performance than FLEX or BISON - also a *headache* to write
   * parsing is recursively called, so that is correctly mapped as a syntax tree (AST)
   * binary expressions rearrange themselves to follow BIDMAS (insanely confusing too)
   * memory management is absent (declares variables on heap but never frees it) 
   * `dump.c` prints the instructions in english if `-fdast` (Dump-Abstact-Syntax-Tree) arg
3. generate an Intermediate Representation based on that AST
4. assembly and linking are done externally using NASM (`nasm`) and GNU's/MSVC's linker (`ld`/`link`)

## Thank you to 
### compiler 
* Introduction to Compilers and Language Designs by Prof. Douglas Thain (book)
* http://www.cs.man.ac.uk/~pjj/farrell/compmain.html
* http://lisperator.net/pltut/
### parser  
* http://lisperator.net/pltut/parser/
* https://stackoverflow.com/questions/2245962
### lexer
* http://www.cse.chalmers.se/edu/year/2015/course/DAT150/lectures/proglang-04.html
### code generator
* https://cs.lmu.edu/~ray/notes/gasexamples/
