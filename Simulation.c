/* *********************************************************************
 * Filename: Simulation.c
 * Description: access the PC's memory to simulate accessing IO on 
 *              hardware
 * Created: 2012/02/15 by Zhu Jinliang
 * Modification: 2012/02/16 by Zhu Jinliang
 * ********************************************************************/
#include "includes.h"

/* Read 8 bit data from address*/
uint8 Read8(uint8 offset)
{
    return (*((uint8 *)(gl_addBase + offset))); 
}
/* Write 8 bit data to address*/
void Write8(uint8 offset, uint8 data)
{
    *((uint8 *)(gl_addBase + offset)) = data;
}

/* Read 16 bit data from address*/
uint16 Read16(uint8 offset)
{
    return (*((uint16 *)(gl_addBase + offset))); 
}
/* Write 16 bit data to address*/
void Write16(uint8 offset, uint16 data)
{
    *((uint16 *)(gl_addBase + offset)) = data;
}

/* Read 32 bit data from address*/
uint32 Read32(uint8 offset)
{
    return (*((uint32 *)(gl_addBase + offset))); 
}
/* Write 32 bit data to address*/
void Write32(uint8 offset, uint32 data)
{
    *((uint32 *)(gl_addBase + offset)) = data;
}
