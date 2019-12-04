ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

objects = main.o shell.o

all: $(objects)
	$(CC) -o program $(objects)

main.o: main.c shell.c shell.h
	$(CC) -c main.c 

shell.o: shell.c shell.h
	$(CC) -c shell.c

leak:
	valgrind --leak-check=yes ./program

run:
	./program

clean:
	rm *.o