#define NORDOUEST 1
#define NORDEST 2
#define SUDOUEST 3
#define SUDEST 4

int nPieces[2];

/*
 * Retourne la couleur de la pièce @pièce
 */
int getColor(int piece);

/*
 * Retourne 1 si la séquence mène vers une position hors du plateau de jeu. Retourne 0 sinon.
 * @seq est une séquence valide
 */
int isOutOfBoard(const struct move_seq *seq);

/*
 * Retourne 0 si la pièce n'est pas une dame, !0 si c'en est une
 * @piece est une pièce valide
 */
int isDame(int piece);

/*
 * Retourne la direction définie par les 2 coordonnée @c_avant et @c_apres
 * @c_avant et @c_apres sont des coordonnées valides
 */
int getDiagonal(struct coord c_avant, struct coord c_apres);

/*
 * Retourne 0 si la prise de la pièce n'est pas valide, retourne !0 si elle l'est
 * On considère que la prise est valide si :
 * - la pièce qui prend et la pièce prise sont de couleurs différentes
 * - la pièce qui prend a sauté au-dessus de la pièce prise :
 * 		cela signifie que les coordonnées de la pièce prise sont les moyennes des coordonnées de la pièce qui joue,
 *		avant et après avoir joué, si la piece qui joue est un pion.
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
 * Retourne 0 si le déplacement du pion de @c_avant à @c_apres est correct.
 * Retourne 1 sinon
 */
int isCorrectMovePion(const struct game *jeu, struct coord c_avant, struct coord c_apres, struct coord *taken);

/*
 * Retourne 0 si le déplacement de la dame de @c_avant à @c_apres est correct.
 * Retourne 1 sinon
 */
int isCorrectMoveDame(const struct game *jeu, struct coord c_avant, struct coord c_apres, struct coord *taken);

/*
 * Retourne 0 si le déplacement de la @piece de @c_avant à @c_apres est correct.
 * Retourne 1 sinon
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
