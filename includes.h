/* ************************************************************************
 *    Filename: includes.h
 *  Programmer: ZhuJinliang
 * Description:  
 *     Version:  2.0
 *     Created:  2012/03
 *    Function: 
 *    Modified by ZhuJinliang 2012-03-28
 * ************************************************************************/
#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#include "picol.h"
#include "Simulation.h"
#include "linenoise.h"

#define DEBUG_INFORMATION
#define CMD_REGISTER_SIZE 100


struct command
{
    int len;
    char *str[CMD_REGISTER_SIZE];
};


/* global variable declaration*/
extern volatile void *gl_addBase;
extern volatile int gl_tabFlag;
extern struct command cmd;


#endif
