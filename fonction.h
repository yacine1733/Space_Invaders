/*
 * fonction.h
 *
 *  Created on: 22 janv. 2020
 *      Author: yacine
 */

#ifndef FONCTION_H_
#define FONCTION_H_

void fenetre(unsigned char character1, unsigned char character2);
void menu_accueil(void);
void appuye_touche(void);
void recap_regle(void);
void initialisation(void);
void jeu(unsigned char x, unsigned char y);
void deplacement_vaisseau(int move);
void delai(unsigned long n);
void hitbox(void);
void tir(int shoot);
void position_mechant(unsigned char g, unsigned char h);
void affmechant(void);
void mouvement_mechant(void);
void bunker(void);
void end(void);


#endif /* FONCTION_H_ */
