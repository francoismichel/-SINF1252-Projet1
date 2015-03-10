all: jeu test test_intermediate

jeu: dames.c dames.h programme.c
	gcc -c dames.c -Wall -Werror -g
	gcc -o programme programme.c dames.o -Wall -Werror -g
	
test: dames.c dames.h test.c
	gcc -c dames.c -Wall -Werror -g
	gcc -o test test.c dames.o -lcunit -Wall -Werror -g

test_intermediate: dames.c dames.h dames_intermediate.h test_intermediate.c
	gcc -c dames.c -Wall -Werror -g
	gcc -o test_intermediate test_intermediate.c dames.o -lcunit -Wall -Werror -g

clean:
	rm -rf *.o

mrproper: clean
	rm -rf programme
	rm -rf test
