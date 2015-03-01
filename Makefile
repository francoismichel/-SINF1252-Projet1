test2: dames.c dames.h test.c
	gcc -c dames.c
	gcc -o test testt.c dames.o
