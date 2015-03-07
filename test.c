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
	print_board(jeu_1);
	int m1_check = apply_moves(jeu_1, m1);
	CU_ASSERT_EQUAL(m1_check, 0);
	printf("%d\n", m1_check);
	print_board(jeu_1);
	
	// Un double pointeur non constant ne peut pas être utilisé en tant qu'argument constant
	const int **plateau = (const int **) malloc(10*sizeof(int *));
	if(plateau == NULL){
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	
	plateau = (const int**) jeu_1->board;
	struct game *jeu_2 = load_game(10, 10, plateau, jeu_1->cur_player);
	CU_ASSERT_PTR_NOT_NULL(jeu_2);
	/*struct game *jeu_3 = load_game(-1, 100, &plateau, jeu_1->cur_player);
	CU_ASSERT_PTR_NULL(jeu_3);
	
	struct game *jeu_4 = load_game(10, 10, NULL, jeu_1->cur_player);
	CU_ASSERT_PTR_NULL(jeu_4);*/
	
	free_game(jeu_1);
	free_game(jeu_2);
	//free_game(jeu_3);
	//free_game(jeu_4);
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
		(NULL == CU_add_test(pSuite, "test de test_load_game()", test_load_game))){
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	// Exécution des tests
	CU_basic_run_tests();
	
	// Afficher le rapport des tests qui ont échoué
	// CU_basic_show_failures(CU_get_failure_list());
	
	// Nécessaire à la fin : libération des ressources utilisées par les tests
	CU_cleanup_registry();
	printf("Tests CUnit terminés !\n");
	return CU_get_error();
}
