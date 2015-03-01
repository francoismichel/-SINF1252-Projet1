#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <stdio.h>
#include "dames.h"

// Test de la fonction newgame
void test_newgame(){
	struct game *jeu = new_game(10,10);
	CU_ASSERT_PTR_NOT_NULL(jeu);
	CU_ASSERT_PTR_NOT_NULL(jeu -> board);
	int i;
	int j;
	for(i = 0 ; i < 10 ; i++){
		CU_ASSERT_PTR_NOT_NULL(*(jeu -> board) + i);
		for(j = 0 ; j < 10 ; j++){
			if (!( *(*((jeu -> board) + i) + j) == 0x1 || *(*((jeu -> board) + i) + j) == 0x5 || *(*((jeu -> board) + i) + j) == 0x0)){
				CU_FAIL("Case du damier mal initailisée\n");
			}
			else CU_PASS("Case du damier initailisée\n");
		}
	}
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
	if(NULL == CU_add_test(pSuite, "test of test_newgame()", test_newgame)){
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	// Exécution des tests
	CU_basic_run_tests();
	
	// Afficher le rapport des tests qui ont échoué
	CU_basic_show_failures(CU_get_failure_list());
	
	// Nécessaire à la fin : libération des ressources utilisées par les tests
	CU_cleanup_registry();
	printf("Test reussi !\n");
	return CU_get_error();
}
