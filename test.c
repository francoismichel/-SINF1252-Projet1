#include <CUnit.h>
#include <stdlib.h>
#include <stdio.h>
#include "dames.h"

//j'arrive pas a utiliser CUnit sur mon PC..
void test_newgame(){
	struct game *jeu = new_game(10,10);
	CU_ASSERT_PTR_NOT_NULL(jeu);
	CU_ASSERT_PTR_NOT_NULL(jeu -> board);
	int i;
	for(i = 0 ; i < 10 ; i++){
		CU_ASSERT_PTR_NOT_NULL(*(jeu -> board) + i);
	}
}

int main(int argc, char *argv[]){
	test_newgame();
	printf("Test reussi !\n");
}
