#include <stdlib.h>
#include <stdio.h>
#include "dames.h"
#define NORDOUEST 1
#define NORDEST 2
#define SUDOUEST 3
#define SUDEST 4
int nPieces[2];
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
	jeu -> cur_player = PLAYER_WHITE;
	int j;
	for(i = 0 ; i < xsize ; i++){
		for(j = 0 ; j < ysize ; j++){
			if( ( (j % 2 == 0) && (i % 2 == 0) ) || ( (j % 2 != 0) && (i % 2 != 0) ) ){
				//*(*((jeu -> board) + j) + i) = 0x0;
				(jeu -> board)[i][j] = 0x0;
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
	nPieces[PLAYER_WHITE] = 20;
	nPieces[PLAYER_BLACK] = 20;
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

int getColor(int piece){
	return (piece & 0x4) >> 2; 
}

int isNotOutOfBoard(const struct move_seq *seq){
	struct coord c_avant = seq -> c_old;
	struct coord c_apres = seq -> c_new;
	return c_avant.x >= 0 && c_avant.x < 10 && c_avant.y >= 0 && c_avant.y < 10
		&& c_apres.x >= 0 && c_apres.x < 10 && c_apres.y >= 0 && c_apres.y < 10;
}
/*
 * retourne 0 si la piece n'est pas une dame, !0 si c'en est une
 * @piece est une piece valide
 */
int isDame(int piece){
 	return (piece & 0x2) == 0x2;
}


int getDiagonal(struct coord c_avant, struct coord c_apres){
	if(c_apres.x - c_avant.x > 0 || c_apres.y - c_avant.y > 0){
		return SUDEST;
	}
	else if(c_apres.x - c_avant.x > 0 || c_apres.y - c_avant.y < 0){
		return NORDEST;
	}
	else if(c_apres.x - c_avant.x < 0 || c_apres.y - c_avant.y > 0){
		return SUDOUEST;
	}	
	else if(c_apres.x - c_avant.x < 0 || c_apres.y - c_avant.y < 0){
		return NORDOUEST;
	}
	else{
		return 0;
	}
}


/*
 * retourne 0 si la prise de la piece n'est pas valide, retourne !0 si elle l'est
 * On considere que la prise est valide si :
 * - la piece qui prend et la piece prise sont de couleurs opposees
 * - la piece qui prend a sauté au-dessus de la piece prise :
 * 		cela signifie que les coordonnees de la piece prise sont les moyennes des coordonnees de la piece qui joue,
 *		avant et apres avoir joué, si la piece qui joue est un pion.
 * @piece est une piece valide
 */
int pieceBienPrise(const struct game *jeu, struct coord *prise, struct coord c_avant, struct coord c_apres){
	int piecePrise = (jeu -> board)[prise -> x][prise -> y];
	int pieceQuiJoue = (jeu -> board)[c_avant.x][c_avant.y];
	if(getColor(piecePrise) ^ getColor(pieceQuiJoue) == 0x1){	//On verifie que la piece qui joue et la piece prise sont de couleurs differentes
		//On verifie que la piece qui joue a bien saute au-dessus de la piece prise :
		return ( (c_avant.x + c_apres.x)/2 == prise -> x ) && ( (c_avant.y + c_apres.y)/2 == prise -> y ); 
	}
	else{		//Sinon, la prise de piece n'est pas valide
		return 0;
	}
}


//retourne 0 si ce n'est pas un mouvement diagonal. Si oui, retourne la distance diagonae parcourue
int isDiagonal(struct coord c_avant, struct coord c_apres){
	int valAbs = abs(c_apres.x - c_avant.x);
	if(valAbs == abs(c_apres.y - c_avant.y)){
		return valAbs;
	}
	else{
		return 0;
	}
}

int isCorrectMovePion(const struct game *jeu, struct coord c_avant, struct coord c_apres, struct coord *taken){
	int valeurMove = isDiagonal(c_avant, c_apres);
	if(valeurMove == 0 || valeurMove > 2){
		return 0;
	}
	//Donc ici, on sait que ValeurMove vaut 1 ou 2 (prise de pion ou non)
	
	if(valeurMove == 2){
		int piecePrise;
		int diagonale = getDiagonal(c_avant, c_apres);
		if(diagonale == SUDEST){
			piecePrise = (jeu -> board)[c_avant.x + 1][c_avant.y + 1];
			if(piecePrise = 0x0){
				return 0;
			}
			taken -> x = c_avant.x + 1;
			taken -> y = c_avant.x + 1;
		}
		else if(diagonale == SUDOUEST){
			piecePrise = (jeu -> board)[c_avant.x - 1][c_avant.y + 1];
			if(piecePrise = 0x0){
				return 0;
			}
			taken -> x = c_avant.x - 1;
			taken -> y = c_avant.x + 1;
		}
		else if(diagonale == NORDEST){
			piecePrise = (jeu -> board)[c_avant.x + 1][c_avant.y - 1];
			if(piecePrise = 0x0){
				return 0;
			}
			taken -> x = c_avant.x + 1;
			taken -> y = c_avant.x - 1;
		}
		else{
			piecePrise = (jeu -> board)[c_avant.x - 1][c_avant.y - 1];
			if(piecePrise = 0x0){
				return 0;
			}
			taken -> x = c_avant.x - 1;
			taken -> y = c_avant.x - 1;
		}
		return 2;
	}
	int piece = (jeu -> board)[c_avant.x][c_avant.y];
	if(getColor(piece) == PLAYER_WHITE){	//Si la piece est un pion blanc, on regarde si son mouvement est correct (mouvement en diagonale)
	return (c_apres.x - c_avant.x == -valeurMove && c_apres.y - c_avant.y == -valeurMove) ||
		   (c_apres.x - c_avant.x == valeurMove && c_apres.y - c_avant.y == -valeurMove);
	}		
	else{//Si c'est un pion noir, meme chose, sauf que le sens de deplacement a changé
		return (c_apres.x - c_avant.x == -valeurMove && c_apres.y - c_avant.y == valeurMove) ||
			   (c_apres.x - c_avant.x == valeurMove && c_apres.y - c_avant.y == valeurMove);
	}
	
		
}

int isCorrectMoveDame(const struct game *jeu, struct coord c_avant, struct coord c_apres, struct coord *taken){
	int valeurMove = isDiagonal(c_avant, c_apres);
	if(valeurMove == 0){
		return 0;
	}
	int diagonale = getDiagonal(c_avant, c_apres);
	int prise = 0;
	int i;
	int position;
	for(i = 1 ; i <= valeurMove ; i++){
		if(diagonale == SUDEST){
			position = (jeu -> board)[c_avant.x + i][c_avant.y + i];
		}
		else if(diagonale == SUDOUEST){
			position = (jeu -> board)[c_avant.x - i][c_avant.y + i];
		}
		else if(diagonale == NORDEST){
			position = (jeu -> board)[c_avant.x + i][c_avant.y - i];
		}
		else{
			position = (jeu -> board)[c_avant.x - i][c_avant.y - i];
		}
		if(position != 0x0 && getColor(position) == getColor((jeu -> board)[c_avant.x][c_avant.y])){
			return 0;
		}
		else if(position != 0x0){
			if(prise = 0){
				prise = 1;
			}
			else{
				return 0;
			}
		}
	}
	
}



int isMoveValid(const struct game *jeu, struct coord c_avant, struct coord c_apres, int piece, struct coord *taken){
	if((jeu -> board)[c_apres.x][c_apres.y] != 0){	//On verifie si la piece atterit sur une case vide
		return 0;
	}
	if(!isDame(piece)){		//Si la piece est un pion
		return isCorrectMovePion(jeu, c_avant, c_apres, taken);
	}
	else{
		return isCorrectMoveDame(jeu, c_avant, c_apres, taken);
	}
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
	if(prev != NULL && (seq == NULL || (prev -> c_new).x != (seq -> c_old).x || (prev -> c_new).y != (seq -> c_old).y) ){
		return 0;
	}
	struct coord c_avant = seq -> c_old;
	struct coord c_apres = seq -> c_new;
	int pieceQuiJoue = (game -> board)[c_avant.x][c_avant.y];	//on recupere la valeur de la piece qui joue.
	if(getColor(pieceQuiJoue) != game -> cur_player){	//on recupere le 3eme bit de la piece qui joue et on le compare au joueur qui doit jouer
		printf("%d\n",pieceQuiJoue);
		return 0;
	}
	if(!isNotOutOfBoard(seq)){	//Si la piece avant ou apres le mouvement se trouve en dehors du tableau, c'est invalide
		return 0;	
	}	
	return isMoveValid(game, c_avant, c_apres, pieceQuiJoue, taken);
}

//ajoute une sequence dans le move en tete de pile du jeu
void push_seq(struct game *jeu, const struct move_seq *seq, struct coord *piece_taken, int old_orig, int piece_value){
	struct move_seq *newSeq;
	newSeq = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(newSeq == NULL){
		printf("Erreur d'allocation de memoire\n");
		exit(EXIT_FAILURE);	
	}
	
	newSeq -> next = (jeu -> moves) -> seq;
	newSeq -> c_old = seq -> c_old;
	newSeq -> c_new = seq -> c_new;
	newSeq -> piece_value = piece_value;
	newSeq -> old_orig = old_orig;
	if(piece_taken != NULL){
		newSeq -> piece_taken = *piece_taken;
	}
	(jeu -> moves) -> seq = newSeq;
}

/*
 * transforme un pion en dame si necessaire
 * retourne 1 si le pion a ete transforme en dame, retourne 0 sinon
 */
int transformDame(struct game *jeu, struct coord c){
	int piece = (jeu -> board)[c.x][c.y];
	if(!isDame(piece)){
		if(getColor(piece) == PLAYER_WHITE && c.y == 0){
			(jeu -> board)[c.x][c.y] = (jeu -> board)[c.x][c.y] | 0x2;
			return 1;
		}
		else if(getColor(piece) == PLAYER_BLACK && c.y == 9){
			(jeu -> board)[c.x][c.y] = (jeu -> board)[c.x][c.y] | 0x2;
			return 1;
		}
		else{
			return 0;
		}
	}
	return 0;
} 

//ajoute un move vide en tete de pile du jeu
void push_move(struct game *jeu){
	struct move *newMove = (struct move *) malloc(sizeof(struct move));
	newMove -> next = jeu -> moves;
	newMove -> seq = NULL;
	jeu -> moves = newMove;
}

struct move *pop_move(struct game *jeu){
	if(jeu == NULL || jeu -> moves == NULL){
		return NULL;
	}
	struct move *mouvement = jeu -> moves;
	jeu -> moves = (jeu -> moves) -> next;
	return mouvement;
}


/*
 * apply_moves
 * Effectuer des mouvements
 *
 * @game: pointeur vers la structure du jeu
 * @moves: liste chainée de mouvements à effectuer
 * @return: -1 si erreur (e.g. mouvement invalide), 0 si mouvements valides, 1 si jeu terminé (game->cur_player est le gagnant)
 */
int apply_moves(struct game *game, const struct move *moves){
	struct move *current = (struct move *) moves;
	struct move_seq *sequence = NULL;
	struct coord *taken = NULL;
	struct move_seq *previousSeq = NULL;
	struct coord c_avant;
	struct coord c_apres;
	int ennemy;						//Variable qui retient la couleur du pion qui se fait manger, s'il y en a un
	int previousValid = 3;			//Variable qui retient la valeur retournee par l'appel de is_move_seq_valid sur la sequence precedente
	int isValid = 3;				//Variable qui retient la valeur qui sera retournee par l'appel de is_move_seq_valid sur la sequence en cours
	int playerPiece;				//Valeur de le piece jouee lors de la sequence en cours
	int gotDame= 0;					//Determine si le joueur a recupere une dame lors de la sequence precedente; 1: oui, 0: non
	while(current != NULL){			//On parcourt la liste de moves
		push_move(game);
		sequence = current -> seq;
		while(sequence != NULL){	//On parcourt la liste de sequences du move
			if(previousValid == 1 || gotDame == 1){
				return -1;
			}
			isValid = is_move_seq_valid(game, sequence, previousSeq, taken);
			if(isValid == 0){		//Si la sequence n'est pas valide, le move n'est pas valide
				return -1;
			}
			else if(isValid == 1){
				c_avant = sequence -> c_old;
				c_apres = sequence -> c_new;
				playerPiece = (game -> board)[c_avant.x][c_avant.y];
				ennemy = 0x0;
				(game -> board)[c_apres.x][c_apres.y] = (game -> board)[c_avant.x][c_avant.y];
				(game -> board)[c_avant.x][c_avant.y] = 0x0;
			}else{
				c_avant = sequence -> c_old;
				c_apres = sequence -> c_new;
				playerPiece = (game -> board)[c_avant.x][c_avant.y];
				(game -> board)[c_apres.x][c_apres.y] = (game -> board)[c_avant.x][c_avant.y];
				(game -> board)[c_avant.x][c_avant.y] = 0x0;
				ennemy = (game -> board)[taken -> x][taken -> y];
				(game -> board)[taken -> x][taken -> y] = 0x0;
				nPieces[getColor(ennemy)]--;
				if(nPieces[getColor(ennemy)] == 0){
					return 1;
				}
			}
			push_seq(game, sequence, taken, playerPiece, ennemy);
			previousSeq = sequence;
			gotDame = transformDame(game, c_apres);		//on recupere une dame si necessaire et on recupere le resultat
			sequence = sequence -> next;
		}
		//creer une fonction pour voir si le joueur peut encore jouer apres ou non (moves encore possibles,...)
		current = current -> next;
		previousValid = 3;		//Comme on change de move, ce n'est plus le meme joueur qui joue, donc on reinitiaise previousValid.
		game -> cur_player = ~(game -> cur_player) & 1;	//On inverse le joueur qui joue
	}
}

/*
 * applique retroactivement la sequence sur le jeu. Retourne -1 si jeu ou sequence est NULL, retourne 0 si tout s'est bien passe.
 */
int undo_seq(struct game *jeu, struct move_seq *sequence){
	if(sequence == NULL || jeu == NULL){
		return -1;
	}
	(jeu -> board)[(sequence -> c_old).x][(sequence -> c_old).y] = sequence -> old_orig;
	(jeu -> board)[(sequence -> c_new).x][(sequence -> c_new).y] = 0x0;
	if(sequence -> piece_value != 0x0){
		(jeu -> board)[(sequence -> piece_taken).x][(sequence -> piece_taken).y] = sequence -> piece_value;
		nPieces[getColor(sequence -> piece_value)]++;
	}
	return 0;
}
/*
 * undo_moves
 * Annule des mouvements et remet l'état du jeu correspondant
 *
 * @game: pointeur vers la structure du jeu
 * @n: nombre de coups à annuler. Si n > nombre total de mouvements, alors ignorer le surplus.
 * @return: 0 si OK, -1 si erreur
 */
int undo_moves(struct game *game, int n){
	int i;
	struct move *mouvement = pop_move(game);
	struct move_seq *current;
	for(i = 0 ; i < n && mouvement != NULL ; i++){
		current = mouvement -> seq;
		while(current != NULL){
			undo_seq(game, current);
			mouvement -> seq = (mouvement -> seq) -> next;
			free(current);
			current = mouvement -> seq;
		}
		game -> cur_player = ~(game -> cur_player) & 1;	//On inverse le joueur qui doit jouer
		free(mouvement);
		mouvement = pop_move(game);
	}
}

/*
 * print_board
 * Affiche l'état du jeu sur le terminal
 *
 * @game: pointeur vers la structure du jeu
 */
void print_board(const struct game *game){
	int i,j;	
	for(j = 0 ; j < 10 ; j++){
		for(i = 0 ; i < 10 ; i++){
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
	printf("Pieces restantes : %d noires, %d blanches.\n", nPieces[PLAYER_BLACK], nPieces[PLAYER_WHITE]);
}


void free_move_seq(struct move_seq * seq){
	struct move_seq *precedent = seq;
	while(precedent != NULL){
		seq = seq -> next;
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

//Fonctionne, normalement. Verifié avec Valgrind
void free_game(struct game *game){
	if(game == NULL){
		printf("Il n'y a pas de jeu a liberer\n");
		exit(EXIT_FAILURE);
	}
	int i;
	for(i = 0 ; i < 10 ; i++){
		free(*((game -> board) + i));
	}
	struct move *precedent = game -> moves;
	while(precedent != NULL){
		game -> moves = (game -> moves) -> next;
		free_move_seq(precedent -> seq);
		free(precedent);
		precedent = game -> moves;
	}
	free(game -> board);
	free(game);
}
