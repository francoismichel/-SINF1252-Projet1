#include <stdlib.h>
#include <stdio.h>
#include "dames.h"
#include "dames_intermediate.h"

struct game *new_game(int xsize, int ysize){
	// On alloue de la mémoire pour créer la structure du jeu
	struct game *jeu = (struct game *) malloc(sizeof(struct game));
	if(jeu == NULL){  // Si le malloc a échoué
		printf("Mémoire insuffisante pour créer le jeu.\n");
		exit(EXIT_FAILURE);
	}
	int i;
	jeu -> board  = (int **) malloc(xsize*sizeof(int *));
	if ((jeu->board) == NULL){ // Si le malloc a échoué
		printf("Mémoire insuffisante pour créer le plateau de jeu.\n");
		exit(EXIT_FAILURE);
	}
	for(i = 0 ; i < xsize ; i++){
		// On alloue de la mémoire pour le plateau de jeu.
		*((jeu -> board) + i) = (int *) malloc(ysize*sizeof(int));
		if((jeu -> board) + i == NULL){ // Si le malloc a échoué
			printf("Mémoire insuffisante pour créer le plateau de jeu.\n");
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
				// *(*((jeu -> board) + j) + i) = 0x0;
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
	
	// Vérification de la taille du plateau de jeu
	if (xsize != 10 || ysize != 10) return NULL;
	
	// Vérification du plateau de jeu donné
	if (board == NULL) return NULL;
	
	// Vérification du tour du joueur donné
	if ((cur_player != PLAYER_BLACK) && (cur_player != PLAYER_WHITE)) return NULL;
	
	struct game *jeu = (struct game *) malloc(sizeof(struct game));  
	if(jeu == NULL){  // Si le malloc a échoué
		printf("Mémoire insuffisante pour créer le jeu.\n");
		exit(EXIT_FAILURE);
	}
	
	// On initialise toutes les variables de la structure.
	jeu -> xsize = xsize;
	jeu -> ysize = ysize;
	
	// On recréé un plateau de jeu pour le jeu chargé
	// L'opération est nécessaire, pour éviter de libérer deux fois le même plateau,
	// Dans le cas où on libère un jeu créé et un jeu chargé à partir de ce dernier.
	jeu -> board  = (int **) malloc(xsize*sizeof(int *));
	if ((jeu->board) == NULL){ // Si le malloc a échoué
		printf("Mémoire insuffisante pour créer le plateau de jeu.\n");
		exit(EXIT_FAILURE);
	}
	int i, j;
	for(i = 0 ; i < xsize ; i++){
		// On alloue de la mémoire pour le plateau de jeu.
		*((jeu -> board) + i) = (int *) malloc(ysize*sizeof(int));
		if((jeu -> board) + i == NULL){ // Si le malloc a échoué
			printf("Mémoire insuffisante pour créer le plateau de jeu.\n");
			exit(EXIT_FAILURE);
		}
	}
	for(i = 0 ; i < xsize ; i++){
		for(j = 0 ; j < ysize ; j++){
			(jeu->board)[i][j] = *(*((board) + i) + j);
		}
	}
	
	jeu -> cur_player = cur_player;
	jeu -> moves = (struct move *) malloc(sizeof(struct move));
	jeu -> moves = NULL;
	if(&(jeu->moves) == NULL){  // Si le malloc a échoué
		printf("Erreur d'allocation de mémoire.\n");
		exit(EXIT_FAILURE);
	}
	return jeu;										  
}

int getColor(int piece){
	if(piece == 0x0){
		return 2;
	}
	if ((piece != 0x1) && (piece != 0x5) && (piece != 0x3) && (piece != 0x7)){
		printf("Pièce non valide, impossible de récupérer la couleur");
		exit(EXIT_FAILURE);
	}
	return ((piece & 0x4) >> 2) & 1; 
}

int isOutOfBoard(const struct move_seq *seq){
	struct coord c_avant = seq -> c_old;
	struct coord c_apres = seq -> c_new;
	if (((c_avant.x >= 0) && (c_avant.x < 10) && (c_avant.y) >= 0 && (c_avant.y < 10)
		&& (c_apres.x >= 0) && (c_apres.x < 10) && (c_apres.y) >= 0 && (c_apres.y) < 10) == 1)
		return 0;
	else return 1;
}

int isDame(int piece){
 	return (piece & 0x2) == 0x2;
}


int isCoordInBoard(int x, int y){
	return x < 10 && x >= 0 && y < 10 && y >= 0;
}


int isValidMovePiece(const struct game *jeu, int x, int y){
	int **plateau = jeu -> board;
	int yMoveValue;	
	//Si la couleur de la piece qu'on verifie n'est pas la couleur du joueur actuel, on retourne 0 direct
	if(getColor(plateau[x][y]) != jeu -> cur_player){
		return 0;
	}
	//Si le joueur actuel est PLAYER_WHITE, il avance de base vers le haut (y diminue, donc yMoveValue = -1)
	if(getColor(plateau[x][y]) == PLAYER_WHITE){
		yMoveValue = -1;
	}
	//Si le joueur actuel est PLAYER_BLACK, il avance de base vers le bas (y augmente, donc yMoveValue = 1)
	else{
		yMoveValue = 1;
	}
	//On regarde s'il y a des cases libres dans les cases adjacentes, "devant" la pièce
	if((isCoordInBoard(x+1, y + yMoveValue) && (plateau[x+1][y + yMoveValue] == 0)) || 
	(isCoordInBoard(x-1, y + yMoveValue) > 0 && (plateau[x-1][y + yMoveValue] == 0))){
		return 1;
	}
	//Si c'est une dame
	if(isDame(plateau[x][y])){
		//On vérifie les cases "derrière elle" aussi
		if((isCoordInBoard(x+1, y - yMoveValue) && (plateau[x+1][y - yMoveValue] == 0)) || 
		(isCoordInBoard(x-1, y - yMoveValue) > 0 && (plateau[x-1][y - yMoveValue] == 0))){
			return 1;
		}	
	}
	//S'il n'y a pas de case libre adjacente à la pièce
	else{
		//On regarde si on peut sauter au-dessus d'un pion, par l'avant EST
		if(isCoordInBoard(x+2, y + 2*yMoveValue) && getColor(plateau[x+1][y + yMoveValue]) != jeu -> cur_player){
			if(plateau[x+2][y + 2*yMoveValue] == 0x0){
				return 1;
			}
		}
		//Par l'avant OUEST
		if(isCoordInBoard(x-2, y + 2*yMoveValue) && getColor(plateau[x-1][y + yMoveValue]) != jeu -> cur_player){
			if(plateau[x-2][y + 2*yMoveValue] == 0x0){
				return 1;
			}
		}
		//Par l'arrière EST
		if(isCoordInBoard(x+2, y - 2*yMoveValue) && plateau[x+1][y - yMoveValue] != 0x0){
			if(getColor(plateau[x+1][y - yMoveValue]) != jeu -> cur_player && plateau[x+2][y + 2*yMoveValue] == 0x0){
				return 1;
			}
		}
		//Par l'arrière OUEST
		if(isCoordInBoard(x-2, y - 2*yMoveValue) && plateau[x-1][y - yMoveValue] != 0x0){
			if(getColor(plateau[x-1][y - yMoveValue]) != jeu -> cur_player && plateau[x-2][y + 2*yMoveValue] == 0x0){
				return 1;
			}
		}
	}
	//Si aucun des tests n'a réussi, c'est qu'il n'y a aucun mouvement valide pour cette pièce.
	return 0;
}

int canPlay(const struct game *jeu){
	int i,j;
	for(i = 0 ; i < jeu -> xsize ; i++){
		for(j = 0 ; j < jeu -> ysize ; j++){
			//On verifie si la piece aux coordonnees i,j peut jouer actuellement
			if(isValidMovePiece(jeu, i, j)){
				//Si la pièce peut effectuer un mouvement valide, on retourne 1
				return 1;
			}
		}
	}
	//Si aucune pièce ne peut jouer actuellement, retourne 0
	return 0;
}

int getDiagonal(struct coord c_avant, struct coord c_apres){
	if(c_apres.x - c_avant.x > 0 && c_apres.y - c_avant.y > 0){
		return SUDEST;
	}
	else if(c_apres.x - c_avant.x > 0 && c_apres.y - c_avant.y < 0){
		return NORDEST;
	}
	else if(c_apres.x - c_avant.x < 0 && c_apres.y - c_avant.y > 0){
		return SUDOUEST;
	}	
	else if(c_apres.x - c_avant.x < 0 && c_apres.y - c_avant.y < 0){
		return NORDOUEST;
	}
	else{
		return 0;
	}
}

int pieceBienPrise(const struct game *jeu, struct coord *prise, struct coord c_avant, struct coord c_apres){
	int piecePrise = (jeu -> board)[prise -> x][prise -> y];
	int pieceQuiJoue = (jeu -> board)[c_avant.x][c_avant.y];
	
	// On vérifie que la pièce qui joue et la pièce prise sont de couleurs différentes
	if((getColor(piecePrise) ^ getColor(pieceQuiJoue)) == 0x1){
		// On vérifie que la pièce qui joue a bien sauté au-dessus de la pièce prise :
		return ( (c_avant.x + c_apres.x)/2 == prise -> x ) && ( (c_avant.y + c_apres.y)/2 == prise -> y ); 
	}
	// Sinon, la prise de piece n'est pas valide
	else{
		return 0;
	}
}

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
	// Donc ici, on sait que ValeurMove vaut 1 ou 2 (prise de pion ou non)
	
	if(valeurMove == 2){
		int piecePrise;
		int diagonale = getDiagonal(c_avant, c_apres);
		if(diagonale == SUDEST){
			piecePrise = (jeu -> board)[c_avant.x + 1][c_avant.y + 1];
			if(piecePrise == 0x0){
				return 0;
			}
			taken -> x = c_avant.x + 1;
			taken -> y = c_avant.y + 1;
		}
		else if(diagonale == SUDOUEST){
			piecePrise = (jeu -> board)[c_avant.x - 1][c_avant.y + 1];
			if(piecePrise == 0x0){
				return 0;
			}
			taken -> x = c_avant.x - 1;
			taken -> y = c_avant.y + 1;
		}
		else if(diagonale == NORDEST){
			piecePrise = (jeu -> board)[c_avant.x + 1][c_avant.y - 1];
			if(piecePrise == 0x0){
				return 0;
			}
			taken -> x = c_avant.x + 1;
			taken -> y = c_avant.y - 1;
		}
		else{
			piecePrise = (jeu -> board)[c_avant.x - 1][c_avant.y - 1];
			if(piecePrise == 0x0){
				return 0;
			}
			taken -> x = c_avant.x - 1;
			taken -> y = c_avant.y - 1;
		}
		return 2;
	}
	int piece = (jeu -> board)[c_avant.x][c_avant.y];
	
	// Si la pièce est un pion blanc, on regarde si son mouvement est correct (mouvement en diagonale)
	if(getColor(piece) == PLAYER_WHITE){
	return (c_apres.x - c_avant.x == -valeurMove && c_apres.y - c_avant.y == -valeurMove) ||
		   (c_apres.x - c_avant.x == valeurMove && c_apres.y - c_avant.y == -valeurMove);
	}
	// Si c'est un pion noir, même chose, sauf que le sens de déplacement a changé	
	else{
		return (c_apres.x - c_avant.x == -valeurMove && c_apres.y - c_avant.y == valeurMove) ||
			   (c_apres.x - c_avant.x == valeurMove && c_apres.y - c_avant.y == valeurMove);
	}
}

int isCorrectMoveDame(const struct game *jeu, struct coord c_avant, struct coord c_apres, struct coord *taken){
	int valeurMove = isDiagonal(c_avant, c_apres);
	//Si la dame ne bouge pas en diagonale
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
		//Si on arrive sur une case d'une piece de la même couleur que nous
		if(position != 0x0 && getColor(position) == getColor((jeu -> board)[c_avant.x][c_avant.y])){
			//On ne peut pas, on retourne 0
			return 0;
		}
		//Sinon, si on n'est pas sur une case vide et donc qu'on passe au-dessus d'une ennemi
		else if(position != 0x0){
			//Si on ne l'a jamais fait
			if(prise == 0){
				//On dit qu'on a pris un pion
				prise = 1;
			}
			//Si on l'a déjà fait lors de la même séquence
			else{
				//On n'a pas le droit de le faire, on retourne donc 0
				return 0;
			}
		}
	}
	return 1; //Si on a passé tous les test, la séquence est valide, on retourne 1
}

int isMoveValid(const struct game *jeu, struct coord c_avant, struct coord c_apres, int piece, struct coord *taken){
	// On vérifie si la pièce atterit sur une case vide
	if((jeu -> board)[c_apres.x][c_apres.y] != 0){
		return 0;
	}
	
	// Si la pièce est un pion
	if(!isDame(piece)){
		return isCorrectMovePion(jeu, c_avant, c_apres, taken);
	}
	// Sinon, la pièce est une dame
	else{
		return isCorrectMoveDame(jeu, c_avant, c_apres, taken);
	}
}

int is_move_seq_valid(const struct game *game, const struct move_seq *seq, const struct move_seq *prev, struct coord *taken){
	if(prev != NULL && (seq == NULL || (prev -> c_new).x != (seq -> c_old).x || (prev -> c_new).y != (seq -> c_old).y) ){
		return 0;
	}
	struct coord c_avant = seq -> c_old;
	struct coord c_apres = seq -> c_new;
	
	// On récupère la valeur de la pièce qui joue.
	int pieceQuiJoue = (game -> board)[c_avant.x][c_avant.y];
	
	//On récupère le 3eme bit de la pièce qui joue et on le compare au joueur qui doit jouer
	if(getColor(pieceQuiJoue) != game -> cur_player){
		printf("%d\n",pieceQuiJoue);
		return 0;
	}
	
	// Si la pièce avant ou après le mouvement se trouve en dehors du tableau, c'est invalide
	if(isOutOfBoard(seq)){
		return 0;
	}	
	return isMoveValid(game, c_avant, c_apres, pieceQuiJoue, taken);
}

void push_seq(struct game *jeu, const struct move_seq *seq, struct coord *piece_taken, int old_orig, int piece_value){
	if(seq == NULL){
		printf("aucune sequence a rajouter dans push_seq"\n);
		return;
	}
	if(jeu == NULL){
		printf("Aucun jeu auquel rajouter une séquence (pointeur NULL)\n");
	}
	struct move_seq *newSeq;
	newSeq = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(newSeq == NULL){
		printf("Erreur d'allocation de mémoire dans push_seq\n");
		return;	
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

void push_move(struct game *jeu){
	struct move *newMove = (struct move *) malloc(sizeof(struct move));
	newMove -> next = jeu -> moves;
	newMove -> seq = NULL;
	jeu -> moves = newMove;
}

void free_move_seq(struct move_seq * seq){
	struct move_seq *precedent = seq;
	while(precedent != NULL){
		seq = seq -> next;
		free(precedent);
		precedent = seq;
	}	
}

struct move *pop_move(struct game *jeu){
	if(jeu == NULL || jeu -> moves == NULL){
		return NULL;
	}
	struct move *mouvement = jeu -> moves;
	jeu -> moves = (jeu -> moves) -> next;
	return mouvement;
}

int apply_moves(struct game *game, const struct move *moves){
	const struct move *current = moves;
	struct move_seq *sequence = NULL;
	struct coord prise = {-1,-1};
	struct coord *taken = &prise;
	struct move_seq *previousSeq = NULL;
	struct coord c_avant;
	struct coord c_apres;
	int ennemy;						// Variable qui retient la couleur du pion qui se fait capturer, s'il y en a un
	int previousValid = 3;			// Variable qui retient la valeur retournée par l'appel de is_move_seq_valid sur la séquence précédente
	int isValid = 3;				// Variable qui retient la valeur qui sera retournée par l'appel de is_move_seq_valid sur la séquence en cours
	int playerPiece;				// Valeur de la pièce jouée lors de la séquence en cours
	int gotDame= 0;					// Détermine si le joueur a récupéré une dame lors de la séquence précédente ; 1: oui, 0: non
	
	// On parcourt la liste de moves
	while(current != NULL){
		push_move(game);
		sequence = current -> seq;
		// On parcourt la liste de séquences du move
		while(sequence != NULL){
			if(previousValid == 1 || gotDame == 1){
				struct move *mv = pop_move(game);
				free_move_seq(mv -> seq);
				free(mv);
				return -1;
			}
			isValid = is_move_seq_valid(game, sequence, previousSeq, taken);
			// Si la séquence n'est pas valide, le move n'est pas valide
			if(isValid == 0){
				return -1;
			}
			else if(isValid == 1){
				c_avant = sequence -> c_old;
				c_apres = sequence -> c_new;
				playerPiece = (game -> board)[c_avant.x][c_avant.y];
				ennemy = 0x0;
				(game -> board)[c_apres.x][c_apres.y] = (game -> board)[c_avant.x][c_avant.y];
				(game -> board)[c_avant.x][c_avant.y] = 0x0;
			}
			else{
				c_avant = sequence -> c_old;
				c_apres = sequence -> c_new;
				playerPiece = (game -> board)[c_avant.x][c_avant.y];
				(game -> board)[c_apres.x][c_apres.y] = playerPiece;
				(game -> board)[c_avant.x][c_avant.y] = 0x0;
				ennemy = (game -> board)[taken -> x][taken -> y];
				printf("hey3 : %d, %d\n", taken -> x, taken -> y);
				(game -> board)[taken -> x][taken -> y] = 0x0;
				nPieces[getColor(ennemy)]--;
				if(nPieces[getColor(ennemy)] == 0){
					return 1;
				}
			}
			push_seq(game, sequence, taken, playerPiece, ennemy);
			printf("push : %d\n", playerPiece);
			//Si le joueur adverse n'a plus de mouvement possible
			previousSeq = sequence;
			// On récupère une dame si nécessaire et on récupère le résultat
			gotDame = transformDame(game, c_apres);
			sequence = sequence -> next;
		}
		// Créer une fonction pour voir si le joueur peut encore jouer apres ou non (moves encore possibles,...)
		current = current -> next;
		// Comme on change de move, ce n'est plus le même joueur qui joue, donc on réinitialise previousValid.
		previousValid = 3;
		// On inverse le joueur qui joue
		game -> cur_player = ~(game -> cur_player) & 1;
		if(!canPlay(game)){
			//On retourne 1 pour dire qu'il a perdu
			return 1;
		}
	}
	return 0;
}

int undo_seq(struct game *jeu, struct move_seq *sequence){
	if(sequence == NULL || jeu == NULL){
		return -1;
	}
	printf("piece : %d\n", sequence -> old_orig);
	(jeu -> board)[(sequence -> c_old).x][(sequence -> c_old).y] = sequence -> old_orig;
	(jeu -> board)[(sequence -> c_new).x][(sequence -> c_new).y] = 0x0;
	if(sequence -> piece_value != 0x0){
		(jeu -> board)[(sequence -> piece_taken).x][(sequence -> piece_taken).y] = sequence -> piece_value;
		nPieces[getColor(sequence -> piece_value)]++;
	}
	return 0;
}

int undo_moves(struct game *game, int n){
	if(game == NULL || n <= 0){
		return -1;
	}
	int i;
	struct move *mouvement = game -> moves;
	if(mouvement == NULL){
		return -1;
	}
	struct move_seq *current;
	for(i = 0 ; i < n && mouvement != NULL ; i++){
		mouvement = pop_move(game);
		printf("%d\n",i);
		current = mouvement -> seq;
		if(current == NULL){
			return -1;
		}
		while(current != NULL){
			undo_seq(game, current);
			mouvement -> seq = (mouvement -> seq) -> next;
			free(current);
			current = mouvement -> seq;
		}
		// On inverse le joueur qui doit jouer
		game -> cur_player = ~(game -> cur_player) & 1;
		free(mouvement);
		mouvement = game -> moves;
	}
	return 0; // Description manquante, que retourner ?
}

void print_board(const struct game *game){
	int i,j;	
	char carac;
	printf("  ");
	for(j = 0 ; j < 10 ; j++){
		printf("%d ", j);
	}
	printf("\n");

	for(j = 0 ; j < 10 ; j++){
		printf("%d ", j);
		for(i = 0 ; i < 10 ; i++){
			if((game -> board)[i][j] == 0x0){
				carac = 'X';
			}
			else if((game -> board)[i][j] == 0x1){
				carac = 'n';
			}
			else if((game -> board)[i][j] == 0x5){
				carac = 'b';
			}
			else if((game -> board)[i][j] == 0x3){
				carac = 'N';
			}
			else if((game -> board)[i][j] == 0x7){
				carac = 'B';
			}
			else{
				printf("erreur de damier...\n");
				return;
			}
			printf("%c ", carac);
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

void free_game(struct game *game){
	if(game == NULL){
		printf("Il n'y a pas de jeu à libérer\n");
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
	free(game);
}
