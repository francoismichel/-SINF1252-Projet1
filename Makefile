test: dames.c dames.h test.c
	gcc -c dames.c -Wall -Werror
	gcc -o test test.c dames.o -lcunit -Wall -Werror
