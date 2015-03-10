#define NORDOUEST 1
#define NORDEST 2
#define SUDOUEST 3
#define SUDEST 4

int nPieces[2];

/*
 * retourne la couleur de la piece en argument (PLAYER_WHITE ou PLAYER_BLACK)
 * retourne 2 si la case est une case vide
 * lance une erreur si la pièce n'est pas valide
 */
int getColor(int piece);

/*
 * Retourne 1 si la séquence est à/mène vers une position hors du plateau de jeu.
 * Retourne 0 sinon.
 * @seq est une séquence valide
 */
int isOutOfBoard(const struct move_seq *seq);

/*
 * Retourne 0 si la pièce n'est pas une dame, 1 si ce n'est pas une dame
 * @piece est une pièce valide
 */
int isDame(int piece);

/*
 * Retourne 1 si plateau[x][y] existe
 * Retourne 0 sinon
 */
int isCoordInBoard(int x, int y);

/*
 * Retourne 1 si la pièce située aux coordonnées (x,y) peut effectuer un mouvement au tour actuel
 * Retourne 0 sinon
 */
int isValidMovePiece(const struct game *jeu, int x, int y, int color);

/*
 * Retourne 1 si le joueur @color a la possibilité de jouer au moins une pièce
 * Retourne 0 sinon
 */
int canPlay(const struct game *jeu, int color);

/*
 * Retourne la direction définie par les 2 coordonnées @c_avant et @c_apres
 * Retourne 0 si les deux coordonnées ne décrivent pas une diagonale
 * @c_avant et @c_apres sont des coordonnées valides
 */
int getDiagonal(struct coord c_avant, struct coord c_apres);

/*
 * Retourne 0 si la prise de la pièce n'est pas valide, retourne 1 si elle l'est
 * On considère que la prise est valide si :
 * - la pièce qui prend et la pièce prise sont de couleurs différentes
 * - la pièce qui prend a sauté au-dessus de la pièce prise :
 * 		cela signifie que les coordonnées de la pièce prise sont les moyennes des coordonnées de la pièce qui joue,
 *		avant et après avoir joué, si la pièce qui joue est un pion.
 * @piece est une pièce valide
 */
int pieceBienPrise(const struct game *jeu, struct coord *prise, struct coord c_avant, struct coord c_apres);

/*
 * Retourne la distance diagonale parcourue entre @c_avant et @c_apres.
 * Sinon, retourne 0 (@c_avant et @c_apres ne forment pas un déplacement diagonal)
 * @c_avant et @c_apres sont des coordonnées valides
 */
int isDiagonal(struct coord c_avant, struct coord c_apres);

/*
 * Retourne 1 si le déplacement du pion de @c_avant à @c_apres est correct.
 * Retourne 2 si le pion a bien capturé une pièce (dont les coordonnées sont enregistrées dans @taken)
 * Retourne 0 sinon
 */
int isCorrectMovePion(const struct game *jeu, struct coord c_avant, struct coord c_apres, struct coord *taken);

/*
 * Retourne 1 si le déplacement de la dame de @c_avant à @c_apres est correct (déplacement simple)
 * Retourne 2 si la dame a bien capturé une pièce (dont les coordonnées sont enregistrées dans @taken)
 * Retourne 0 sinon (déplacement incorrect)
 */
int isCorrectMoveDame(const struct game *jeu, struct coord c_avant, struct coord c_apres, struct coord *taken);

/*
 * Retourne 1 si le déplacement de la @piece de @c_avant à @c_apres est correct (déplacement simple)
 * Retourne 2 si la @piece a capturé une pièce (dont les coordonnées sont enregistrées dans @taken)
 * Retourne 0 sinon (déplacement incorrect)
 */
int isMoveValid(const struct game *jeu, struct coord c_avant, struct coord c_apres, int piece, struct coord *taken);

/*
 * Ajoute une séquence dans le move en tête de pile du jeu
 */ 
void push_seq(struct game *jeu, const struct move_seq *seq, struct coord *piece_taken, int old_orig, int piece_value);

/*
 * Transforme un pion en dame si nécessaire.
 * Retourne 1 si le pion a été transformé en dame, retourne 0 sinon.
 */
int transformDame(struct game *jeu, struct coord c);

/*
 * Ajoute un move vide en tête de pile du jeu
 */
void push_move(struct game *jeu);

/*
 * Libère les ressources allouées à une séquence d'un move
 */
void free_move_seq(struct move_seq * seq);

/*
 * Supprime un move en tête de pile du jeu, et le retourne.
 */
struct move *pop_move(struct game *jeu);

/*
 * Applique rétroactivement la séquence sur le jeu. Retourne -1 si jeu ou séquence est NULL, retourne 0 si tout s'est bien passé.
 */
int undo_seq(struct game *jeu, struct move_seq *sequence);
