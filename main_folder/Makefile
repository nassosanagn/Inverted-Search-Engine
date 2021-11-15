CC = g++

default: BKTree

BKTree:  BKTree.o entry.o 
	$(CC) -o main BKTree.o entry.o 

entry.o: entry.cpp entry.h core.h 
	$(CC) -c entry.cpp

BKTree.o: BKTree.cpp BKTree.h entry.cpp entry.h core.h 
	$(CC) -c BKTree.cpp

clean: 
	$(RM) count *.o *~ main test

test: test.o entry.o
	$(CC) -o test test.o entry.o
	./test

test.o: test.cpp BKTree.cpp BKTree.h entry.cpp entry.h core.h 
	$(CC) -c test.cpp

.PHONY: test