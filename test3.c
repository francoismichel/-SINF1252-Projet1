#include <stdlib.h>
#include <stdio.h>
#include "dames.h"

int main(int argc, char *argv[]){
	struct coord old = {1,6};
	struct coord new = {2,5};
	struct move_seq sequence = {NULL, old, new};
	struct move mouvement = {NULL, &sequence};
	//mouvement.next = NULL;
	//(mouvement.seq) -> c_old = {1,6};
	//(mouvement.seq) -> c_new = {2,5};
	struct game *jeu = new_game(10,10);
	print_board(jeu);
	apply_moves(jeu, &mouvement);
	print_board(jeu);
	undo_moves(jeu, 1);
	print_board(jeu);
	free_game(jeu);
	return EXIT_SUCCESS;
}
