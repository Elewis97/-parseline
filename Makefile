Target = parseline
CC = gcc
CFLAGS = -pedantic -std=gnu99 -Wall -g
MAIN = parseline
OBJS = parseline.o

all : $(MAIN)

$(main) : $(OBJS)
	$(CC) -o $(MAIN) $(CFLAGS) parseline.c

parseline.o : parseline.c
	$(CC) $(CFLAGS) -c parseline.c

clean :
	rm *.o $(MAIN) core
