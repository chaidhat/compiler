# MinimalistiC by Chaidhat Chaimongkol
### Programming Language & Compiler started 27.08.19, over 75 manhours across 5 months
![MinimalistiC Programming Language & Compiler](https://github.com/Chai112/MinC-Compiler/blob/master/doc/thumb.png)

## Table of Contents
* [**About**](#about)
* [MinimalistiC Programming Language](#minimalistic-programming-language)
* [MinimalistiC Compiler](#minimalistic-compiler)
* [Usage Guide](#usage-guide)
* [**Learning/Teaching Guide**](#learning-guide)
* [Example](#example)
* [**Development Process & Log**](#devlog)
* [Install](#install)
* [Bibliography](#thank-you-to)

### To learn more, please read the sections below on the programming language and its compiler. Installation instructions are at the bottom

## About
This is a personal project of mine to...
1. learn how compilers work, writing low level languages, CPU architecture.
2. offer this as a learning experience by publically sharing my ideas and documented src code
3. create a super-simple C-like programming language as a good starting step for LL programming
4. (learn to use VIm)
### Mission
I aim to develop a programming language for beginners to learn basic programming concepts & conventions.\
\
MinC has very limited set of features and consistent syntax for the leaner to keep focused on *a) learning new techniques* and *b) practicing learned techniques* without getting distracted on calling external libaries (such as in Java/Python) or unituitive semantics (C++). It performs extremely well as it is a compiled language (unlike JS or Java) and can do just as much as C can. Find out more: [Learning/Teaching Guide](#learning-guide).\
\
In this repository is a basic compiler which compiles my variant of C (called MinimalistiC or MinC)
into x86 assembly as a *.s* file in the most simple way so others can learn from the source code.

## MinimalistiC Programming Language
MinimalistiC (MinC) is my take on a ultra-simplified, ultra-lightweight, derated version of the C Programming language.
### Aims
MinC's aims to be...
* **minimal**, very easy to teach/learn as it lacks niches and only offers essential programming concepts
* **consistent**, pragmatic use of syntax to easily understand & adapt
* **forward compatible** with C - it can be (almost) be treated as C code and be compiled & optimised with GCC/[TCC](https://bellard.org/tcc/)
* **lightweight**, requires a smaller library and compiler than C/C++, faster compilation, smaller memory footprint
* **lower-level**, more finely controlled optimisations such as memory management without relying on an assembler
* **Most importantly, it aims to keep learners focused on *a) learning new techniques* and *b) practicing learned techniques***

### All Features
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

## MinimalistiC Compiler
MinC Compiler `mcc` compiles MinC to 32-bit x86 assembly *.s* files.
### Summary
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
   * `memalloc.c` manipulates memory on stack from infinite registers
   * `gen_x86.c` formats linear IR as actual x86 32-bit code in AT&T formatting
   * if `-S` flag, `dump.c` outputs the human-readble IR as an `.s`
5. assembly and linking are done externally
   * GNU's assembler (`as`) converts the human-readable x86 `.s` to `.o` files
   * if `-c` flag, `dump.c` outputs the object file as an `.o`
   * GNU's linker (`ld`) links all `.o` files together and with the C standard library
   * code is linked with the C standard library for portability across OSes
   
### Notes
   * MCC is so small it can fit into a 1980s floppy disk (>160KB)
   * parser rearranges binary expressions to respect precedence such as BIDMAS (insanely confusing to write)
   * generator is non-optimising to keep it simple and easy to understand
   * mingw is 32-bit normally, so asm is also 32 bits
   * memalloc pushes and pops the stack instead of allocating r10-16 as GAS doesn't support it
   * compiler memory management is absent (declares variables on heap but never frees it) 
   * semantic analysis is non-existent therefore allowing implicit type casting (odd quirk)
   * error-checking is minimal too, so errors may be uncaught
   
## Usage Guide
The compiler can build MinC files `.mc` to\
GAS assembly `.s`,\
object files `.o`,\
MinC ast `.md`,\
MinC preprocessed files `.mi`\
**or executables `.exe`.**\
\
To compile verbosely into x86 (Powershell):
```
./mcc -g -S code.mc
```
To compile & execute .exe (Powershell):
```
./mcc -o code code.mc ; ./code
```
Full instructions:
```
usage: mcc [-h] [-V] [arg1 arg2 ...] <inpath1 inpath2 ...>

mcc only accepts .mc as inpath, dir and include accepts any types
args:
   -V                  display version info
   -b                  display compilation benchmarks
   -o <path>           write output to <path>

   -g                  enable compiler debugger
   -w                  supress all warnings
   -W <wcode>          supress <wcode> warning
   -We                 treat all warnings as errors

   -I <dir>            add include path <dir>
   -D <macro> <val>    set <macro> to <val>
   -E                  preprocess only

   -S                  stop before assembly
   -c                  stop before linking
   -fd                 print AST

   -h                  display this help
 ```

   
## Learning Guide
### Prerequsites
The learner must intend to learn

## Example
This code demonstrates all features of MinC, excluding preprocessor directives\
**helloWorld.mc**
```
#include "standard.h"

int a; // external declaration
int func_a (byte a); // function declaration

// main function definition
int main ()
{
    int b; // declaration
    b = 0; // assignment
    static byte c = 1 + b; // static, byte type & unary arithmetic, casting
    byte *d = &c; // indirection
    byte e[3]; // array declaration
    e[0] = *d; // array assignment
    byte f[6] = "hello"; // assignment of string literals
    byte g = func_a(c); // assignment from function call
    // conditionals and equality testing
    if (g == 0 && e[0] == 1)
    {
        g = 2 >> 1; // bitwise operations
    }
    // while loop
    while (g > 0)
    {
        g = g - 2 + 1; // binary precedence with BIDMAS
    }
    printf("%s world %d\n", f, g); // standard library call
    return 0; // return
}
// struct definition
struct struct_a 
{
    // union definition
    union
    {
        int val_a = 0; // static assignment
        byte val_b;
    };
};
int func_a (byte a)
{
    struct_a s; // struct declaration
    s.val_a = 0; // struct member assignment
    s.val_b = a; // union member overwrite
    return s.val_b;
}
```
Powershell:
```
./mcc -o helloWorld helloWorld.mc ; ./helloWorld
hello world 0
```
## Devlog
**Development started 27.08.19 and over 75 manhours across 5 months were dedicated for this project**\
**All commits and changes can be viewed [here](https://github.com/Chai112/MinC-Compiler/commits/master)**\
There are all the weekly learning experience documented to show my progress of learning and developing a compiler and language - hopefully this will show problems which I faced and how I overcame themand will serve as a good reference for anyone who starts a project similar (or not) to this.
### Research & Inspirations
Inspired by [Nora Sandler's](https://norasandler.com/2017/11/29/Write-a-Compiler.html), [Rui Ueyama's](https://www.sigbus.info/how-i-wrote-a-self-hosting-c-compiler-in-40-days) and [this](https://www.freecodecamp.org/news/the-programming-language-pipeline-91d3f449c919/) article to make a C compiler, I started to write a compiler. I didn't really want to write a C compiler though, I could just copy code from the hundreds of repositories there are online, so I decided to write my own language where all my ideas could be original and not a replication of others. It was initially named BitC and was drastically different to C. It would be cool though if I wrote a MinC compiler which compiled itself so I could see how inefficient it got over the iterations of self-compilation.\
\
By this point, I was very familiar with C++ from my [OpenGL ESC engine](https://github.com/Chai112/ESC) and Facial Identification [AIFRED](https://github.com/Chai112/AIFRED) projects but relatively unaware of how featureless C was compared to C++.
I chose to write my compiler in C, seeing that it was a popular choice for most compiler development examples online, and knew that C/C++ dealt very closely with lower-level aspects such as controlling memory allocation on both the stack and heap memory and lacked complicated library callings (`printf` instead of `System.out.println`). C, as a compiled language unlike Java or JS, runs more efficiently and therefore faster - so if I were to write the compiler in C, compile-times of MinC projects will be relatively short - this only matters in large projects and I'm not very sure why someone would write large projects in MinC. 
### Week 1 - Initial Development
Basically, a compiler is a text converter. It is fed text as input (for example, a `.c` file) and outputs text as a `.s` assembly or an `.exe` executable. I am familiar with GNU's Compiler Collection (GCC) - it runs on terminal or command prompt or whatever text-based shell - sans GUI - so MinC will be done pretty much the same way. A compiler doesn't require dealing with fancy I/O, rendering or any other external libraries or APIs or SDKs - it is a very clean and lightweight project and saves time on learning APIs and debugging them. For example, I had experienced horrible memory leaking with [libPNG](http://www.libpng.org/pub/png/libpng.html) when implementing texture loading for my OpenGL renderer which tooks days to debug.\
\
Day one was configuring the only I/O I needed, which was to read a file and write one, and begin writing a preprocessor for my C code.

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
Thank you to these sources for information, almost all significant code is cited here.
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
