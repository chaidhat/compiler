#include <stdio.h>
#include <stdlib.h>

char* dSuf = "Bit-C Compiler: ";
char* data[128];
FILE* fp;
char* ch;

// reads files
void IOread (char *filename)
{
    fp = fopen(filename, "r"); // read mode

   if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }

   printf("The contents of %s file are:\n", filename);

   while((ch = fgetc(fp)) != EOF)
      printf("%c", ch);

   fclose(fp);
}

int main (int argc, char* argv[])
{
    printf("%sChaidhat Chaimongkol\n", dSuf);
    IOread("/Users/chaidhatchaimongkol/Documents/GitHub/Bit-C-Compiler/README.md");  
    printf("%sTerminated\n", dSuf);
}
