test: glob.o test.o
	cc -o test glob.o test.o

test.o: test.c
	cc -c -Wall test.c

glob.o: glob.c
	cc -c -Wall glob.c
