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
	
	free_game(jeu);
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
	const int **plateau;
	plateau = (const int**) jeu_1->board;
	
	struct game *jeu_2 = load_game(10, 10, plateau, jeu_1->cur_player);
	CU_ASSERT_PTR_NOT_NULL(jeu_2);
	
	struct game *jeu_3 = load_game(10, 10, NULL, jeu_1->cur_player);
	CU_ASSERT_PTR_NULL(jeu_3);
	
	struct game *jeu_4 = load_game(-1, 100, plateau, jeu_1->cur_player);
	CU_ASSERT_PTR_NULL(jeu_4);
	
	struct game *jeu_5 = load_game(10, 10, plateau, 2);
	CU_ASSERT_PTR_NULL(jeu_5);
	free(m1);
	free(seq1);
	free_game(jeu_1);
	free_game(jeu_2);
	// Pas de free_game(jeu_3), celui-ci vaut NULL
	// Pas de free_game(jeu_4), celui-ci vaut NULL
	// Pas de free_game(jeu_5), celui-ci vaut NULL
}

// Test de la fonction free_game
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

// Test de la fonction apply_moves
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
	
	free(m1);
	free(m2);
	free(m3);
	free(m4);
	free(m5);
	free(seq1);
	free(seq2);
	free(seq3);
	free(seq4);
	free(seq5);
	free_game(jeu_1);
}

// Test de la fonction is_move_seq_valid
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

// Test de la fonction undo_moves
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
	int check_undo_1 = undo_moves(jeu_1, 100);
	CU_ASSERT_EQUAL(check_undo_1, 0);
	
	// On ne peut pas retirer un nombre négatif de moves
	int check_undo_2 = undo_moves(jeu_1, -1);
	CU_ASSERT_EQUAL(check_undo_2, -1);
	
	// Retirer 0 moves est considéré comme une erreur
	int check_undo_3 = undo_moves(jeu_1, 0);
	CU_ASSERT_EQUAL(check_undo_3, -1);
	
	free(m1);
	free(m2);
	free(m3);
	free(m4);
	free(seq1);
	free(seq2);
	free(seq3);
	free(seq4);
	free_game(jeu_1);
}

/*
 * Test d'une partie avec plusieurs coups pouvant paraître "tricky"
 */
void test_partie(){
	// Démarrage d'une nouvelle partie
	struct game *jeu_1 = new_game(10, 10);
	
	// Allocation des 30 moves à exécuter
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
	if(m5 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m6 = (struct move *) malloc(sizeof(struct move));
	if(m6 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m7 = (struct move *) malloc(sizeof(struct move));
	if(m7 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m8 = (struct move *) malloc(sizeof(struct move));
	if(m8 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m9 = (struct move *) malloc(sizeof(struct move));
	if(m9 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m10 = (struct move *) malloc(sizeof(struct move));
	if(m10 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m11 = (struct move *) malloc(sizeof(struct move));
	if(m11 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m12 = (struct move *) malloc(sizeof(struct move));
	if(m12 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m13 = (struct move *) malloc(sizeof(struct move));
	if(m13 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m14 = (struct move *) malloc(sizeof(struct move));
	if(m14 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m15 = (struct move *) malloc(sizeof(struct move));
	if(m15 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m16 = (struct move *) malloc(sizeof(struct move));
	if(m16 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m17 = (struct move *) malloc(sizeof(struct move));
	if(m17 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m18 = (struct move *) malloc(sizeof(struct move));
	if(m18 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m19 = (struct move *) malloc(sizeof(struct move));
	if(m19 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m20 = (struct move *) malloc(sizeof(struct move));
	if(m20 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m21 = (struct move *) malloc(sizeof(struct move));
	if(m21 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m22 = (struct move *) malloc(sizeof(struct move));
	if(m22 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m23 = (struct move *) malloc(sizeof(struct move));
	if(m23 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m24 = (struct move *) malloc(sizeof(struct move));
	if(m24 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m25 = (struct move *) malloc(sizeof(struct move));
	if(m25 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m26 = (struct move *) malloc(sizeof(struct move));
	if(m26 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m265 = (struct move *) malloc(sizeof(struct move));
	if(m265 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m27 = (struct move *) malloc(sizeof(struct move));
	if(m27 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m28 = (struct move *) malloc(sizeof(struct move));
	if(m28 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m29 = (struct move *) malloc(sizeof(struct move));
	if(m29 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move *m30 = (struct move *) malloc(sizeof(struct move));
	if(m30 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	m1->next = NULL;
	m2->next = NULL;
	m3->next = NULL;
	m4->next = NULL;
	m5->next = NULL;
	m6->next = NULL;
	m7->next = NULL;
	m8->next = NULL;
	m9->next = NULL;
	m10->next = NULL;
	m11->next = NULL;
	m12->next = NULL;
	m13->next = NULL;
	m14->next = NULL;
	m15->next = NULL;
	m16->next = NULL;
	m17->next = NULL;
	m18->next = NULL;
	m19->next = NULL;
	m20->next = NULL;
	m21->next = NULL;
	m22->next = NULL;
	m23->next = NULL;
	m24->next = NULL;
	m25->next = NULL;
	m26->next = NULL;
	m265->next = NULL;
	m27->next = NULL;
	m28->next = NULL;
	m29->next = NULL;
	m30->next = NULL;
	
	// Allocation des séquences à exécuter dans les mouvements
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
	struct move_seq *seq6 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq6 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq7 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq7 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq8 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq8 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq9 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq9 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq10a = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq10a == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq10b = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq10b == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq11 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq11 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq12 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq12 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq13 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq13 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq14 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq14 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq15 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq15 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq16 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq16 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq17 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq17 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq18 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq18 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq19 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq19 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq20 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq20 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq21 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq21 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq22 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq22 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq23a = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq23a == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq23b = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq23b == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq23c = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq23c == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq23d = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq23d == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq24 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq24 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq25 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq25 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq26 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq26 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq265 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq265 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq27 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq27 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq28 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq28 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq29 = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq29 == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq30a = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq30a == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	struct move_seq *seq30b = (struct move_seq*) malloc(sizeof(struct move_seq));
	if(seq30b == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	
	// Assignation de valeurs aux séquences et aux moves
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

	struct coord old_5 = {5,6};
	struct coord new_5 = {6,5};
	seq5->next = NULL;
	seq5->c_old = old_5;
	seq5->c_new = new_5;
	m5->seq = seq5;

	struct coord old_6 = {4,3};
	struct coord new_6 = {3,4};
	seq6->next = NULL; 
	seq6->c_old = old_6;
	seq6->c_new = new_6;
	m6->seq = seq6;

	struct coord old_7 = {4,7};
	struct coord new_7 = {3,6};
	seq7->next = NULL;
	seq7->c_old = old_7;
	seq7->c_new = new_7;
	m7->seq = seq7;

	struct coord old_8 = {3,4};
	struct coord new_8 = {4,5};
	seq8->next = NULL;
	seq8->c_old = old_8;
	seq8->c_new = new_8;
	m8->seq = seq8;

	struct coord old_9 = {5,8};
	struct coord new_9 = {4,7};
	seq9->next = NULL;
	seq9->c_old = old_9;
	seq9->c_new = new_9;
	m9->seq = seq9;

	struct coord old_10a = {7,4};
	struct coord new_10a = {5,6};
	struct coord piece_taken_5 = {6,5};
	seq10a->next = NULL;
	seq10a->piece_taken = piece_taken_5;
	seq10a->c_old = old_10a;
	seq10a->c_new = new_10a;
	m10->seq = seq10a;

	struct coord old_10b = {5,6};
	struct coord new_10b = {3,4};
	seq10b->c_old = old_10b;
	seq10b->c_new = new_10b;
	seq10b->next = NULL;
	seq10a->next = seq10b;

	struct coord old_11 = {7,4};
	struct coord new_11 = {5,6};
	seq11->next = NULL;
	seq11->piece_taken = piece_taken_5;
	seq11->c_old = old_11;
	seq11->c_new = new_11;
	m11->seq = seq11;

	struct coord old_12 = {3,6};
	struct coord new_12 = {5,4};
	seq12->next = NULL;
	seq12->c_old = old_12;
	seq12->c_new = new_12;
	m12->seq = seq12;
	
	struct coord old_13 = {8,3};
	struct coord new_13 = {7,4};
	seq13->next = NULL;
	seq13->c_old = old_13;
	seq13->c_new = new_13;
	m13->seq = seq13;

	struct coord old_14 = {4,7};
	struct coord new_14 = {3,6};
	seq14->next = NULL;
	seq14->c_old = old_14;
	seq14->c_new = new_14;
	m14->seq = seq14;

	struct coord old_15 = {2,3};
	struct coord new_15 = {3,4};
	seq15->next = NULL;
	seq15->c_old = old_15;
	seq15->c_new = new_15;
	m15->seq = seq15;

	struct coord old_16 = {3,8};
	struct coord new_16 = {4,7};
	seq16->next = NULL;
	seq16->c_old = old_16;
	seq16->c_new = new_16;
	m16->seq = seq16;

	struct coord old_17 = {5,6};
	struct coord new_17 = {3,8};
	seq17->next = NULL;
	seq17->c_old = old_17;
	seq17->c_new = new_17;
	m17->seq = seq17;

	struct coord old_18 = {4,9};
	struct coord new_18 = {5,8};
	seq18->next = NULL;
	seq18->c_old = old_18;
	seq18->c_new = new_18;
	m18->seq = seq18;

	struct coord old_19 = {3,8};
	struct coord new_19 = {4,9};
	seq19->next = NULL;
	seq19->c_old = old_19;
	seq19->c_new = new_19;
	m19->seq = seq19;

	struct coord old_20 = {1,6};
	struct coord new_20 = {2,5};
	seq20->next = NULL;
	seq20->c_old = old_20;
	seq20->c_new = new_20;
	m20->seq = seq20;

	struct coord old_21 = {3,4};
	struct coord new_21 = {4,5};
	seq21->next = NULL;
	seq21->c_old = old_21;
	seq21->c_new = new_21;
	m21->seq = seq21;

	struct coord old_22 = {6,7};
	struct coord new_22 = {5,6};
	seq22->next = NULL;
	seq22->c_old = old_22;
	seq22->c_new = new_22;
	m22->seq = seq22;

	struct coord old_23a = {4,9};
	struct coord new_23a = {1,6};
	seq23a->next = NULL;
	seq23a->c_old = old_23a;
	seq23a->c_new = new_23a;
	m23->seq = seq23a;

	struct coord old_23b = {1,6};
	struct coord new_23b = {4,3};
	seq23a->next = seq23b;
	seq23b->c_old = old_23b;
	seq23b->c_new = new_23b;
	
	struct coord old_23c = {4,3};
	struct coord new_23c = {6,5};
	seq23b->next = seq23c;
	seq23c->c_old = old_23c;
	seq23c->c_new = new_23c;

	struct coord old_23d = {6,5};
	struct coord new_23d = {4,7};
	seq23c->next = seq23d;
	seq23d->c_old = old_23d;
	seq23d->c_new = new_23d;
	seq23d->next = NULL;

	struct coord old_24 = {3,6};
	struct coord new_24 = {2,5};
	seq24->c_old = old_24;
	seq24->c_new = new_24;
	seq24->next = NULL;
	m24->seq = seq24;

	struct coord old_25 = {4,7};
	struct coord new_25 = {6,5};
	seq25->c_old = old_25;
	seq25->c_new = new_25;
	seq25->next = NULL;
	m25->seq = seq25;

	struct coord old_26 = {9,6};
	struct coord new_26 = {8,5};
	seq26->c_old = old_26;
	seq26->c_new = new_26;
	seq26->next = NULL;
	m26->seq = seq26;

	struct coord old_265 = {6,5};
	struct coord new_265 = {4,3};
	seq265->c_old = old_265;
	seq265->c_new = new_265;
	seq265->next = NULL;
	m265->seq = seq265;

	struct coord old_27 = {2,5};
	struct coord new_27 = {1,4};
	seq27->c_old = old_27;
	seq27->c_new = new_27;
	seq27->next = NULL;
	m27->seq = seq27;

	struct coord old_28 = {4,3};
	struct coord new_28 = {1,6};
	seq28->c_old = old_28;
	seq28->c_new = new_28;
	seq28->next = NULL;
	m28->seq = seq28;

	struct coord old_29 = {8,5};
	struct coord new_29 = {6,3};
	seq29->c_old = old_29;
	seq29->c_new = new_29;
	seq29->next = NULL;
	m29->seq = seq29;
	
	struct coord old_30a = {1,6};
	struct coord new_30a = {4,9};
	seq30a->c_old = old_30a;
	seq30a->c_new = new_30a;
	seq30a->next = NULL;
	m30->seq = seq30a;

	struct coord old_30b = {4,9};
	struct coord new_30b = {6,7};
	seq30b->c_old = old_30b;
	seq30b->c_new = new_30b;
	seq30a->next = seq30b;
	

	// Exécution des mouvements à effectuer + vérification des valeurs retournées par apply_move et de cur_player
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_WHITE);
	int m1_check = apply_moves(jeu_1, m1);
	CU_ASSERT_EQUAL(m1_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	int m2_check = apply_moves(jeu_1, m2);
	CU_ASSERT_EQUAL(m2_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_WHITE);
	int m3_check = apply_moves(jeu_1, m3);
	CU_ASSERT_EQUAL(m3_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	int m4_check = apply_moves(jeu_1, m4);
	CU_ASSERT_EQUAL(m4_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_WHITE);
	int m5_check = apply_moves(jeu_1, m5);
	CU_ASSERT_EQUAL(m5_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	int m6_check = apply_moves(jeu_1, m6);
	CU_ASSERT_EQUAL(m6_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_WHITE);
	int m7_check = apply_moves(jeu_1, m7);
	CU_ASSERT_EQUAL(m7_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	int m8_check = apply_moves(jeu_1, m8);
	CU_ASSERT_EQUAL(m8_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_WHITE);
	int m9_check = apply_moves(jeu_1, m9);
	CU_ASSERT_EQUAL(m9_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	//Le move m10 déplace un pion noir pour le faire manger un pion blanc puis un pion noir. Ca doit retorner une erreur.
	int m10_check = apply_moves(jeu_1, m10);
	CU_ASSERT_EQUAL(m10_check, -1);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	int m11_check = apply_moves(jeu_1, m11);
	CU_ASSERT_EQUAL(m11_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_WHITE);
	int m12_check = apply_moves(jeu_1, m12);
	CU_ASSERT_EQUAL(m12_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	int m13_check = apply_moves(jeu_1, m13);
	CU_ASSERT_EQUAL(m13_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_WHITE);
	int m14_check = apply_moves(jeu_1, m14);
	CU_ASSERT_EQUAL(m14_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	int m15_check = apply_moves(jeu_1, m15);
	CU_ASSERT_EQUAL(m15_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_WHITE);
	int m16_check = apply_moves(jeu_1, m16);
	CU_ASSERT_EQUAL(m16_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	int m17_check = apply_moves(jeu_1, m17);
	CU_ASSERT_EQUAL(m17_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_WHITE);
	int m18_check = apply_moves(jeu_1, m18);
	CU_ASSERT_EQUAL(m18_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	int m19_check = apply_moves(jeu_1, m19);
	CU_ASSERT_EQUAL(m19_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_WHITE);
	int m20_check = apply_moves(jeu_1, m20);
	CU_ASSERT_EQUAL(m20_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	int m21_check = apply_moves(jeu_1, m21);
	CU_ASSERT_EQUAL(m21_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_WHITE);
	int m22_check = apply_moves(jeu_1, m22);
	CU_ASSERT_EQUAL(m22_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	int m23_check = apply_moves(jeu_1, m23);
	CU_ASSERT_EQUAL(m23_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_WHITE);
	int m24_check = apply_moves(jeu_1, m24);
	CU_ASSERT_EQUAL(m24_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	int m25_check = apply_moves(jeu_1, m25);
	CU_ASSERT_EQUAL(m25_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_WHITE);
	int m26_check = apply_moves(jeu_1, m26);
	CU_ASSERT_EQUAL(m26_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	int m265_check = apply_moves(jeu_1, m265);
	CU_ASSERT_EQUAL(m265_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_WHITE);
	int m27_check = apply_moves(jeu_1, m27);
	CU_ASSERT_EQUAL(m27_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	int m28_check = apply_moves(jeu_1, m28);
	CU_ASSERT_EQUAL(m28_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_WHITE);
	int m29_check = apply_moves(jeu_1, m29);
	CU_ASSERT_EQUAL(m29_check, 0);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	int m30_check = apply_moves(jeu_1, m30);
	CU_ASSERT_EQUAL(m30_check, -1);
	CU_ASSERT_EQUAL(jeu_1 -> cur_player, PLAYER_BLACK);
	
	// On retire un mouvement de trop, mais cela reste valide
	int check_undo_1 = undo_moves(jeu_1, 100);
	CU_ASSERT_EQUAL(check_undo_1, 0);
	
	// On ne peut pas retirer un nombre négatif de moves
	int check_undo_2 = undo_moves(jeu_1, -1);
	CU_ASSERT_EQUAL(check_undo_2, -1);
	
	// Retirer 0 moves est considéré comme une erreur
	int check_undo_3 = undo_moves(jeu_1, 0);
	CU_ASSERT_EQUAL(check_undo_3, -1);
	
	free(m1);
	free(m2);
	free(m3);
	free(m4);
	free(m5);
	free(m6);
	free(m7);
	free(m8);
	free(m9);
	free(m11);
	free(m12);
	free(m13);
	free(m14);
	free(m15);
	free(m16);
	free(m17);
	free(m18);
	free(m19);
	free(m20);
	free(m21);
	free(m22);
	free(m23);
	free(m24);
	free(m25);
	free(m26);
	free(m27);
	free(m28);
	free(m29);
	free(m30);
	free(seq1);
	free(seq2);
	free(seq3);
	free(seq4);
	free(seq5);
	free(seq6);
	free(seq7);
	free(seq8);
	free(seq9);
	free(seq10a);
	free(seq10b);
	free(seq11);
	free(seq12);
	free(seq13);
	free(seq14);
	free(seq15);
	free(seq16);
	free(seq17);
	free(seq18);
	free(seq19);
	free(seq20);
	free(seq21);
	free(seq22);
	free(seq23a);
	free(seq23b);
	free(seq23c);
	free(seq23d);
	free(seq24);
	free(seq25);
	free(seq26);
	free(seq27);
	free(seq28);
	free(seq29);
	free(seq30a);
	free(seq30b);
	free_game(jeu_1);
}

void test_print_board(){
	struct game *jeu_1 = new_game(10, 10);
	
	print_board(jeu_1);
	CU_PASS("Plateau imprimé");
	
	// print_board(NULL);
	// CU_PASS("Message d'erreur imprimé");
	
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
		(NULL == CU_add_test(pSuite, "test de test_undo_moves()", test_undo_moves)) ||
		(NULL == CU_add_test(pSuite, "test de test_undo_moves()", test_partie)) ||
		(NULL == CU_add_test(pSuite, "test de test_print_board()", test_print_board))){
		
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
