/* ************************************************************************
 *    Filename:  tclInterp.c
 *  Programmer: ZhuJinliang
 * Description:  
 *     Version:  2.0
 *     Created:  2012/03
 *    Function: 
 *    Modified by ZhuJinliang 2012-03-28
 * ************************************************************************/

#include "includes.h"

#ifdef DEBUG_INFORMATION
#define Debugf printf
#else
void Debugf(char *format, ...)
{
}
#endif

volatile void *gl_addBase = NULL;


int HardwareInitial(void)
{
    gl_addBase = malloc(300);
    if (!gl_addBase)
    {
        printf("memory alloction failed!");
        return 0;
    }
    return 1;
}
void Completion(const char *buf, linenoiseCompletions *lc)
{
    int i = 0;
    if (buf[0] == 'p')
    {
        linenoiseAddCompletion(lc, "proc");
        linenoiseAddCompletion(lc, "puts");
    }
    else if (buf[0] == 'w')
    {
        linenoiseAddCompletion(lc, "while");
        linenoiseAddCompletion(lc, "w8");
    }
    else if (buf[0] == 'r')
    {
        linenoiseAddCompletion(lc, "r8");
    }
    else if (buf[0] == '\0')
    {
        gl_tabFlag = 1;
        printf("\n");
        for (i=0; i<cmd.len; i++)
        {
            printf("\r%s\n", cmd.str[i]);
        }
    }
}
char *GetDirectory(void)
{
    char *home;
    char *dir;

    home = getenv("HOME");
    dir = (char*)malloc((strlen(home) + 20));
    strcpy(dir, home);
    strcat(dir, "/");
    strcat(dir, "history.txt");
    
    return dir;
}

int main(int argc, char **argv) 
{
    struct picolInterp interp;
    FILE *fp;
    char buf[1024*16];
    int retcode;
    char *line;
    char *dir;

    if (!HardwareInitial()) return 0;
    picolInitInterp(&interp);
    picolRegisterCoreCommands(&interp);
    dir = GetDirectory();

    linenoiseSetCompletionCallback(Completion);
    if (linenoiseHistoryLoad(dir)) Debugf("history.txt didn't exist!"); /* Load the history at startup*/

    switch (getopt(argc, argv, "s:i:"))
    {
        case 's':
            fp = fopen(optarg,"r");
            if (!fp) 
            {
                perror("open"); exit(1);
            }
            buf[fread(buf,1,1024*16,fp)] = '\0';
            fclose(fp);
            if (picolEval(&interp,buf) != PICOL_OK) printf("%s\n", interp.result);
            break;
            
        case 'i':
            fp = fopen(optarg,"r");
            if (!fp) 
            {
                perror("open"); exit(1);
            }
            buf[fread(buf,1,1024*16,fp)] = '\0';
            fclose(fp);
            if (picolEval(&interp,buf) != PICOL_OK) printf("%s\n", interp.result);
            while((line = linenoise("picol>")) !=NULL)
            {
                if (line[0] != '\0')
                {
                    linenoiseHistoryAdd(line);
                    if (linenoiseHistorySave(dir)) Debugf("save failed!\n");
                }
                if (gl_tabFlag == 1)
                {
                    gl_tabFlag = 0;
                    continue;
                }
                if (!strcmp(line, "exit")) 
                {
                    printf("exit success!\n");
                    break;
                }
                retcode = picolEval(&interp,line);
                if (interp.result[0] != '\0')
                printf("[%d] %s\n", retcode, interp.result);
                free(line);
            }
            break;

        default:
            while((line = linenoise("picol>")) !=NULL)
            {
                if (line[0] != '\0')
                {
                    linenoiseHistoryAdd(line);
                    if (linenoiseHistorySave(dir)) Debugf("save failed!\n");
                }
                if (gl_tabFlag == 1)
                {
                    gl_tabFlag = 0;
                    continue;
                }
                if (!strcmp(line, "exit")) 
                {
                    printf("exit success!\n");
                    break;
                }
                retcode = picolEval(&interp,line);
                if (interp.result[0] != '\0')
                printf("[%d] %s\n", retcode, interp.result);
                free(line);
            }
            break;
    }
    free(dir);
    free((void*)gl_addBase);
    return 0;
}
