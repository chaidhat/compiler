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
Mac & Windows are OK, tested on Mac\
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
Mainly because I don't want to follow C ISOs. \
It is faithful to C so it does not add anything new, only removals and a few changes.

**All Capabilities**
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
* standard library

That's it. 8 keywords, 4 preprocessor directives,\
and a charset of `a..z`, `0..9` with 20 symbols `. , ; + - * / = # ! & | " > ( ) [ ] { }`\
**It's incredibly easy to master compared to C.**

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
