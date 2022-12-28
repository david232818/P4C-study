# Makefile

CC = gcc
EXENAME = dlltest
SOURCES := $(wildcard *.c)
OBJS = ${SOURCES:.c=.o}

all: ${OBJS}
	$(CC) -g $^ -o $(EXENAME)

.c.o:
	$(CC) -std=c99 -O2 -c -I ../include/ $^

clean:
	rm -f ${OBJS} ${EXENAME}
