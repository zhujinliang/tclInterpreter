CC = gcc

LIBS = -lm
TARGET = tcl
SRCS = picol.c Simulation.c linenoise.c 
OBJS := $(SRCS:%.c=%.o)

all :
	$(CC) -o $(TARGET) $(SRCS) $(LIBS)

clean :
	rm -f $(OBJS) $(TARGET) *~ 

picol.o:picol.c includes.h
Simulation.o:Simulation.c includes.h
linenoise.o:linenoise.c linenoise.h
