#include <stdio.h>
#include <stdlib.h>
#include "io.c"

char* dSuf = "Bit-C Compiler: ";

int main (int argc, char* argv[])
{
    printf("%sChaidhat Chaimongkol\n", dSuf);
    IOread("/Users/chaidhatchaimongkol/Documents/GitHub/Bit-C-Compiler/README.md");
    char *data = getDataBuffer(); 
    data[2] = 'v';
    setDataBuffer(data);
    IOwrite("/Users/chaidhatchaimongkol/Documents/GitHub/Bit-C-Compiler/README.md");
    printf("%sTerminated\n", dSuf);
}
