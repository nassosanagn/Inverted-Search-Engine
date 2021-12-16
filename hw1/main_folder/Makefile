CC = g++

main: BKTree.o main.o entry.o payload.o
	$(CC) -o main main.o BKTree.o entry.o payload.o

main.o: main.cpp entry.cpp BKTree.cpp entry.h BKTree.h
	$(CC) -c main.cpp

entry.o: entry.cpp entry.h core.h ../../hw2/payload.cpp ../../hw2/payload.h
	$(CC) -c entry.cpp

BKTree.o: BKTree.cpp ../../hw2/payload.cpp ../../hw2/payload.h BKTree.h entry.cpp entry.h core.h 
	$(CC) -c BKTree.cpp

payload.o: ../../hw2/payload.cpp core.h
	$(CC) -c ../../hw2/payload.cpp

clean: 
	$(RM) count *.o *~ main test

test: test.o entry.o BKTree.o
	$(CC) -o test BKTree.o test.o entry.o
	./test

test.o: test.cpp BKTree.cpp BKTree.h entry.cpp entry.h core.h 
	$(CC) -c test.cpp 

.PHONY: test