#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//definition constantes
#define MAX_JOUEUR_PAR_EQUIPE 3
#define DOSSARD 100
#define lgMot 50
#define MAX_EQUIPE 32
#define MAX_TOUR 10

#pragma warning(disable : 4996)

int compteur_dossard = 1;//variables globales qui seront supprimées par la suite
int compteur_nb_equipes = 0;

typedef struct {
	float temps;
} Mesure;

typedef struct {
	char nom[lgMot + 1];
	unsigned int num_dossard;
	Mesure liste_temps[MAX_TOUR];
	unsigned int tours;
} Patineur;//structure contenant les informations des Patineurs

typedef struct {
	Patineur personnes[MAX_JOUEUR_PAR_EQUIPE];
	char pays[lgMot + 1];
} Equipe;//structure contenant les informations d'une équipe

typedef struct {
	Equipe equipes[MAX_EQUIPE];
} Course;






void inscrire_equipe(Equipe* e, Course* liste_equipes) {
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

	if ((dossard % 3) == 0) {
		*num_equipe = (dossard / 3) - 1;
		*num_patineur = 2;
	}
	else {
		*num_equipe = (dossard / 3);
		*num_patineur = (dossard % 3) - 1;
	}
}

void enregistrement_temps(Course* liste_equipes) {
	unsigned int num_equipe, num_patineur, dossard, num_tours;
	float temps;

	scanf("%u %u %f", &dossard, &num_tours, &temps);
	if (num_tours <= MAX_TOUR) {
		trouver_dossard(dossard, &num_equipe, &num_patineur);

		liste_equipes->equipes[num_equipe].personnes[num_patineur].tours = num_tours;
		liste_equipes->equipes[num_equipe].personnes[num_patineur].liste_temps[num_tours - 1].temps = temps;
	}
	else printf("Heu ca fais pas un peu beaucoup la non ?");
	
}

void affichage_temps(const Course* liste_equipes) {
	unsigned int num_equipe, num_patineur, dossard, num_tours=0 ,i=0;
	

	scanf("%u", &dossard);
	trouver_dossard(dossard, &num_equipe, &num_patineur);

	num_tours = liste_equipes->equipes[num_equipe].personnes[num_patineur].tours;

	while (i < num_tours) {

		printf("%s %u %s %.1f\n", liste_equipes->equipes[num_equipe].pays,
								i+1,
								liste_equipes->equipes[num_equipe].personnes[num_patineur].nom,
								liste_equipes->equipes[num_equipe].personnes[num_patineur].liste_temps[i].temps);
		i++;
	}
	
}

int main() {
	Course liste_equipes;
	Equipe e;
	char mot[lgMot+1];

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