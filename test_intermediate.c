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
		(NULL == CU_add_test(pSuite, "test de test_isOutOfBoard()", test_isOutOfBoard))){
		
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
