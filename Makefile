
PRJ=main
#
PROGS=$(PRJ)
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

all: $(PROGS)

$(PRJ): $(PRJ).c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c

clean:
	rm -f *.o *.out $(PROGS)
#