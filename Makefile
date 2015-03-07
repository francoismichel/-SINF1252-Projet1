all: programme test

programme: dames.c dames.h programme.c
	gcc -c dames.c -Wall -Werror
	gcc -o programme programme.c dames.o -Wall -Werror
	
test: dames.c dames.h test.c
	gcc -c dames.c -Wall -Werror
	gcc -o test test.c dames.o -lcunit -Wall -Werror

clean:
	rm -rf *.o

mrproper: clean
	rm -rf programme
	rm -rf test
