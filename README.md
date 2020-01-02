 # MinimalistiC by Chaidhat Chaimongkol
### Programming Language & Compiler started 27.08.19
![MinimalistiC Programming Language & Compiler](https://github.com/Chai112/MinC-Compiler/blob/master/doc/thumb.png)

## About
This is a personal project of mine of trying to teach myself...
1. how compilers work
2. how to write assembly (for an x86 Intel & CPU architecture)
3. the C programming language
4. (teach myself how to use Vim)

it's a fun challenge.\
\
I aim to develop a basic compiler which compiles my variant of C (called MinimalistiC or MinC)
into x86 assembly as a *.s* file in the most simple way so others can learn from the source code. \
### To learn more, please read the sections below on the programming language and its compiler. Installation instructions are at the bottom 

## MinimalistiC Programming Language
MinimalistiC (MinC) is my take on a ultra-simplified, ultra-lightweight, derated version of the C Programming language.\
MinC's aims to be...
* **heavily-simplifed** and contains only the bare-minimum of C
* **basic**, very easy to learn/teach as it lacks niches and has only basic/essential concepts
* **lightweight**, requires a smaller library and compiler than C/C++, faster compilation, smaller memory footprint
* **lower-level**, more finely controlled optimisations such as memory management without relying on assembler
* **forward compatible** with C - it can be treated as C code and be compiled & optimised with GCC/[TCC](https://bellard.org/tcc/)
* **Most importantly, a good learning experience for me to learn how compilers/low-level programming works**

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
Compiles MinC into x86 assembly *.o* files then asks linker to link into binaries\
**What it does**
1. reads input char by char, being lexed into tokens
   * `file.c` and `io.c` take in the source code as a stream
   * `pp.c` for preprocesses input char stream.
   * `lex.c` tokenise into one of `NULL`, `LITERAL`, `IDENTIFIER`, `KEYWORD`, `SEPARATOR`, `OPERATOR`, or `END OF FILE`
   * `pp.c` parses preprocessor directives `#` and macros
   * if `-E` flag, `dump.c` prints preprocessed code
2. parses token stream in (handwritten!) `parse.c` into an Abstract Syntax Tree
   * `parse.c` parses tokens into abstract instructions
   * parser is a top-down recursive decent parser, a *headache* to write
   * parser is hand-written by me specifically for MinC as opposed to FLEX/YACC
   * if `-fd` flag, `dump.c` pretty-prints the AST in readable form
3. generate an Intermediate Representation based on that AST
   * `gen_ir.c` generates IR as a Directed Acyclic Graph based on AST
   * `gen_ir.c` initally uses an infinite amount of registers
   * `map.c` converts DAG to linear IR based on x86 assembly
   * `mac.c` allocates physical registers from infinite registers
   * `gen_x86.c` formats linear IR as actual x86 32-bit code in AT&T formatting
   * if `-S` flag, `dump.c` outputs the human-readble IR as an `.s`
5. assembly and linking are done externally
   * GNU's assembler (`as`) converts the human-readable x86 `.s` to `.o` files
   * if `-c` flag, `dump.c` outputs the object file as an `.o`
   * GNU's linker (`ld`) links all `.o` files together and with the C standard library
   * code is linked with the C standard library for portability across OSes

Notes
   * MCC is so small it can fit into a 1980s floppy disk (>160KB)
   * parser rearranges binary expressions to follow BIDMAS (insanely confusing to write)
   * memory management is absent (declares variables on heap but never frees it) 
   * generator is non-optimising, so a lot of jumps and inefficient memory usage
   * semantic analysis and error-checking is limited which may result in un-caught errors

## Install
The compiler generates x86 32-bit assembly code for Intel CPUs.
\
Should work for Unix-like OSes and 32-bit Windows. \
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
