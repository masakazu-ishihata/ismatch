# Makefile
TAR = test
CC = gcc -O6 -Wall
L = -lm -liutil -licsv -lilist -lihash
OBJS = main.o ismatch.o

all : $(TAR)
$(TAR): $(OBJS)
	  $(CC) -o $@ $(OBJS) $(L)
clean:
	  rm -f $(OBJS) $(LIB) $(TAR) *~ *.bak

.c.o:	  $(CC) -c $<om

main.o: main.h ismatch.o
ismatch.o: ismatch.h
