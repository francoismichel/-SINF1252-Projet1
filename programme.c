#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dames.h"

struct move_seq *head;	// Tête de la file globale de move_seq
struct move_seq *tail;	// Queue de la file globale de move_seq

/*
 * Rajoute la séquence composée des deux coordonnées dans la file globale de move_seq
 * @oldc et @newc sont des struct coord valides
 */
void enqueue(struct coord oldc, struct coord newc){
	// Cas d'une file vide
    if(head == NULL && tail == NULL){
        struct move_seq *newNode = (struct move_seq *) malloc(sizeof(struct move_seq));	// Création du 1er élément de la file
        newNode -> c_old = oldc;	// On assigne les valeurs au noeud
        newNode -> c_new = newc;
        newNode -> next = NULL;		// On signale qu'il n'y a pas d'élément suivant celui-ci
        head = newNode;			// Cet élément est la tête et la queue de la file
        tail = newNode;
    }
    // Cas d'une file non vide
    else{
        struct move_seq *newNode = (struct move_seq *) malloc(sizeof(struct move_seq));	// Création du nouvel élément de la file
        newNode -> c_old = oldc;	// On assigne les valeurs au noeud
        newNode -> c_new = newc;	
        newNode -> next = NULL;		// On signale qu'il n'y a pas d'élément après celui-ci
        tail -> next = newNode;		// On accroche ce noeud à la queue de la file
        tail = newNode;			// Ce noeud devient la nouvelle queue
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
	// Cas du pointeur NULL
	if(input == NULL){
		return 0;
	}
	char *undo = "undo ";	// Chaine de caractères de référence
	int longueurUndo = strlen(undo);
	//On vérifie d'abord si les deux chaines ont la même longueur
	if(strlen(input) <= longueurUndo){
		return 0;
	}
	int i;
	// Pour tous les caractères de la chaine
	for(i = 0 ; i < longueurUndo ; i++){
		// Si un des caractère diffère de la chaine de référence
		if(input[i] != undo[i]){
			// On retourne 0
			return 0;
		}
	}
	char *res = NULL;
	// On transforme le chiffre en int
	int n = (int) strtol(input + longueurUndo, &res, 10);
	// Si n est négatif ou =0, ou que res pointe vers la suite de "undo " dans input
	if(n <= 0 || res == input + longueurUndo){
		//On retourne 0
		return 0;
	}
	// Sinon
	else{
		// On retourne n, le nombre de undo à effectuer
		return n;
	}
}

/*
 * Retourne 0 si l'input est correct, et crée une liste chainée de séquences à appliquer sur le jeu.
 * Retourne -1 si l'input est incorrect
 * Retourne -2 si l'utilisateur demande de quitter le jeu
 * Retourne un nombre n strictement positif s'il y a n moves à annuler.
 */
int isCorrectInput(char *input){
	// Cas du pointeur NULL
    if(input == NULL){
        return -1;
    }
    // Si l'input est "exit"
	if(strcmp(input, "exit\n") == 0){
		// On retourne -2
		return -2;
	}
	int n = isUndo(input);	// On stocke le nombre de Undo à faire, s'il y en a
	// S'il y a des coups à undo, le signale.
	if(n > 0){
		return n;
	}
    char chaine[] = "(x,y) to (x,y)";	// Initialisation des patterns utilisés pour le parsing d'input
	char pattern[] = " , ";
    int longueur = strlen(chaine);
    int longueurPattern = strlen(" , ");
    void *poubelle = NULL;
    int i = 0;		// Variables de boucle for
    int j = 0;
    int chiffre = 0; // Variable stockant un entier, récupéré d'input
    struct coord c1, c2;
    // Tant qu'on est pas à la fin de la chaîne
    while(input[i] != '\n'){
		//Pour tous les caracteres du premier pattern
        for(i = 0 ; i < longueur ; i++){
			// Si input ne correspond pas au pattern
			// (excepté à l'endroit de x et y)
            if(input[j] != chaine[i] && i != 1 && i != 10 && i != 3 && i != 12){
				// Il y a une erreur, on l'affiche
				printf("erreur de caractère : %c\n", input[j]);
				// On le fait remarquer en retournant -1
                return -1;
            }
            // Si on est à la place du premier y
            if(i == 3){
				// On récupère la valeur entière du x dans l'input
                chiffre = (int) strtol(&input[j - 2], poubelle, 10);
                c1.x = chiffre;
                // On récupère la valeur entière du y dans l'input
                chiffre = (int) strtol(&input[j], poubelle, 10);
                c1.y = chiffre;
            }
            // Si on est à la place du deuxième y
            else if(i == 12){
				//On récupère la valeur entière du x dans l'input
                chiffre = (int) strtol(&input[j - 2], poubelle, 10);
                c2.x = chiffre;
                // On récupère la valeur entière du y dans l'input
                chiffre = (int) strtol(&input[j], poubelle, 10);
                c2.y = chiffre;
                enqueue(c1, c2);
            }
            j++;
        }
        // Si on est à la fin de l'input
        if(input[j] == '\n'){
			// On retourne 0
            return 0;
        }
        // Si le pattern n'est pas respecté
        else if(input[j] != ' '){
			// On affiche une erreur
			printf("Erreur de caractère : %c\n", input[j]);
            return -1;
        }
        j++;
        // On vérifie le respect du pattern
        for(i = 1 ; i < longueurPattern ; i++){
			// S'il n'est pas respecté
            if(input[j] != pattern[i]){
				// On affiche une erreur
				printf("Erreur de caractère : %c\n", input[j]);
                return -1;
            }
            j++;
        }
    }
    return 0;
}

/*
 * Récupère la séquence à jouer et la met dans une liste chainée de move_seq, prête a être appliquée au jeu
 * Retourne -2 s'il faut quitter le jeu
 */
int getSequence(char *chaine){
    fgets(chaine, 100, stdin);	//On récupère l'input avec une taille limite de 100
	int n = isCorrectInput(chaine);	//On récupère la valeur renvoyée par la fonction isCorrectInput
    while(n == -1){					//Tant que l'input rentré est incorrect
        printf("\nVous n'avez pas entré une commande valide...\n");
        return getSequence(chaine);
    }
	return n;
}

/*
 * Fonction main, qui exécute le programme principal
 */
int main(int argc, char *argv[]){
    char input[100];
    // On crée un nouveau jeu
    struct game *jeu = new_game(10,10);
	int jouer = 0;	// Vaudra -1 quand il faudra arrêter le jeu
	struct move mouvement;
	int n = 0;
	int undo = 0;
	int apply = 0;
	while(jouer != -1){
		// On affiche le jeu
    	print_board(jeu);
    	printf("Quel pion déplacer ? ('(x1,y1) to (x2,y2) , (x2,y2) to ...')\n");	//Commandes possibles pour l'utilisateur
		printf("Entrer 'exit' pour quitter le jeu\n");
		printf("Entrer 'undo n' pour annuler n mouvements de joueurs\n");
		// On récupère le résultat de getSequence
		n = getSequence(input);
		// Si on doit arrêter le jeu
		if(n == -2){
			jouer = -1;
		}
		// Si l'action à effectuer est un mouvement
		else if(n == 0){
			mouvement.next = NULL;
			mouvement.seq = head;
			// On applique le mouvement
			apply = apply_moves(jeu, &mouvement);
			// On vide la file globale de move_seq
			free_queue();
			// Si la partie est terminée
			if(apply == 1){
				if(jeu -> cur_player == PLAYER_WHITE){
					printf("\nLes blancs ont gagné !\n");
				}	
				else if(jeu -> cur_player == PLAYER_BLACK){
					printf("\nLes noirs ont gagné !\n");
				}
				else{								
					printf("\nJoueur gagnant indéterminé !\n");
				}
			}
			// Si le mouvement n'a pas pu être appliqué
			else if(apply == -1){
				printf("\nImpossible d'appliquer ce mouvement : mouvement invalide.\n");
			}
		}
		// Si l'action à effectuer est un undo
		else{
			// On l'applique
			undo = undo_moves(jeu, n);
			// Si undo s'est mal passée
			if(undo != 0){
				printf("\nImpossible d'annuler ces mouvements...\n");
				undo = 0;
			}
		}
	}
	free_game(jeu);
    return EXIT_SUCCESS;
}
