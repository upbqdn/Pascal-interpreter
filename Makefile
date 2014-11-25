NAME=main
OBJFILES=$(NAME).o scanner.o stack.o parser.o whattoken.o

CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -W -g

# univerzální pravidlo pro generování všech objektových souborů
%.o : %.c
	$(CC) $(CFLAGS) -c $<

# Startovací pravidlo
all: $(NAME)

# Slinkování všech objektových souborů do jednoho spustitelného programu.
$(NAME): $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) -o $@

clean:
	rm -f *.o *.out $(PROGS)
