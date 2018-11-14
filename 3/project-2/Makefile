# To run, enter
# make all

all: diningphilosophers

diningphilosophers: main.o dining.o eating.o thinking.o philosopher.o
	gcc -pthread -o diningphilosophers main.o dining.o thinking.o eating.o philosopher.o

main.o: main.c dp.h
	gcc -pthread -c main.c

dining.o: dining.c dp.h
	gcc -pthread -c dining.c

philosopher.o: philosopher.c dp.h
	gcc -pthread -c philosopher.c

eating.o: eating.c
	gcc -pthread -c eating.c

thinking.o: thinking.c
	gcc -pthread -c thinking.c

