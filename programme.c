#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dames.h"

struct move_seq *head;	//Tête de la file globale de move_seq
struct move_seq *tail;	//Queue de la file globale de move_seq

/*
 * Rajoute la séquence composée des deux coordonnées dans la file globale de move_seq
 * oldc et newc sont des struct coord valides
 */
void enqueue(struct coord oldc, struct coord newc){
    if(head == NULL && tail == NULL){   //Cas d'une file vide
        struct move_seq *newNode = (struct move_seq *) malloc(sizeof(struct move_seq));	//Création du 1er élément de la file
        newNode -> c_old = oldc;	//On assigne les valeurs au noeud
        newNode -> c_new = newc;
        newNode -> next = NULL;		//On signale qu'il n'y a pas d'élément suivant celui-ci
        head = newNode;			//Cet élément est la tête et la queue de la file
        tail = newNode;
    }
    else{				//Cas d'une file non vide
        struct move_seq *newNode = (struct move_seq *) malloc(sizeof(struct move_seq));	//Création du nouvel élément de la file
        newNode -> c_old = oldc;	//On assigne les valeurs au noeud
        newNode -> c_new = newc;	
        newNode -> next = NULL;		//On signale qu'il n'y a pas d'élément après celui-ci
        tail -> next = newNode;		//On accroche ce noeud à la queue de la file
        tail = newNode;			//Ce noeud devient la nouvelle queue
    }
}

/*
 * Vide entièrement la file globale de move_seq
 */
void free_queue(){
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

/*
 * Retourne n > 0 si la séquence de caractères input est égale à "undo n" et que n est un entier strictement positif
 * Retourne 0 sinon
 */
int isUndo(char *input){
	if(input == NULL){	//Cas du pointeur NULL
		return 0;
	}
	char *undo = "undo ";	//Chaine de caractères de référence
	int longueurUndo = strlen(undo);	
	if(strlen(input) <= longueurUndo){	//On vérifie d'abord si les deux chaines ont la même longueur
		return 0;
	}
	int i;
	for(i = 0 ; i < longueurUndo ; i++){	//Pour tous les caractères de la chaine
		if(input[i] != undo[i]){	//Si un des caractère diffère de la chaine de référence
			return 0;		//On retourne 0
		}
	}
	char *res = NULL;
	int n = (int) strtol(input + longueurUndo, &res, 10);	//On transforme le chiffre en int
	if(n <= 0 || res == input + longueurUndo){	//Si n est négatif ou =0, ou que res pointe vers la suite de "undo " dans input
		return 0;				//On retourne 0
	}
	else{						//Sinon
		return n;				//On retourne n, le nombre de undo à opérer
	}
}

/*
 * retourne 0 si l'input est correct, et crée une liste chainée de séquences à appliquer sur le jeu.
 * retourne -1 si l'input est incorrect
 * retourne -2 si l'utilisateur demande de quitter le jeu
 * retourne un nombre n strictement positif s'il y a n moves à annuler.
 */
int isCorrectInput(char *input){
    if(input == NULL){	//Cas du pointeur NULL
        return -1;
    }
	if(strcmp(input, "exit\n") == 0){	//Si l'input est "exit"
		return -2;			//On retourne -2
	}
	int n = isUndo(input);			//On stocke le nombre de Undo à faire, s'il y en a
	if(n > 0){	// s'il y a des coups à undo, le signale.
		return n;
	}
    char chaine[] = "(x,y) to (x,y)";	//Initialisation des patterns utilisés pour le parsing d'input
	char pattern[] = " , ";
    int longueur = strlen(chaine);
    int longueurPattern = strlen(" , ");
    void *poubelle = NULL;
    int i = 0;		//Variables de boucle for
    int j = 0;
    int chiffre = 0;//Variable stockant un entier, récupéré d'input
    struct coord c1, c2;
    while(input[i] != '\n'){	//Tant qu'on est pas à la fin de la chaine
        for(i = 0 ; i < longueur ; i++){	//Pour tous les caracteres du premier pattern
            if(input[j] != chaine[i] && i != 1 && i != 10 && i != 3 && i != 12){	//Si input ne correspond pas au pattern
																					//(excepté à l'endroit de x et y)
				printf("erreur1 : %c\n", input[j]);				//Il y a une erreur, on l'affiche
                return -1;										//On le fait remarquer en retournant -1
            }
            if(i == 3){											//Si on est à la place du premier y
                chiffre = (int) strtol(&input[j - 2], poubelle, 10);	//On récupère la valeur entière du x dans l'input
                c1.x = chiffre;
                chiffre = (int) strtol(&input[j], poubelle, 10);		//On récupère la valeur entière du y dans l'input
                c1.y = chiffre;
            }
            else if(i == 12){									//Si on est à la place du deuxième y
                chiffre = (int) strtol(&input[j - 2], poubelle, 10);	//On récupère la valeur entière du x dans l'input
                c2.x = chiffre;
                chiffre = (int) strtol(&input[j], poubelle, 10);		//On récupère la valeur entière du y dans l'input
                c2.y = chiffre;
                enqueue(c1, c2);
            }
            j++;
        }
        if(input[j] == '\n'){	//Si on est à la fin de l'input
            return 0;			//On retourne 0
        }
        else if(input[j] != ' '){	//Si le pattern n'est pas respecté
			printf("erreur2 : %c\n", input[j]);	//On affiche une erreur
            return -1;
        }
        j++;
        for(i = 1 ; i < longueurPattern ; i++){	//On vérifie le respect du pattern
            if(input[j] != pattern[i]){			//S'il n'est pas respecté
				printf("erreur3 : %c\n", input[j]);	//On affiche une erreur
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
    fgets(chaine, 100, stdin);	//On récupère l'input avec une taille limite de 100
	int n = isCorrectInput(chaine);	//On récupère la valeur renvoyée par la fonction isCorrectInput
    while(n == -1){					//Tant que l'input rentré est incorrect
        printf("Vous n'avez malheureusement pas entre une commande valide...\n");
        return getSequence(chaine);
    }
	return n;
	
}

/*
 * Fonction main, qui exécute le programme principal
 */
int main(int argc, char *argv[]){
    char input[100];	
    struct game *jeu = new_game(10,10);	//On crée un nouveau jeu
	int jouer = 0;	//Vaudra -1 quand il faudra arrêter le jeu
	struct move mouvement;
	int n = 0;
	int undo = 0;
	int apply = 0;
	while(jouer != -1){
    	print_board(jeu);	//On affiche le jeu
    	printf("Quel pion déplacer ? ('(x1,y1) to (x2,y2) , (x2,y2) to ...')\n");	//Commandes possibles pour l'utilisateur
		printf("taper 'exit' pour quitter le jeu\n");
		printf("taper 'undo n' pour annuler n mouvements de joueurs\n");
		n = getSequence(input);		//On récupère le résultat de getSequence
		if(n == -2){				//Si on doit arrêter le jeu
			jouer = -1;
		}
		else if(n == 0){			//Si l'action à effectuer est un mouvement
			mouvement.next = NULL;
			mouvement.seq = head;
			apply = apply_moves(jeu, &mouvement);	//On applique le mouvement
			free_queue();							//On vide la file globale de move_seq
			if(apply == 1){							//Si la partie est terminée
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
			else if(apply == -1){					//Si le mouvement n'a pas pu être appliqué
				printf("Impossible d'appliquer ce mouvement : mouvement invalide.\n");
			}
		}
		else{							//Si l'action à effectuer est un undo
			undo = undo_moves(jeu, n);	//On l'applique
			if(undo != 0){				//Si undo s'est mal passée
				printf("Impossible d'annuler ces mouvements...\n");
				undo = 0;
			}
		}
	}
    return EXIT_SUCCESS;
}





