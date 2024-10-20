test: parse.o test.o
	gcc -o test parse.o test.o

clean:
	rm *.o test