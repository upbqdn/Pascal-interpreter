NAME=main
OBJFILES=$(NAME).o scanner.o stack.o parserito.o

CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -W -g

# univerzální pravidlo pro generování všech objektových souborů
%.o : %.c
	$(CC) $(CFLAGS) -c $<

# Startovací pravidlo
all: $(NAME)

## ## ## 
# pravidla bez těla - to se sem doplní z univerzálního pravidla
#modul1.o: modul1.c modul1.h
#modul2.o: modul2.c modul1.h modul2.h
#modul3.o: modul3.c modul3.h
#program.o: program.c modul1.h modul2.h modul3.h
## ## ## 

# Slinkování všech objektových souborů do jednoho spustitelného programu.
$(NAME): $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) -o $@

clean:
	rm -f *.o *.out $(PROGS)