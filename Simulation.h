#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include "includes.h"


typedef unsigned char     uint8;
typedef signed char       int8;
typedef unsigned int      uint16;
typedef signed int        int16;
typedef unsigned long int uint32;
typedef signed long int   int32;

/* Funtion Declaration*/
uint8 Read8(uint8 offset);
void Write8(uint8 offset, uint8 data);
uint16 Read16(uint8 offset);
void Write16(uint8 offset, uint16 data);
uint32 Read32(uint8 offset);
void Write32(uint8 offset, uint32 data);

#endif
