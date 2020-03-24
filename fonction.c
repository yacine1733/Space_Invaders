#include "vt100.h"
#include "serial.h"
#include "fonction.h"


//structure pour coordonnées en x,y ainsi que l'état mort ou vivant des vaisseaux et des missiles
typedef struct {
	unsigned char vx, vy, etat;
} mechant_t;
mechant_t mechant[6];
mechant_t missile[1];
mechant_t vaisseau[1];


//fonction qui nous permet de créé les bordures d'une fenetre avec les caractère de notre choix
void fenetre(unsigned char character1, unsigned char character2) {

	unsigned char i;

	for (i = 1; i < 90; i++) {
		vt100_move(i, 1);
		serial_putchar(character1);
		vt100_move(i, 29);
		serial_putchar(character1);
	}
	for (i = 1; i < 30; i++) {
		vt100_move(1, i);
		serial_putchar(character2);
		vt100_move(90, i);
		serial_putchar(character2);
	}
}

//Première fenêtre afficher du jeu c'est l'accueil du jeu
void menu_accueil(void) {
	fenetre('#', '#');
	vt100_move(40, 10);
	serial_puts("Space Invader");
	vt100_move(45, 15);
	serial_puts("PLAY");
	vt100_move(65, 26);
	serial_puts("By Yacine Hamida");
	vt100_move(35, 20);
	serial_puts("Appuyer sur une touche");
	appuye_touche();
}

//fonction qui detecte l'appuie sur une touche
//utilisé pour passer à la fonction d'affichage de fenetre precedent
void appuye_touche(void) {
	signed char touche = -1;
	while (touche == -1) {
		touche = serial_get_last_char();
	}
}

//fenetre explication du jeu
void recap_regle(void) {
	vt100_clear_screen(); //efface la fenetre precedente
	fenetre('#', '#');
	vt100_move(25, 4);
	serial_puts("Vous etes seul face à l'univers");
	vt100_move(25, 8);
	serial_puts("Tuez les vaisseaux ennemis en leur tirant dessus");
	vt100_move(40, 16);
	serial_puts("Touche :");
	vt100_move(40, 18);
	serial_puts("<- : q");
	vt100_move(40, 20);
	serial_puts("-> : d");
	vt100_move(40, 22);
	serial_puts("tire : a");
	vt100_move(65, 26);
	serial_puts("Appuyer sur une touche");
	appuye_touche();
}

//fonction qui initialise la position de notre vaisseau
//et l'etat de notre missile quand le jeu debute
void initialisation(void) {
	vaisseau[1].vx = 43;
	missile[1].etat = 1; //Notre missile est à l'état mort, sinon il s'afficherai dès le debut
	for (int i = 0; i < 5; i++) {
		mechant[i].etat = 0;
	}
}
//fenetre de jeu
void jeu(unsigned char x, unsigned char y) {
	vt100_clear_screen();
	fenetre('#', '#');
}

//affiche des bunkers qui sont la juste pour faire jolie :)
void bunker(void) {
	int i;

	for (i = 3; i < 89; i = i + 20) {
		vt100_move(i, 20);
		serial_puts("_-^^^-_");
	}
}

//fonction qui permet le deplacement
void deplacement_vaisseau(int move) {

	//deplacement vers la droite
	if (vaisseau[1].vx < 85) {
		if (move == 'd') {
			vt100_move(vaisseau[1].vx, 25);
			serial_puts("     ");
			vaisseau[1].vx = vaisseau[1].vx + 1;
			vt100_move(vaisseau[1].vx, 25);
			serial_puts("|-O-|");
		}
	}
	//deplacement vers la gauche
	if (vaisseau[1].vx > 2) {
		if (move == 'q') {
			vt100_move(vaisseau[1].vx, 25);
			serial_puts("     ");
			vaisseau[1].vx = vaisseau[1].vx - 1;
			vt100_move(vaisseau[1].vx, 25);
			serial_puts("|-O-|");
		}
	}
}

//fonction tir de notre vaisseau
void tir(int shoot) {

	if (shoot == 'a') {
		missile[1].vx = vaisseau[1].vx + 2; // le missile part du centre de notre vaisseau en x
		missile[1].vy = 24; //le missile part toujours de la même ligne, soit celle juste au dessus de notre vaisseau en y
		missile[1].etat = 0; //affiche notre missile, car initialisé avant à 1
	}

	if (missile[1].etat == 0) {
		missile[1].vy--; // si le missile est vivant, il se deplace sur y
		vt100_move(missile[1].vx, missile[1].vy); //on recupère les coordonnée du missile
		serial_puts("|"); // et on l'affiche
		delai(2);
		vt100_move(missile[1].vx, missile[1].vy);
		serial_puts(" "); //Puis le supprime pour un effet de deplacement
		delai(2);
		if (missile[1].vy == 2) { //si le missile arrive tout en haut
			missile[1].etat = 1; //alors le missile disparait car mort.
		}
	}

}

//Fonction qui permet de definir si une colision existe entre missile et vaisseaux ennemis
void hitbox(void) {
	for (int i = 0; i < 6; i++) {
		if (mechant[i].vy == missile[1].vy
				&& (mechant[i].vx == missile[1].vx
						|| mechant[i].vx == missile[1].vx + 1
						|| mechant[i].vx + 2 == missile[1].vx
						|| mechant[i].vx + 3 == missile[1].vx)) {
			mechant[i].etat = 1; // si oui mechant mort
			missile[1].etat = 1; // et missile mort
		}
	}
}

//fonction qui positionne mes ennemies a leur point de depart
void position_mechant(unsigned char g, unsigned char h) {
	for (int i = 0; i < 6; i++) {
		mechant[i].vx = g;
		mechant[i].vy = h;
		g = g + 10; //ecart entre chaque ennemis
	}
}

//fonction qui affiche les vaisseaux ennemis
void affmechant(void) {
	for (int i = 0; i < 6; i++) {
		if (mechant[i].etat == 0) { // si il est vivant on l'affiche
			vt100_move(mechant[i].vx, mechant[i].vy);
			serial_puts(" x0x ");
			vt100_move(mechant[i].vx, mechant[i].vy - 1);
			serial_puts("     ");
		}
		if (mechant[i].etat == 1) { // si il est mort on n'affiche du vide
			vt100_move(mechant[i].vx, mechant[i].vy);
			serial_puts("     ");
		}

	}
}

float g = 4;
double h = 5;
float direction = 0.1;
void mouvement_mechant(void) {
	g = g + direction;
	if (g > 35) {
		direction = -0.1; //les mechants vont vers la droite
	}
	if (g < 4) {
		direction = 0.1; //les mechants vont vers la gauche
	}
	if (h < 24) {
		h = h + 0.01;
	}
	position_mechant(g, h); // les nouvelles valeurs sont données à la fonction position méchants
							// cela permet le mouvement, car de nouvelles valeurs a chaque boucle
}


void delai(unsigned long n) {
	int i = 0;
	unsigned long int max = n * 100000;
	do {
		i++;
	} while (i <= max);
}

//fonction qui affiche la fenetre de fin de jeu
void end(void) {
	if (mechant[0].etat == 1 && mechant[1].etat == 1 && mechant[2].etat == 1
			&& mechant[3].etat == 1 && mechant[4].etat == 1
			&& mechant[5].etat == 1) { // Si tout les ennemis sont mort affiche la fenetre de victoire
		vt100_clear_screen();
		fenetre('#', '#');
		vt100_move(35, 10);
		serial_puts("VICTOIRE POUR LE PEUPLE !");
		vt100_move(45, 15);
		serial_puts("BRAVO !");
		vt100_move(65, 26);
		serial_puts("Assez simple en vrai..");
		vt100_move(35, 20);
		serial_puts("Appuyer sur une touche");
		appuye_touche();


	}
	if (mechant[0].vy == 24) { // si les ennemis sont arrivé en bas avant de tous être tué affiche la fenetre de defaite
		vt100_clear_screen();
		fenetre('#', '#');
		vt100_move(35, 6);
		serial_puts("BIENVENUE AU PARADIS !");
		vt100_move(35, 10);
		serial_puts("Pas fou hein ?");
		vt100_move(35, 12);
		serial_puts("Et oui vous etes mort");
		vt100_move(30, 16);
		serial_puts("Les ennemis sont pas tres fort pourtant..");
		vt100_move(60, 20);
		serial_puts("Appuyer sur une touche");
		vt100_move(60, 22);
		serial_puts("Pour ressusciter");
		vt100_move(60, 24);
		serial_puts("ET RETOURNER VOUS BATTRE!");
		appuye_touche();

	}
}
