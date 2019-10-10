#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_JOUEUR_PAR_EQUIPE 3
#define DOSSARD 100
#define lgMot 50
#define MAX_EQUIPE 32
#define MAX_TOUR 10

#pragma warning(disable : 4996)

int compteur_dossard = 1;
int compteur_nb_equipes = 0;

typedef struct {
	float temps;
} Mesure;

typedef struct {
	char nom[lgMot + 1];
	unsigned int num_dossard;
	Mesure liste_temps[MAX_TOUR];
} Patineur;

typedef struct {
	Patineur personnes[MAX_JOUEUR_PAR_EQUIPE];
	char pays[lgMot + 1];
} Equipe;

typedef struct {
	Equipe equipes[MAX_EQUIPE];
} Course;






void inscrire_equipe(Equipe* e, Course* liste_equipes) {
	if (compteur_nb_equipes <= MAX_EQUIPE) {
		scanf("%s %s %s %s", e->pays, e->personnes[0].nom, e->personnes[1].nom, e->personnes[2].nom);
		for (int i = 0; i <= MAX_JOUEUR_PAR_EQUIPE - 1; ++i) {
			e->personnes[i].num_dossard = DOSSARD + compteur_dossard++;
			printf("inscription dossard %s %d\n", e->personnes[i].nom, e->personnes[i].num_dossard);
		}
		liste_equipes->equipes[compteur_nb_equipes] = *e;
		++compteur_nb_equipes;
	}
	else
		printf("Vous avez déjà atteint le nombre d'équipe maximale");
}

void afficher_equipes(const Course* liste_equipes) {
	Equipe e;
	for (int i = 0; i < compteur_nb_equipes; i++) {
		e = liste_equipes->equipes[i];
		printf("%s %s %u %s %u %s %u\n", e.pays, e.personnes[0].nom, e.personnes[0].num_dossard, e.personnes[1].nom,
			e.personnes[1].num_dossard, e.personnes[2].nom, e.personnes[2].num_dossard);
	}
}

void trouver_dossard(unsigned int dossard, unsigned int* num_equipe, unsigned int* num_patineur) {
	dossard -= 100;
	*num_equipe = (dossard / 3);
	*num_patineur = (dossard % 3);
}

void enregistrement_temps(Course* liste_equipes) {
	unsigned int num_equipe, num_patineur;
	unsigned int dossard, num_tours;
	float temps;

	scanf("%d %d %f", &dossard, &num_tours, &temps);
	trouver_dossard(dossard, &num_equipe, &num_patineur);

	liste_equipes->equipes[num_equipe].personnes[num_patineur].liste_temps[num_tours - 1].temps = temps;
	printf("%d %d %d %f %f", num_equipe, num_patineur, num_tours, temps, liste_equipes->equipes[num_equipe].personnes[num_patineur].liste_temps[num_tours - 1].temps);
}

void affichage_temps(const Course* liste_equipes) {

}

int main() {
	Course liste_equipes;
	Equipe e;
	char mot[lgMot];

	do {
		scanf("%s", &mot);
		if (strcmp(mot, "inscrire_equipe") == 0) {
			inscrire_equipe(&e, &liste_equipes);
		}
		if (strcmp(mot, "afficher_equipes") == 0) {
			afficher_equipes(&liste_equipes);
		}
		if (strcmp(mot, "enregistrer_temps") == 0) {
			enregistrement_temps(&liste_equipes);
		}
		if (strcmp(mot, "afficher_temps") == 0) {
			affichage_temps(&liste_equipes);
		}
	} while (strcmp(mot, "exit") != 0);
	exit(0);
}