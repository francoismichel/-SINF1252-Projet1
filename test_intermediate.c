#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <stdio.h>
#include "dames.h"
#include "dames_intermediate.h"

void test_getColor(){
	int pionNoir = 0x1;
	int pionBlanc = 0x5;
	int dameNoir = 0x3;
	int dameBlanc = 0x7;
	int caseVide = 0x0;
	int check_couleur = 3;
	
	check_couleur = getColor(pionNoir);
	CU_ASSERT_EQUAL(check_couleur, 0);
	check_couleur = getColor(pionBlanc);
	CU_ASSERT_EQUAL(check_couleur, 1);
	check_couleur = getColor(dameNoir);
	CU_ASSERT_EQUAL(check_couleur, 0);
	check_couleur = getColor(dameBlanc);
	CU_ASSERT_EQUAL(check_couleur, 1);
	check_couleur = getColor(caseVide);
	CU_ASSERT_EQUAL(check_couleur, 2);
}

void test_isOutOfBoard(){
	struct coord c_old = {7,6};
	struct coord c_new = {8,5};
	struct move_seq *seq1 = (struct move_seq *) malloc(sizeof(struct move_seq));
	seq1->c_old = c_old;
	seq1->c_new = c_new;
	struct coord c_old_2 = {0,3};
	struct coord c_new_2 = {-1,4};
	struct move_seq *seq2 = (struct move_seq *) malloc(sizeof(struct move_seq));
	seq1->c_old = c_old_2;
	seq1->c_new = c_new_2;
	struct coord c_old_3 = {4,9};
	struct coord c_new_3 = {5,10};
	struct move_seq *seq3 = (struct move_seq *) malloc(sizeof(struct move_seq));
	seq1->c_old = c_old_3;
	seq1->c_new = c_new_3;
	int check_board = 0;
	
	check_board = isOutOfBoard(seq1);
	CU_ASSERT_EQUAL(check_board, 1);
	
	check_board = isOutOfBoard(seq2);
	CU_ASSERT_EQUAL(check_board, 0);
	
	check_board = isOutOfBoard(seq3);
	CU_ASSERT_EQUAL(check_board, 0);
	
	free(seq1);
	free(seq2);
	free(seq3);
}

void test_isDame(){
	int pionNoir = 0x1;
	int pionBlanc = 0x5;
	int dameNoir = 0x3;
	int dameBlanc = 0x7;
	int caseVide = 0x0;
	int check_dame = 2;
	
	check_dame = isDame(pionNoir);
	CU_ASSERT_EQUAL(check_dame, 0);
	check_dame = isDame(pionBlanc);
	CU_ASSERT_EQUAL(check_dame, 0);
	check_dame = isDame(dameNoir);
	CU_ASSERT_EQUAL(check_dame, 1);
	check_dame = isDame(dameBlanc);
	CU_ASSERT_EQUAL(check_dame, 1);
	check_dame = isDame(caseVide);
	CU_ASSERT_EQUAL(check_dame, 0);
}

void test_isCoordInBoard(){
	CU_ASSERT_EQUAL(isCoordInBoard(3,7), 1);
	CU_ASSERT_EQUAL(isCoordInBoard(7,9), 1);
	CU_ASSERT_EQUAL(isCoordInBoard(10,9), 0);
	CU_ASSERT_EQUAL(isCoordInBoard(0,0), 1);
	CU_ASSERT_EQUAL(isCoordInBoard(-1,5), 0);
}

void test_isValidMovePiece(){
	struct game *jeu_1 = new_game(10, 10);
	
	CU_ASSERT_EQUAL(isValidMovePiece(jeu_1, 1, 6, PLAYER_WHITE), 1);
	CU_ASSERT_EQUAL(isValidMovePiece(jeu_1, 0, 7, PLAYER_WHITE), 0);
	CU_ASSERT_EQUAL(isValidMovePiece(jeu_1, 5, 2, PLAYER_BLACK), 0);
	CU_ASSERT_EQUAL(isValidMovePiece(jeu_1, 0, 3, PLAYER_BLACK), 1);
	
	free_game(jeu_1);
}

void test_canPlay(){
	struct game *jeu_1 = new_game(10, 10);
	
	CU_ASSERT_EQUAL(canPlay(jeu_1, PLAYER_WHITE), 1);
	CU_ASSERT_EQUAL(canPlay(jeu_1, PLAYER_BLACK), 1);
	
	free_game(jeu_1);
}

void test_getDiagonal(){
	struct coord c1 = {9,6};
	struct coord c2 = {8,5};
	struct coord c3 = {8,7};
	struct coord c4 = {4,1};
	
	// Les quatres directions possibles
	CU_ASSERT_EQUAL(getDiagonal(c1,c2), 1);
	CU_ASSERT_EQUAL(getDiagonal(c2,c1), 4);
	CU_ASSERT_EQUAL(getDiagonal(c1,c3), 3);
	CU_ASSERT_EQUAL(getDiagonal(c3,c1), 2);
	
	// Longue diagonale
	CU_ASSERT_EQUAL(getDiagonal(c1,c4), 1);
}

int main(int argc, char *argv[]){
	
	// Notre suite de tests
	CU_pSuite pSuite = NULL;
	
	// Initialisation du registre des tests de CUnit
	if(CUE_SUCCESS != CU_initialize_registry()){
		return CU_get_error();
	}
	
	// Les fonctions de "setup" et de "teardown" ne sont pas utilisées ici
	pSuite = CU_add_suite("Suite_1", NULL, NULL);
	if(NULL == pSuite){
		CU_cleanup_registry();
		return CU_get_error();
	}
	// Ajout de nos tests à notre suite de tests
	if((NULL == CU_add_test(pSuite, "test de test_getColor()", test_getColor)) ||
		(NULL == CU_add_test(pSuite, "test de test_isOutOfBoard()", test_isOutOfBoard)) ||
		(NULL == CU_add_test(pSuite, "test de test_isDame()", test_isDame)) ||
		(NULL == CU_add_test(pSuite, "test de test_isCoordInBoard()", test_isCoordInBoard)) ||
		(NULL == CU_add_test(pSuite, "test de test_canPlay()", test_canPlay)) ||
		(NULL == CU_add_test(pSuite, "test de test_isValidMovePiece()", test_isValidMovePiece)) ||
		(NULL == CU_add_test(pSuite, "test de test_getDiagonal()", test_getDiagonal))){
		
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	// Exécution des tests
	CU_basic_run_tests();
	
	// Nécessaire à la fin : libération des ressources utilisées par les tests
	CU_cleanup_registry();
	printf("Tests CUnit terminés !\n");
	return CU_get_error();
}
