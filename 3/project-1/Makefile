# To run, enter
# make all

all: sleepingta

sleepingta: main.o student.o ta.o hangout.o help_student.o
	gcc -pthread -o sleepingta main.o student.o ta.o hangout.o help_student.o -std=gnu99 -lrt

main.o: main.c ta.h
	gcc -pthread -c main.c -std=gnu99 -lrt

student.o: student.c ta.h
	gcc -pthread -c student.c -std=gnu99 -lrt

ta.o: ta.c ta.h
	gcc -pthread -c ta.c -std=gnu99 -lrt

hangout.o: hangout.c
	gcc -c hangout.c -std=gnu99 -lrt

help_student.o: help_student.c
	gcc -c help_student.c -std=gnu99 -lrt

