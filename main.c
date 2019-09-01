#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bcc.h"

int main (int argc, char* argv[])
{
    IoInp(argc,argv);
    IoLog("Chaidhat Chaimongkol on %s %s", __DATE__, __TIME__);
    IoLog("reading from %s", inFilepath);
    IoLog("writing to %s", outFilepath);
    IoLog("with mode %d", mode);
    IoRead(inFilepath);
    // do code
    //dataBuffer[2] = 'B';
    IoWrite(outFilepath);
    printf("\n");
    IoExit(0, __LINE__);
}


