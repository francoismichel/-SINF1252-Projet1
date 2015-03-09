#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <stdio.h>
#include "dames.h"

// Test de la fonction new_game
void test_new_game(){
	struct game *jeu = new_game(10, 10);
	CU_ASSERT_PTR_NOT_NULL(jeu);
	CU_ASSERT_PTR_NOT_NULL(jeu -> board);
	int i;
	int j;
	for(i = 0 ; i < 10 ; i++){
		CU_ASSERT_PTR_NOT_NULL(*(jeu -> board) + i);
		for(j = 0 ; j < 10 ; j++){
			if (!( *(*((jeu -> board) + i) + j) == 0x1 || *(*((jeu -> board) + i) + j) == 0x5 || *(*((jeu -> board) + i) + j) == 0x0)){
				CU_FAIL("Case du damier mal initialisée\n");
			}
			else CU_PASS("Case du damier initialisée\n");
		}
	}
	CU_ASSERT_EQUAL(jeu->cur_player, 1);
}

// Test de la fonction load_game
void test_load_game(){
	struct game *jeu_1 = new_game(10, 10);
	struct move *m1 = (struct move *) malloc(sizeof(struct move));
	if(m1 == NULL){
		printf("Erreur d'allocation de memoire\n");
		exit(EXIT_FAILURE);	
	}
	m1->next = jeu_1->moves;
	struct move_seq *seq1 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq1 == NULL){
		printf("Erreur d'allocation de memoire\n");
		exit(EXIT_FAILURE);	
	}
	
	struct coord old = {1,6};
	struct coord new = {0,5};
	seq1->next = NULL;
	seq1->c_old = old;
	seq1->c_new = new;
	m1->seq = seq1;
	
	int m1_check = apply_moves(jeu_1, m1);
	CU_ASSERT_EQUAL(m1_check, 0);
	
	// Un double pointeur non constant ne peut pas être utilisé en tant qu'argument constant
	const int **plateau = (const int **) malloc(10*sizeof(int *));
	if(plateau == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	
	plateau = (const int**) jeu_1->board;
	struct game *jeu_2 = load_game(10, 10, plateau, jeu_1->cur_player);
	CU_ASSERT_PTR_NOT_NULL(jeu_2);
	
	struct game *jeu_3 = load_game(10, 10, NULL, jeu_1->cur_player);
	CU_ASSERT_PTR_NULL(jeu_3);
	
	struct game *jeu_4 = load_game(-1, 100, plateau, jeu_1->cur_player);
	CU_ASSERT_PTR_NULL(jeu_4);
	
	struct game *jeu_5 = load_game(10, 10, plateau, 2);
	CU_ASSERT_PTR_NULL(jeu_5);
	
	free_game(jeu_1);
	free_game(jeu_2);
	// Pas de free_game(jeu_3), celui-ci vaut NULL
	// Pas de free_game(jeu_4), celui-ci vaut NULL
	// Pas de free_game(jeu_5), celui-ci vaut NULL
}

void test_free_game(){
	/*
	 * Un seul test est fait ici, pour deux raisons :
	 * - La fonction free_game est déjà testée à plusieurs reprises dans les autres fonctions de test
	 * - free_game ne renvoie aucune valeur à tester
	 */
	struct game *jeu_1 = new_game(10, 10);
	
	free_game(jeu_1);
	CU_PASS("Jeu libéré avec succès");
}

void test_apply_moves(){
	
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
	struct move *m5 = (struct move *) malloc(sizeof(struct move));
	if(m4 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	
	m1->next = NULL;
	m2->next = NULL;
	m3->next = NULL;
	m4->next = NULL;
	m5->next = NULL;
	
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
	struct move_seq *seq5 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq5 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	
	// Déplacement d'un pion blanc
	struct coord old = {1,6};
	struct coord new = {0,5};
	seq1->next = NULL;
	seq1->c_old = old;
	seq1->c_new = new;
	m1->seq = seq1;
	
	// Déplacement d'un pion noir
	struct coord old_2 = {2,3};
	struct coord new_2 = {1,4};
	seq2->next = NULL;
	seq2->c_old = old_2;
	seq2->c_new = new_2;
	m2->seq = seq2;
	
	// Capture d'un pion noir par les blancs
	struct coord old_3 = {0,5};
	struct coord new_3 = {2,3};
	struct coord piece_taken_3 = {1,4};
	seq3->next = NULL;
	seq3->c_old = old_3;
	seq3->c_new = new_3;
	seq3->piece_value = 0x1;
	seq3->piece_taken = piece_taken_3;
	seq3->old_orig = 0x5;
	m3->seq = seq3;
	
	// Capture d'un pion blanc par les noirs
	struct coord old_4 = {1,2};
	struct coord new_4 = {3,4};
	struct coord piece_taken_4 = {2,3};
	seq4->next = NULL;
	seq4->c_old = old_4;
	seq4->c_new = new_4;
	seq4->piece_value = 0x5;
	seq4->piece_taken = piece_taken_4;
	seq4->old_orig = 0x1;
	m4->seq = seq4;
	
	// Essayer d'avancer verticalement
	struct coord old_5 = {7,6};
	struct coord new_5 = {7,5};
	seq5->next = NULL;
	seq5->c_old = old_5;
	seq5->c_new = new_5;
	m5->seq = seq5;
	
	int m1_check = apply_moves(jeu_1, m1);
	CU_ASSERT_EQUAL(m1_check, 0);
	
	int m2_check = apply_moves(jeu_1, m2);
	CU_ASSERT_EQUAL(m2_check, 0);
	
	int m3_check = apply_moves(jeu_1, m3);
	CU_ASSERT_EQUAL(m3_check, 0);
	
	int m4_check = apply_moves(jeu_1, m4);
	CU_ASSERT_EQUAL(m4_check, 0);
	
	int m5_check = apply_moves(jeu_1, m5);
	CU_ASSERT_EQUAL(m5_check, -1);
	
	free_game(jeu_1);
}

void test_is_move_seq_valid(){
	struct game *jeu_1 = new_game(10, 10);
	
	// Déplacement simple, valide
	struct move_seq *seq1 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq1 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	
	struct coord old = {5,6};
	struct coord new = {4,5};
	seq1->next = NULL;
	seq1->c_old = old;
	seq1->c_new = new;
	
	// Déplacement en dehors du plateau, invalide
	struct move_seq *seq2 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq2 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	
	struct coord old_2 = {9,6};
	struct coord new_2 = {10,5};
	seq2->next = NULL;
	seq2->c_old = old_2;
	seq2->c_new = new_2;
	
	// Déplacement des noirs au premier tour, invalide
	struct move_seq *seq3 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq3 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	
	struct coord old_3 = {4,3};
	struct coord new_3 = {3,4};
	seq3->next = NULL;
	seq3->c_old = old_3;
	seq3->c_new = new_3;
	
	int check_valid_1 = is_move_seq_valid(jeu_1, seq1, NULL, NULL);
	CU_ASSERT_EQUAL(check_valid_1, 1);
	
	int check_valid_2 = is_move_seq_valid(jeu_1, seq2, NULL, NULL);
	CU_ASSERT_EQUAL(check_valid_2, 0);
	
	int check_valid_3 = is_move_seq_valid(jeu_1, seq3, NULL, NULL);
	CU_ASSERT_EQUAL(check_valid_3, 0);
	
	free_game(jeu_1);
	free(seq1);
	free(seq2);
	free(seq3);
}

void test_undo_moves(){
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
	
	struct coord old = {3,6};
	struct coord new = {4,5};
	seq1->next = NULL;
	seq1->c_old = old;
	seq1->c_new = new;
	m1->seq = seq1;
	
	struct coord old_2 = {6,3};
	struct coord new_2 = {5,4};
	seq2->next = NULL;
	seq2->c_old = old_2;
	seq2->c_new = new_2;
	m2->seq = seq2;
	
	struct coord old_3 = {4,5};
	struct coord new_3 = {6,3};
	struct coord piece_taken_3 = {5,4};
	seq3->next = NULL;
	seq3->c_old = old_3;
	seq3->c_new = new_3;
	seq3->piece_value = 0x1;
	seq3->piece_taken = piece_taken_3;
	seq3->old_orig = 0x5;
	m3->seq = seq3;
	
	struct coord old_4 = {5,2};
	struct coord new_4 = {7,4};
	struct coord piece_taken_4 = {6,3};
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
	int m3_check = apply_moves(jeu_1, m3);
	CU_ASSERT_EQUAL(m3_check, 0);
	int m4_check = apply_moves(jeu_1, m4);
	CU_ASSERT_EQUAL(m4_check, 0);
	
	// On retire un mouvement de trop, mais cela reste valide
	/*int check_undo_1 = undo_moves(jeu_1, 5);
	CU_ASSERT_EQUAL(check_undo_1, 0);*/
	
	free_game(jeu_1);
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
	if((NULL == CU_add_test(pSuite, "test de test_new_game()", test_new_game)) ||
		(NULL == CU_add_test(pSuite, "test de test_load_game()", test_load_game)) ||
		(NULL == CU_add_test(pSuite, "test de test_free_game()", test_free_game)) ||
		(NULL == CU_add_test(pSuite, "test de test_apply_moves()", test_apply_moves)) ||
		(NULL == CU_add_test(pSuite, "test de test_is_move_seq_valid()", test_is_move_seq_valid)) ||
		(NULL == CU_add_test(pSuite, "test de test_undo_moves()", test_undo_moves))){
		
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
