#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bcc.h"

void preprocess ()
{
    IoLog("preprocess");

    char line[128];
    int i = 0;
    while (i < DB_SIZE)
    {
        if (dataBuffer[i] == '#')
        {
            int j = i;
            while (dataBuffer[j] != '\n')
            {
                j++;
                line[j - i] = dataBuffer[j];
            }
            line[j - i] = '\0';
            printf("%s\n",line);
        }
        i++;
    }
}
