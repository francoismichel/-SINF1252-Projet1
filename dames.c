#include <stdlib.h>
#include <stdio.h>
#include "dames.h"

struct game *new_game(int xsize, int ysize){
	struct game *jeu = (struct game *) malloc(sizeof(struct game *));//On alloue de la memoire pour creer la structure du jeu
	if(jeu == NULL){  //Si le maloc a echoue
		printf("Memoire insuffisante pour creer le jeu.\n");
		exit(EXIT_FAILURE);
	}
	int i;
	for(i = 0 ; i < xsize ; i++)
		*((jeu -> board) + i) = (int *) malloc(ysize*sizeof(int *)); //On alloue de la memoire pour le plateau de jeu.
		if((jeu -> board) + i) == NULL){ //si le malloc a echoue
			printf("Memoire insuffisante pour creer le plateau de jeu.\n");
			exit(EXIT_FAILURE);
		}
	}
	jeu -> xsize = xsize;
	jeu -> ysize = ysize;
	*(jeu -> moves) = (struct move *) malloc(sizeof(struct move *)); //On alloue de la memoire pour stocker la liste de coups
	if(jeu ->moves == NULL){
		printf("Memoire insuffisante pour generer la move list.\n"); 
		exit(EXIT_FAILURE);
	}
	jeu -> cur_player = PLAYER_WHITE;
	int j;
	//POUR L'INSTANT ES PIONS SONT PAS MIS UNE CASE SUR DEUX, A CORRIGER, LA J'AI PAS LE TEMPS DUCON
	for(i = 0 ; i < xsize ; i++){ //On initialise le plateau (les pions noirs en haut, les blancs en bas.)
		for(j = 0 ; j < ysize ; j++){
			if(j < 4){
				*(*((jeu -> board) + i) + j) = 0x1;	//001 en binaire (pion noir)
			} else if (j < 6){
				*(*((jeu -> board) + i) + j) = 0x0;	//000 en binaire (pas de pion)
			} else {
				*(*((jeu -> board) + i) + j) = 0x5;	//101 en binaire (pion blanc)
			}
		}	
	}
	return jeu;
}
