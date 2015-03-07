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
	if(head == NULL || tail == NULL){
		return;
	}
	struct move_seq *precedent = head;
	head = head -> next;
	while(head != tail){
		free(precedent);
		precedent = head;
		head = head -> next;
	}
	free(precedent);
	free(head);
	head = NULL;
	tail = NULL;
}



int isCorrectInput(char *input){
    if(input == NULL){
        return 0;
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
                return 0;
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
            return 1;
        }
        else if(input[j] != ' '){
			printf("erreur2 : %c\n", input[j]);
            return 0;
        }
        j++;
        for(i = 1 ; i < longueurPattern ; i++){
            if(input[j] != pattern[i]){
				printf("erreur3 : %c\n", input[j]);
                return 0;
            }
            j++;
        }
    }
    return 1;
}

int getSequence(char *chaine){
    fgets(chaine, 100, stdin);
	printf("entre : %s\n", chaine);
    while(!isCorrectInput(chaine)){
		if(strcmp(chaine, "end\n") == 0){
			return -1;
		}
        printf("Vous n'avez malheureusement pas entre une commande valide...\n");
        return getSequence(chaine);
    }
	return 0;
	
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
	while(jouer != -1){
    	print_board(jeu);
    	printf("Quel pion deplacer ? ('(x1,y1) to (x2,y2)')\n");
		if(getSequence(input) == -1){
			jouer = -1;
		}
		else{
			mouvement.next = NULL;
			mouvement.seq = head;
			apply_moves(jeu, &mouvement);
			free_queue();
		}
	}
    return EXIT_SUCCESS;
}





