#include "btcc.h"

static void setDefaults ()
{
    strcpy(inFilepath, "main.btc");
    strcpy(outFilepath, "$");
    mode = 1; // default mode (compiler debug)s
    doBenchmarking = false;
    doRun = false;
    doParsing = true;
    doAssemble = true;
    doLinker = true;
    doWarnings = true;
}
static void doargs (int argc, char* argv[])
{
    setDefaults ();

    if (argc > 1)
    {
        for (int line = 1; line < argc; line++)
        {
            char* args[] =
            {
                "-V",
                "-b",
                "-o",
                "-r",
                "-v",
                "-w",
                "-I",
                "-D",
                "-U",
                "-E",
                "-S",
                "-c",
                "-h",
            };
            int argNo = -1;
            for (int i = 0; i < sizeof(args)/sizeof(args[0]); i++)
            {
                if (strncmp(argv[line],args[i],strlen(args[i])) == 0)
                {
                    argNo = i;
                    break;
                }
            }
             
            switch (argNo)
            {
                case 0:
                    printf("\033[1;30mBit-C Compiler created by Chaidhat Chaimongkol\033[m\n"
                    "compiled on: %s %s\n"
                    "\n"
                    , __DATE__, __TIME__);
                    btccExit(2, __LINE__);
                    break;
                case 1:
                    doBenchmarking = true; 
                    break; 
                case 2:
                    strcpy(outFilepath, argv[line + 1]);
                    line++;
                    break;
                case 3:
                    doRun = true;
                    strcpy(doRunArgs, argv[line + 1]);
                    line++;
                    break;
                case 4:
                    mode = 0;
                    break;
                case 5:
                    doWarnings = false;
                    break;

                case 6:
                    strcat(predefPP, "#include ");
                    strcat(predefPP, argv[line + 1]);
                    line++;
                    break;
                case 7:
                    strcat(predefPP, "#define ");
                    int c = 0;
                    while (argv[line + 1][c] != '\0')
                    {
                        c++;
                        if (argv[line + 1][c] == '=')
                            argv[line + 1][c] = ' '; 
                    }
                    strcat(predefPP, argv[line + 1]);
                    line++;
                    break;
                case 8:
                    strcat(predefPP, "#undef ");
                    strcat(predefPP, argv[line + 1]);
                    line++;
                    break;
                case 9:
                    doParsing = false;
                    break;
                case 10:
                    doAssemble = false;
                    break;
                case 11:
                    doLinker = false;
                    break;
                case 12:
                    printf("\033[1;30mBit-C Compiler created by Chaidhat Chaimongkol\n%s %s\n\n\033[m", __DATE__, __TIME__);
                    printf("usage: btcc [-h] [-V] [arg1 arg2 ...] <inpath1 inpath2 ...>\n\n" 
                    "args:\n"
                    "   -V                  display version info\n"
                    "   -b                  display compilation stats once end\n"
                    "   -o <path>           write output to <path>\n"
                    "   -r <args ...>       run output\n"
                    "   -v                  verbose compiler debugger\n"
                    "   -w                  supress all warnings\n"
                    "\n"
                    "   -I <dir>            add include path <dir>\n"
                    "   -D <macro>          predefine <macro>\n"
                    "   -D <macro>[=val]    set <macro> to [val]\n"
                    "   -U <macro>          undefine <macro>\n"
                    "   -E                  stop after preproccessing\n"
                    "\n"
                    "   -S                  stop after parsing. do not asm\n"
                    "   -c                  do not link\n"
                    "\n"
                    "   -h                  display this help\n"
                    "\n"
                    );
                    btccExit(2, __LINE__);
                    break;
                default:
                    strcpy(inFilepath, argv[line]);
                    char btcName[100];
                    int i = 0;
                    int j = 0;
                    while (argv[line][i] != '.')
                        i++;
                    if (argv[line][i + 1] != 'b' || argv[line][i + 2] != 't' || argv[line][i + 3] != 'c')
                    {
                        btccErr("Unexpected file type (expected .btc)\n ./btcc -h for help");
                        btccExit(1, __LINE__);
                    }
                    while (i > 0)
                    {
                        i--;
                        j++;
                    }
                    while (j > 0)
                    {
                        j--;
                        btcName[j] = argv[line][i + j];
                    }

                    if (strncmp(outFilepath,"$",strlen("$")) == 0)
                        strcpy(outFilepath, btcName);
                        strcat(outFilepath, ".o");
                    break;
            }
        }
        btccLog("%s\n", predefPP); //log predefined preprocessor buffer
    }
    else
    {
        btccErr("no input arguments.\n ./btcc -h for help");
        btccExit(2, __LINE__);
    }
}

int main (int argc, char* argv[])
{
    doargs(argc,argv);
    btccLog("Chaidhat Chaimongkol on %s %s", __DATE__, __TIME__);
    btccLog("reading from %s", inFilepath);
    btccLog("writing to %s", outFilepath);
    btccLog("verbose debugger enabled");
    inpOpen(inFilepath);
    printf("\n");

    lexAll();
    //preprocess();
    // do code
    //dataBuffer[2] = 'B';
    inpClose();
    inpOutput(outFilepath);
    btccExit(0, __LINE__);
}


