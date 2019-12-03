ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

objects = main.o

all: $(objects)
	$(CC) -o program $(objects)

main.o: main.c
	$(CC) -c main.c

leak:
	valgrind --leak-check=yes ./program

run:
	./program

clean:
	rm *.o