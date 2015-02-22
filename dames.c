#include <stdlib.h>
#include <stdio.h>
#include "dames.h"

struct game *new_game(int xsize, int ysize){
	struct game *jeu = (struct game *) malloc(sizeof(struct game));//On alloue de la memoire pour creer la structure du jeu
	if(jeu == NULL){  //Si le malloc a echoue
		printf("Memoire insuffisante pour creer le jeu.\n");
		exit(EXIT_FAILURE);
	}
	int i;
	jeu -> board  = (int **) malloc(xsize*sizeof(int *));
	for(i = 0 ; i < xsize ; i++){
		*((jeu -> board) + i) = (int *) malloc(ysize*sizeof(int)); //On alloue de la memoire pour le plateau de jeu.
		if((jeu -> board) + i == NULL){ //si le malloc a echoue
			printf("Memoire insuffisante pour creer le plateau de jeu.\n");
			exit(EXIT_FAILURE);
		}
	}
	jeu -> xsize = xsize;
	jeu -> ysize = ysize;
	jeu -> moves = (struct move *) malloc(sizeof(struct move)); //On alloue de la memoire pour stocker la liste de coups
	if(jeu -> moves == NULL){
		printf("Memoire insuffisante pour generer la move list.\n"); 
		exit(EXIT_FAILURE);
	}
	jeu -> cur_player = PLAYER_WHITE;
	int j;
	for(i = 0 ; i < xsize ; i++){
		for(j = 0 ; j < ysize ; j++){
			if( ( (j % 2 == 0) && (i % 2 == 0) ) || ( (j % 2 != 0) && (i % 2 != 0) ) ){
				*(*((jeu -> board) + i) + j) = 0x0;
			}
			else{
				if(j < 4){
					*(*((jeu -> board) + i) + j) = 0x1;
				} else if (j < 6){
					*(*((jeu -> board) + i) + j) = 0x0;
				} else {
					*(*((jeu -> board) + i) + j) = 0x5;
				}
			}
		}	
	}
	return jeu;
}
