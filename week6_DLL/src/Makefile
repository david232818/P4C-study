# Makefile

CC = gcc
EXENAME = dllmain
SOURCES := $(wildcard *.c)
OBJS = ${SOURCES:.c=.o}

all: ${OBJS}
	$(CC) -g $^ -o $(EXENAME)

.c.o:
	$(CC) -g -c -I ../include/ $^

clean:
	rm -f ${OBJS} ${EXENAME}
