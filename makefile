ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

objects = main.o shell.o parsing.o

all: $(objects)
	$(CC) -o program $(objects)

main.o: main.c shell.c shell.h
	$(CC) -c main.c 

shell.o: shell.c shell.h parsing.c parsing.h
	$(CC) -c shell.c

parsing.o: parsing.c parsing.h
	$(CC) -c parsing.c

leak:
	valgrind --leak-check=yes ./program

run:
	./program

clean:
	rm *.o