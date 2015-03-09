#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dames.h"

struct move_seq *head;
struct move_seq *tail;

void enqueue(struct coord oldc, struct coord newc){
    if(head == NULL && tail == NULL){   //cas d'une file vide
        struct move_seq *newNode = (struct move_seq *) malloc(sizeof(struct move_seq));
        newNode -> c_old = oldc;
        newNode -> c_new = newc;
        newNode -> next = NULL;
        head = newNode;
        tail = newNode;
    }
    else{
        struct move_seq *newNode = (struct move_seq *) malloc(sizeof(struct move_seq));
        newNode -> c_old = oldc;
        newNode -> c_new = newc;
        newNode -> next = NULL;
        tail -> next = newNode;
        tail = newNode;
    }
}

void free_queue(){
	printf("coucou1\n");
	if(head == NULL || tail == NULL){
		return;
	}
	struct move_seq *precedent = head;
	struct move_seq *current = head -> next;
	while(current != NULL){
		free(precedent);
		precedent = current;
		current = current -> next;
	}
	free(precedent);
	head = NULL;
	tail = NULL;
}

int isUndo(char *input){
	if(input == NULL){
		return 0;
	}
	char *undo = "undo ";
	int longueurUndo = strlen(undo);
	if(strlen(input) <= longueurUndo){
		return 0;
	}
	int i;
	for(i = 0 ; i < longueurUndo ; i++){
		if(input[i] != undo[i]){
			return 0;
		}
	}
	char *res = NULL;
	int n = (int) strtol(input + longueurUndo, &res, 10);
	if(n <= 0 || res == input + longueurUndo){
		return 0;
	}
	else{
		return n;
	}
}

/*
 * retourne 0 si l'input est correct, et crée une liste chainée de séquences à appliquer sur le jeu.
 * retourne -1 si l'input est incorrect
 * retourne -2 si l'utilisateur demande de quitter le jeu
 * retourne un nombre n strictement positif s'il y a n moves à annuler.
 */
int isCorrectInput(char *input){
    if(input == NULL){
        return -1;
    }
	if(strcmp(input, "exit\n") == 0){
		return -2;
	}
	int n = isUndo(input);
	if(n > 0){	// s'il y a des coups à annuler, le signale.
		return n;
	}
    char chaine[] = "(x,y) to (x,y)";
	char pattern[] = " , ";
    int longueur = strlen(chaine);
    int longueurPattern = strlen(" , ");
    void *poubelle = NULL;
    int i = 0;
    int j = 0;
    int chiffre = 0;
    struct coord c1, c2;
    while(input[i] != '\n'){
        for(i = 0 ; i < longueur ; i++){
            if(input[j] != chaine[i] && i != 1 && i != 10 && i != 3 && i != 12){
				printf("erreur1 : %c\n", input[j]);
                return -1;
            }
            if(i == 3){
                chiffre = (int) strtol(&input[j - 2], poubelle, 10);
                c1.x = chiffre;
                chiffre = (int) strtol(&input[j], poubelle, 10);
                c1.y = chiffre;
            }
            else if(i == 12){
                chiffre = (int) strtol(&input[j - 2], poubelle, 10);
                c2.x = chiffre;
                chiffre = (int) strtol(&input[j], poubelle, 10);
                c2.y = chiffre;
                enqueue(c1, c2);
            }
            j++;
        }
        if(input[j] == '\n'){
            return 0;
        }
        else if(input[j] != ' '){
			printf("erreur2 : %c\n", input[j]);
            return -1;
        }
        j++;
        for(i = 1 ; i < longueurPattern ; i++){
            if(input[j] != pattern[i]){
				printf("erreur3 : %c\n", input[j]);
                return -1;
            }
            j++;
        }
    }
    return 0;
}
/*
 * récupère la séquence à jouer et la met dans une liste chainée de move_seq, prête a etre appliquée au jeu
 * retourne -2 s'il faut quitter le jeu
 */
int getSequence(char *chaine){
    fgets(chaine, 100, stdin);
	//printf("entre : %s\n", chaine);
	int n = isCorrectInput(chaine);
    while(n == -1){
		if(n == -2){
			return -2;
		}
        printf("Vous n'avez malheureusement pas entre une commande valide...\n");
        return getSequence(chaine);
    }
	return n;
	
}

int main(int argc, char *argv[]){
    /*char chaine[6] = "";
    scanf("%s", chaine);
    printf("%s, %d\n", chaine, strlen(chaine));
    return 0;
    */
    char input[100];
    struct game *jeu = new_game(10,10);
	int jouer = 0;
	struct move mouvement;
	int n = 0;
	int undo = 0;
	int apply = 0;
	while(jouer != -1){
    	print_board(jeu);
    	printf("Quel pion déplacer ? ('(x1,y1) to (x2,y2) , (x3,y3) to ...')\n");
		printf("taper 'exit' pour quitter le jeu\n");
		printf("taper 'undo n' pour annuler n mouvements de joueurs\n");
		n = getSequence(input);
		if(n == -2){
			jouer = -1;
		}
		else if(n == 0){
			mouvement.next = NULL;
			mouvement.seq = head;
			apply = apply_moves(jeu, &mouvement);
			free_queue();
			if(apply == 1){
				if(jeu -> cur_player == PLAYER_WHITE){
					printf("Les blancs ont gagné !\n");
				}	
				else if(jeu -> cur_player == PLAYER_BLACK){
					printf("Les noirs ont gagné !\n");
				}
				else{
					printf("On sait pas qui a gagné !\n");
				}
			}
			else if(apply == -1){
				printf("Impossible d'appliquer ce mouvement : mouvement invalide.\n");
			}
		}
		else{
			
			undo = undo_moves(jeu, n);
			if(undo != 0){
				printf("Impossible d'annuler ces mouvements...\n");
				undo = 0;
			}
		}
	}
    return EXIT_SUCCESS;
}





