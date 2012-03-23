#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Simulation.h"
#include "linenoise.h"

#define DEBUG_INFORMATION
#define CMD_REGISTER_SIZE 100

enum {PICOL_OK, PICOL_ERR, PICOL_RETURN, PICOL_BREAK, PICOL_CONTINUE};
enum {PT_ESC,PT_STR,PT_CMD,PT_VAR,PT_SEP,PT_EOL,PT_EOF};
struct command
{
    int len;
    char *str[CMD_REGISTER_SIZE];
};

struct picolParser {
    char *text;
    char *p; /* current text position */
    int len; /* remaining length */
    char *start; /* token start */
    char *end; /* token end */
    int type; /* token type, PT_... */
    int insidequote; /* True if inside " " */
};
/* 单向链表存放变量名和值，值可以是任何类型的，因为只是存储指针*/
struct picolVar {
    char *name, *val;
    struct picolVar *next;
};

struct picolInterp; /* forward declaration */
typedef int (*picolCmdFunc)(struct picolInterp *i, int argc, char **argv, void *privdata);
/* 单向链表，存放命令，命令的名字，具体实现的函数指针，留有的数据接口*/
struct picolCmd {
    char *name;
    picolCmdFunc func;
    void *privdata;
    struct picolCmd *next;
};
/* 函数调用时,parent保存函数体外的变量链表的表头指针,vars保存函数体内定义的变量的链表*/
struct picolCallFrame {
    struct picolVar *vars;
    struct picolCallFrame *parent; /* parent is NULL at top level */
};

struct picolInterp {
    int level; /* Level of nesting */
    struct picolCallFrame *callframe;
    struct picolCmd *commands;
    char *result;  // 记录一些出错信息
};

/* declaration*/
extern volatile void *gl_addBase;

void picolInitParser(struct picolParser *p, char *text);
int picolParseSep(struct picolParser *p);
int picolParseEol(struct picolParser *p);
int picolParseCommand(struct picolParser *p);
/*识别出变量*/
int picolParseVar(struct picolParser *p);
/* 识别出花括弧*/
int picolParseBrace(struct picolParser *p);
/* 识别出字符串*/
int picolParseString(struct picolParser *p);
/* 识别命令注释*/
int picolParseComment(struct picolParser *p);
/* 通过判断第一个字符来继续识别词，识别出后标记type */
int picolGetToken(struct picolParser *p);
void picolInitInterp(struct picolInterp *i);
void picolSetResult(struct picolInterp *i, char *s);    
/* 通过查找变量的名字来寻找变量，返回变量结构体指针*/
struct picolVar *picolGetVar(struct picolInterp *i, char *name);
/* 先查找变量（GetVar），若找到则复制val给它，若否则新开辟空间，存储变量及其值val*/
int picolSetVar(struct picolInterp *i, char *name, char *val);
/* 从链表中通过命令的名字查找命令，返回命令结构体指针*/
struct picolCmd *picolGetCommand(struct picolInterp *i, char *name);
/* 把命令的名字，实现函数的函数指针，数据存储到picolInterp链表中*/
int picolRegisterCommand(struct picolInterp *i, char *name, picolCmdFunc f, void *privdata);
/* 通过picolParser中的start和end来分段词，复制出词后，通过判断type，对取得的词识别进行操作*/
/* EVAL! */
int picolEval(struct picolInterp *i, char *t);
/* ACTUAL COMMANDS! */
int picolArityErr(struct picolInterp *i, char *name);
int picolCommandMath(struct picolInterp *i, int argc, char **argv, void *pd);
/* set命令（定义变量）实现函数 */
int picolCommandSet(struct picolInterp *interp, int argc, char **argv, void *pd) ;
int picolCommandGet(struct picolInterp *i, int argc, char **argv, void *pd) ;
int picolCommandPuts(struct picolInterp *i, int argc, char **argv, void *pd);
int picolCommandIf(struct picolInterp *i, int argc, char **argv, void *pd);
int picolCommandWhile(struct picolInterp *i, int argc, char **argv, void *pd);
int picolCommandRetCodes(struct picolInterp *i, int argc, char **argv, void *pd);
void picolDropCallFrame(struct picolInterp *i);
int picolCommandCallProc(struct picolInterp *i, int argc, char **argv, void *pd);
int picolCommandProc(struct picolInterp *i, int argc, char **argv, void *pd);
int picolCommandReturn(struct picolInterp *i, int argc, char **argv, void *pd);
void picolRegisterCoreCommands(struct picolInterp *i);

#endif
