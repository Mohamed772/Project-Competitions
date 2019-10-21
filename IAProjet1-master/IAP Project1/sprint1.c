#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//definition constantes
#define MAX_JOUEUR_PAR_EQUIPE 3
#define DOSSARD 100
#define lgMot 50
#define MAX_EQUIPE 32

#pragma warning(disable : 4996)

int compteur_dossard = 1; //variables globales qui seront supprimées par la suite
int compteur_nb_equipes = 0;



typedef struct {
	char nom[lgMot + 1];
	unsigned int num_dossard;
}Patineur; //structure contenant les informations d'un patineur

typedef struct {
	Patineur personnes[MAX_JOUEUR_PAR_EQUIPE];
	char pays[lgMot + 1];
}Equipe;//structure contenant les informations d'une équipe

typedef struct {
	Equipe equipes[MAX_EQUIPE];

}Equipes;//structure contenant un tableau de toutes les équipes de la compétition

Equipes liste_equipes;

void inscrire_equipe(Equipe* e) {
	char mot[lgMot+1];
	if (compteur_nb_equipes <= MAX_EQUIPE) {
		scanf("%s", &mot);
		strcpy(e->pays, mot);

		for (int i = 0; i <= MAX_JOUEUR_PAR_EQUIPE - 1; ++i) {
			scanf("%s", &mot);
			strcpy(e->personnes[i].nom, mot);
		}

		for (int i = 0; i <= MAX_JOUEUR_PAR_EQUIPE - 1; ++i) {
			e->personnes[i].num_dossard = DOSSARD + compteur_dossard++;
			printf("inscription dossard %d\n", e->personnes[i].num_dossard);
		}
		liste_equipes.equipes[compteur_nb_equipes] = *e;
		++compteur_nb_equipes;
	}
	else
		printf("Vous avez déjà atteint le nombre d'équipe maximale");
}

void afficher_equipes(const Equipes* liste_equipes) {
	Equipe e;

	for (int i = 0; i < compteur_nb_equipes; i++) {
		e = liste_equipes->equipes[i];
		printf("%s %s %u %s %u %s %u\n", e.pays, e.personnes[0].nom, e.personnes[0].num_dossard, e.personnes[1].nom,
			e.personnes[1].num_dossard, e.personnes[2].nom, e.personnes[2].num_dossard);
	}
}

int main() {
	Equipe e;
	char mot[lgMot+1];

	do {
		scanf("%s", &mot);

		if (strcmp(mot, "inscrire_equipe") == 0) {
			inscrire_equipe(&e);
		}
		if (strcmp(mot, "afficher_equipes") == 0) {
			afficher_equipes(&liste_equipes);
		}
	} while (strcmp(mot, "exit") != 0);
	exit(0);
}