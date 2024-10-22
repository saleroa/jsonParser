parseTest: parse.c parse_test.c
	gcc -g -o parseTest parse.c  parse_test.c

printTest: print.c print_test.c
	gcc -g -o printTest print.c print_test.c parse.c

clean:
	rm *.o  parseTest printTest