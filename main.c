#include <stdio.h>
#include <stdlib.h>
#include "bcc.h"

char* dSuf = "Bit-C Compiler: ";

int main (int argc, char* argv[])
{
    printf("%sChaidhat Chaimongkol\n", dSuf);
    IOread("/Users/chaidhatchaimongkol/Documents/GitHub/Bit-C-Compiler/test/test.bc");
    // do code
    //dataBuffer[2] = 'B';
    IOwrite("/Users/chaidhatchaimongkol/Documents/GitHub/Bit-C-Compiler/test/test.o");
    printf("\n");
    printf("%sTerminated\n", dSuf);
}
