#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bcc.h"

int main (int argc, char* argv[])
{
    strcpy(outFilepath, "$");
    if (IoInp(argc,argv) == 0)
        return 0;
   IoLog("reading from %s", inFilepath);
    IoLog("writing to %s", outFilepath);
    IoLog("with mode %d", mode);
    IoLog("Chaidhat Chaimongkol on %s %s", __DATE__, __TIME__);
    IoRead(inFilepath);
    // do code
    //dataBuffer[2] = 'B';
    IoWrite(outFilepath);
    printf("\n");
    IoLog("Terminated");
    return 0;
}

