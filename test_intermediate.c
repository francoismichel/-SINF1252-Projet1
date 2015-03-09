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

void test_pieceBienPrise(){
	struct game *jeu_1 = new_game(10, 10);
	
	struct move *m1 = (struct move *) malloc(sizeof(struct move));
	if(m1 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m2 = (struct move *) malloc(sizeof(struct move));
	if(m2 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m3 = (struct move *) malloc(sizeof(struct move));
	if(m3 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m4 = (struct move *) malloc(sizeof(struct move));
	if(m4 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	m1->next = NULL;
	m2->next = NULL;
	m3->next = NULL;
	m4->next = NULL;
	
	struct move_seq *seq1 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq1 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq2 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq2 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq3 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq3 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq4 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq4 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	
	struct coord old = {5,6};
	struct coord new = {6,5};
	seq1->next = NULL;
	seq1->c_old = old;
	seq1->c_new = new;
	m1->seq = seq1;
	
	struct coord old_2 = {8,3};
	struct coord new_2 = {7,4};
	seq2->next = NULL;
	seq2->c_old = old_2;
	seq2->c_new = new_2;
	m2->seq = seq2;
	 
	struct coord old_3 = {6,5};
	struct coord new_3 = {8,3};
	struct coord piece_taken_3 = {7,4};	
	seq3->next = NULL;
	seq3->c_old = old_3;
	seq3->c_new = new_3;
	seq3->piece_value = 0x1;
	seq3->piece_taken = piece_taken_3;
	seq3->old_orig = 0x5;
	m3->seq = seq3;
	
	struct coord old_4 = {9,2};
	struct coord new_4 = {7,4};
	struct coord piece_taken_4 = {8,3};
	seq4->next = NULL;
	seq4->c_old = old_4;
	seq4->c_new = new_4;
	seq4->piece_value = 0x5;
	seq4->piece_taken = piece_taken_4;
	seq4->old_orig = 0x1;
	m4->seq = seq4;
	
	int m1_check = apply_moves(jeu_1, m1);
	CU_ASSERT_EQUAL(m1_check, 0);
	int m2_check = apply_moves(jeu_1, m2);
	CU_ASSERT_EQUAL(m2_check, 0);
	
	CU_ASSERT_EQUAL(pieceBienPrise(jeu_1, &piece_taken_3, old_3, new_3), 1);
	
	int m3_check = apply_moves(jeu_1, m3);
	CU_ASSERT_EQUAL(m3_check, 0);
	
	CU_ASSERT_EQUAL(pieceBienPrise(jeu_1, &piece_taken_4, old_4, new_4), 1);
	
	int m4_check = apply_moves(jeu_1, m4);
	CU_ASSERT_EQUAL(m4_check, 0);
	
	free_game(jeu_1);
}

void test_isDiagonal(){
	struct coord c1 = {1,8};
	struct coord c2 = {6,3};
	struct coord c3 = {9,0};
	struct coord c4 = {1,3};
	
	// Diagonales valides
	CU_ASSERT_EQUAL(isDiagonal(c1,c2), 5);
	CU_ASSERT_EQUAL(isDiagonal(c1,c3), 8);
	CU_ASSERT_EQUAL(isDiagonal(c3,c2), 3);
	
	// Diagonale non valide
	CU_ASSERT_EQUAL(getDiagonal(c1,c4), 0);
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
		(NULL == CU_add_test(pSuite, "test de test_getDiagonal()", test_getDiagonal)) ||
		(NULL == CU_add_test(pSuite, "test de test_pieceBienPrise()", test_pieceBienPrise)) ||
		(NULL == CU_add_test(pSuite, "test de test_isDiagonal()", test_isDiagonal))){
		
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
