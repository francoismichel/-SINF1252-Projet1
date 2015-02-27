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
				if(i < 4){
					*(*((jeu -> board) + i) + j) = 0x1;
				} else if (i < 6){
					*(*((jeu -> board) + i) + j) = 0x0;
				} else {
					*(*((jeu -> board) + i) + j) = 0x5;
				}
			}
		}	
	}
	return jeu;
}

struct game *load_game(int xsize, int ysize, const int **board, int cur_player){
	struct game *jeu = (struct game *) malloc(sizeof(struct game));  
	jeu -> xsize = xsize;  	//on initialise toutes les variables de la structure.
	jeu -> ysize = ysize;
	jeu -> board = (int **) board; 	//Pas besoin de creer un tableau, on nous donne un pointeur qui pointe deja vers les bonnes valeurs.
	jeu -> cur_player = cur_player; 
	jeu -> moves = (struct move *) malloc(sizeof(struct move));   // peut-etre faire un calloc plutot que malloc, par souci de proprete ?
	return jeu;															  // vu qu'on initialise aucune variable de moves dans cette fonction...
}



/*
 * is_move_seq_valid
 * Vérifier si une séquence d'un mouvement est valide. La fonction ne MODIFIE PAS l'état du jeu !
 * 
 * @game: pointeur vers la structure du jeu
 * @seq: séquence du mouvement à vérifier
 * @prev: séquence précédent immédiatement la séquence à vérifier, NULL s'il @seq est la première séquence du mouvement
 * @taken: pointeur vers des coordonnées qui seront définies aux coordonnées de la pièce prise s'il en existe une
 * @return: 0 si mouvement invalide, 1 si déplacement uniquement, 2 si capture
 */
int is_move_seq_valid(const struct game *game, const struct move_seq *seq, const struct move_seq *prev, struct coord *taken){
	return 0;
}

/*
 * print_board
 * Affiche l'état du jeu sur le terminal
 *
 * @game: pointeur vers la structure du jeu
 */
void print_board(const struct game *game){
	int i,j;	
	for(i = 0 ; i < 10 ; i++){
		for(j = 0 ; j < 10 ; j++){
			printf("%d ", (game -> board)[i][j]);
		}
		printf("\n");
	}
	if(game -> cur_player == PLAYER_BLACK){
		printf("C'est au joueur noir de jouer.\n");
	}
	else if(game -> cur_player == PLAYER_WHITE){
		printf("C'est au joueur blanc de jouer.\n");
	}
	else{
		printf("Erreur : impossible de déterminer le joueur qui doit jouer...\n");
	}
}


void free_move_seq(move_seq * seq){
	move_seq *precedent = seq
	while(seq != NULL){
		seq = seq -> next;
		free(precedent -> c_old);
		free(precedent -> c_new);
		free(precedent -> piece_taken);
		free(precedent);
		precedent = seq;
	}	
}

/*
* free_game
* Libérer les ressources allouées à un jeu
*
* @game: pointeur vers la structure du jeu
*/

//pas verifie... pas fini
void free_game(struct game *game){
	int i;
	int j;
	for(i = 0 ; i < 10 ; i++){
		free(*((game -> board) + i));
	}
	struct move *precedent = game -> moves;
	while(game -> moves != NULL){
		game -> moves = (game -> moves) -> next;
		free_move_seq(precedent -> seq):
		free(precedent);
		precedent = games -> moves;
	}
}














