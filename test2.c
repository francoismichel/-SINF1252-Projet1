#include <stdlib.h>
#include <stdio.h>
#include "dames.h"

void test_newgame(){
	struct game *jeu = new_game(10,10);
	if(jeu == NULL){
		printf("Test échoué : jeu NULL. \n");
		exit(EXIT_FAILURE);
	}
	if(jeu -> board == NULL){
		printf("Test échoué : board NULL. \n");
		exit(EXIT_FAILURE);
	}
	int i;
	int j;
	for(i = 0 ; i < 10 ; i++){
		for(j = 0 ; j < 10 ; j++){
			if(!( *(*((jeu -> board) + i) + j) == 0x1 || *(*((jeu -> board) + i) + j) == 0x5 || *(*((jeu -> board) + i) + j) == 0x0) ){ //Si le damier n'est pas composé que de 0, 1 ou 5)
				printf("Test échoué : damier mal initialisé. \n");
				exit(EXIT_FAILURE);
			}
		}
	}
	if(jeu -> moves == NULL){
		printf("Test échoué : movelist mal initialisée\n");
		exit(EXIT_FAILURE);
	}
	if(jeu -> cur_player != PLAYER_WHITE){
		printf("Test échoué : Le joueur blanc ne commence pas le jeu.\n");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char *argv[]){
	test_newgame();
	printf("Test reussi !\n");
}
