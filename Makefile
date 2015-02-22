test2: dames.c dames.h test2.c
	gcc -c dames.c
	gcc -o test2 test2.c dames.o
