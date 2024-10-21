parseTest: parse.o parse_test.o
	gcc -o parseTest parse.o  parse_test.o

printTest: print.o print_test.o
	gcc -o printTest print.o print_test.o


clean:
	rm *.o  parseTest printTest test