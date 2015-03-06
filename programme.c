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



int isCorrectInput(char *input){
    if(input == NULL){
        return 1;
    }
    char chaine[] = "(x,y) to (x,y)";
    int longueur = strlen(chaine);
    int longueurPattern = strlen(" , ");
    if(strlen(input) != longueur){
        return 1;
    }
    void *poubelle = NULL;
    int i = 0;
    int j = 0;
    int counter = 0;
    int chiffre = 0;
    struct coord c1, c2;
    while(input[i] != '\0'){
        for(i = 0 ; i < longueur ; i++){
            if(input[j] != chaine[i]){
                return 1;
            }
            if(i == 8){
                chiffre = (int) strtol(&input[j - 2], poubelle, 10);
                c1.x = chiffre;
                chiffre = (int) strtol(&input[j], poubelle, 10);
                c1.y = chiffre;
            }
            else if(i == 17){
                chiffre = (int) strtol(&input[j - 2], poubelle, 10);
                c2.x = chiffre;
                chiffre = (int) strtol(&input[j], poubelle, 10);
                c2.y = chiffre;
                enqueue(c1, c2);
            }
            j++;
        }
        if(input[j] == '\0'){
            return 0;
        }
        else if(input[j] != " "){
            return 1;
        }
        j++;
        for(i = 1 ; i < longueur ; i++){
            if(input[j] != chaine[i]){
                return 1;
            }
            j++;
        }
        counter ++;
    }
    return 1;
}

/*struct move_seq getSequence(){
    char chaine[100];
    fgets(chaine, 20, stdin);
    while(!isCorrectInput(chaine)){
        printf("Vous n'avez malheureusement pas entre une commande valide...\n");
        return getSequence();
    }
    char **poubelle = NULL;
    struct coord c1;
    long chiffre = strtol(&chaine[6], poubelle, 10);
    c1.x = chiffre;
    chiffre = strtol(&chaine[8], poubelle, 10);
    c1.y = chiffre;
    struct coord c2;
    chiffre = strtol(&chaine[15], poubelle, 10);
    c2.x = chiffre;
    chiffre = strtol(&chaine[17], poubelle, 10);
    c2.y = chiffre;
}*/

int main(int argc, char *argv[]){
    /*char chaine[6] = "";
    scanf("%s", chaine);
    printf("%s, %d\n", chaine, strlen(chaine));
    return 0;
    */
    char input[20];
    struct game *jeu = new_game(10,10);
    print_board(jeu);
    printf("Quel pion deplacer ? ('move (x1,y1) to (x2,y2)')\n");
    return EXIT_SUCCESS;
}





