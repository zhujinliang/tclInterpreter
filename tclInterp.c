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
void CmpStr(const char *buf, char *str, int *p)
{
    int i =0;

    while ((buf[i] != '\0') && (str[i] != '\0'))
    {
        if (buf[i] == str[i])
        {
            (*p) ++;
        }
        else
        {
            break;
        }
        i ++;
    }
  
}

void Completion(const char *buf, linenoiseCompletions *lc)
{
    int i = 0;
    int same[100];
    int temp = 0;
    int index = 0;

    for (i=0; i<cmd.len; i++)
    {
        same[i] = 0;
    }
    for (i=0; i<cmd.len; i++)
    {
        CmpStr(buf, cmd.str[i], &same[i]);
    }
    for (i=0; i<cmd.len; i++)
    {
        if (temp < same[i])
        {
            temp = same[i];
        }
    }
    if (temp != 0)
    {
        for (i=0; i<cmd.len; i++)
        {
            if (same[i] == temp)
            {
                linenoiseAddCompletion(lc, cmd.str[i]);
            }
        }
    }
    else  
    {
        if (buf[0] != '\0')
        {
            printf("\r\nCan't find the match command!");
        }
        gl_tabFlag = 1;
        printf("\n\rThe command you can use are:\n");
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

        case '?':
            printf("Illegal parameter!\n");
            printf("Every time you can only use one parameter!\n");
            printf("Format: tcl              : default mode: interactive mode\n"
                   "        tcl -s <tcl file>: script mode\n"
                   "        tcl -i <tcl file>: interactive and script mode, you can run a script at first, then go to interactive mode\n"
                  );
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
