#include <stdio.h>
#include <stdlib.h>
#include "IO.c"

char* dSuf = "Bit-C Compiler: ";

int main (int argc, char* argv[])
{
    printf("%sChaidhat Chaimongkol\n", dSuf);
    IOread("/Users/chaidhatchaimongkol/Documents/GitHub/Bit-C-Compiler/README.md");  
    printf("%sTerminated\n", dSuf);
}
