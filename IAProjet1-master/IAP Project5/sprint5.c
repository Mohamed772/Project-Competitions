#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//definition constantes
#define MAX_JOUEUR_PAR_EQUIPE 3
#define lgMot 50
#define MAX_EQUIPE 32
#define MAX_TOUR 10
#define NB_EQUIPE_EPREUVE 2
#define MAX_EPREUVE 16

#pragma warning(disable : 4996)

typedef struct {
	float temps;
} Mesure;//structure de la mesure du temps

typedef struct {
	char nom[lgMot + 1];
	unsigned int num_dossard;
	Mesure liste_temps[MAX_TOUR];
	unsigned int tours;
} Patineur;//structure contenant les informations d'un patineurs

typedef struct {
	Patineur personnes[MAX_JOUEUR_PAR_EQUIPE];
	char pays[lgMot + 1];
	Mesure liste_temps[MAX_TOUR];
	unsigned int tours;
} Equipe;//structure contenant les informations d'une équipe

typedef struct {
	Equipe equipes[MAX_EQUIPE];
	unsigned int compteur_nb_equipes;
	unsigned int tour_max;
	unsigned int epreuves_max;
	unsigned int epreuve_actuel;
} Course; // /!\ structure définissant toute la competition


void initialisation_temps(Course* liste_equipes);
void inscrire_equipe(Course* liste_equipes);
void afficher_equipes(const Course* liste_equipes);
void trouver_dossard(unsigned int dossard, unsigned int* num_equipe, unsigned int* num_patineur);
void enregistrement_temps(Course* liste_equipes);
void affichage_temps(const Course* liste_equipes);
unsigned int compare_tour_equipe(Equipe equipe);
float compare_temps_joueurs(Equipe equipe, int tour_actuel);
void affichage_temps_equipes(Course* liste_equipes);
void detection_fin_parcours(Course* liste_equipes);
void detection_fin_competition(Course* liste_equipes);
void trier_equipes(Course* liste_equipes);

void inscrire_equipe(Course* liste_equipes) {
	Equipe e;
	initialisation_temps(&e);
	char mot[lgMot + 1];
	static unsigned int dos = 101;
	if (liste_equipes->compteur_nb_equipes <= MAX_EQUIPE) {
		scanf("%s", &mot);
		strcpy(e.pays, mot);

		for (int i = 0; i <= MAX_JOUEUR_PAR_EQUIPE - 1; ++i) {
			scanf("%s", &mot);
			strcpy(e.personnes[i].nom, mot);
			e.personnes[i].num_dossard = dos++;
			e.personnes[i].tours = 0;
			printf("inscription dossard %d\n", e.personnes[i].num_dossard);
		}

		liste_equipes->equipes[liste_equipes->compteur_nb_equipes] = e;
		liste_equipes->compteur_nb_equipes += 1;
	}
	else
		printf("Vous avez deja atteint le nombre d'equipe maximale");
}

void afficher_equipes(const Course* liste_equipes) {
	Equipe e;
	for (unsigned int i = 0; i < liste_equipes->compteur_nb_equipes; i++) {
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
	if (num_tours <= liste_equipes->tour_max) {
		trouver_dossard(dossard, &num_equipe, &num_patineur);

		liste_equipes->equipes[num_equipe].personnes[num_patineur].tours = num_tours;
		liste_equipes->equipes[num_equipe].personnes[num_patineur].liste_temps[num_tours - 1].temps = temps;
	}
	else printf("Heu ca fais pas un peu beaucoup la non ?");

	liste_equipes->equipes[num_equipe].tours = compare_tour_equipe(liste_equipes->equipes[num_equipe]);
	liste_equipes->equipes[num_equipe].liste_temps[num_tours - 1].temps = compare_temps_joueurs(liste_equipes->equipes[num_equipe], num_tours);
	detection_fin_parcours(liste_equipes);
	liste_equipes->epreuve_actuel = (num_equipe / 2);
}

void affichage_temps(const Course* liste_equipes) {
	unsigned int num_equipe, num_patineur, dossard, num_tours = 0, i = 0;


	scanf("%u", &dossard);
	trouver_dossard(dossard, &num_equipe, &num_patineur);

	num_tours = liste_equipes->equipes[num_equipe].personnes[num_patineur].tours;

	while (i < num_tours) {

		printf("%s %u %s %.1f\n", liste_equipes->equipes[num_equipe].pays,
			i + 1,
			liste_equipes->equipes[num_equipe].personnes[num_patineur].nom,
			liste_equipes->equipes[num_equipe].personnes[num_patineur].liste_temps[i].temps);
		i++;
	}
}

unsigned int compare_tour_equipe(Equipe equipe) {
	unsigned int a = 0; a = equipe.personnes[0].tours;
	unsigned int b = 0; b = equipe.personnes[1].tours;
	unsigned int c = 0; c = equipe.personnes[2].tours;

	if ((a <= b) && (a <= c)) {
		return a;
	}
	else if (b <= c) {
		return b;
	}
	else
		return c;
}

float compare_temps_joueurs(Equipe equipe, int tour_actuel) {
	tour_actuel--;
	if ((equipe.personnes[0].liste_temps[tour_actuel].temps >= equipe.personnes[1].liste_temps[tour_actuel].temps)
		&& (equipe.personnes[0].liste_temps[tour_actuel].temps >= equipe.personnes[2].liste_temps[tour_actuel].temps)) {
		return equipe.personnes[0].liste_temps[tour_actuel].temps;
	}
	else if (equipe.personnes[1].liste_temps[tour_actuel].temps >= equipe.personnes[2].liste_temps[tour_actuel].temps) {
		return equipe.personnes[1].liste_temps[tour_actuel].temps;
	}
	else return equipe.personnes[2].liste_temps[tour_actuel].temps;

}

void affichage_temps_equipes(Course* liste_equipes) {
	unsigned int tour;
	scanf("%u", &tour);
	unsigned int i = liste_equipes->epreuve_actuel*2;
	
	liste_equipes->equipes[i].liste_temps[tour - 1].temps = compare_temps_joueurs(liste_equipes->equipes[i], tour);
	liste_equipes->equipes[i + 1].liste_temps[tour - 1].temps = compare_temps_joueurs(liste_equipes->equipes[i + 1], tour);
	for (unsigned int j=i; j < i+NB_EQUIPE_EPREUVE; j++){
		if (liste_equipes->equipes[j].liste_temps[tour - 1].temps == -1) {
			printf("%s Indisponible\n", liste_equipes->equipes[j].pays);
		}
		else {
			printf("%s %.1f\n", liste_equipes->equipes[j].pays, liste_equipes->equipes[j].liste_temps[tour - 1].temps);
		}
	}
	
}

void detection_fin_parcours(Course* liste_equipes) {
	unsigned int tour_max = liste_equipes->tour_max;
	static unsigned int compteur_fin_parcours=0;
	static unsigned char etat_parcours[16] = { 0 };

	for (unsigned int i = 0; i < liste_equipes->compteur_nb_equipes; i += 2) {
		if ((liste_equipes->equipes[i].tours == tour_max) && (liste_equipes->equipes[i + 1].tours == tour_max) && etat_parcours[i/2] == 0) {
			
			printf("detection_fin_poursuite\n");
			if (liste_equipes->equipes[i].liste_temps[tour_max - 1].temps <= liste_equipes->equipes[i + 1].liste_temps[tour_max - 1].temps){
				printf("%s %.1f\n", liste_equipes->equipes[i].pays, liste_equipes->equipes[i].liste_temps[tour_max - 1].temps);
				printf("%s %.1f\n", liste_equipes->equipes[i + 1].pays, liste_equipes->equipes[i + 1].liste_temps[tour_max - 1].temps);
			}
			else if (liste_equipes->equipes[i].liste_temps[tour_max - 1].temps > liste_equipes->equipes[i + 1].liste_temps[tour_max - 1].temps){
				printf("%s %.1f\n", liste_equipes->equipes[i + 1].pays, liste_equipes->equipes[i + 1].liste_temps[tour_max - 1].temps);
				printf("%s %.1f\n", liste_equipes->equipes[i].pays, liste_equipes->equipes[i].liste_temps[tour_max - 1].temps);
			}
			etat_parcours[i/2] = 1;
			compteur_fin_parcours++;

		}
	}
	if (liste_equipes->epreuves_max == compteur_fin_parcours)
		detection_fin_competition(liste_equipes);
}

void detection_fin_competition(Course* liste_equipes){
	unsigned int courses_max = liste_equipes->epreuves_max;
	unsigned int tour_max = liste_equipes->tour_max;

	trier_equipes(liste_equipes);
	printf("detection_fin_competition\n");

	for (unsigned int i = 0; i < (courses_max*NB_EQUIPE_EPREUVE); i++){
		printf("%s %.1f\n", liste_equipes->equipes[i].pays, liste_equipes->equipes[i].liste_temps[tour_max - 1].temps);
	}
}

void trier_equipes(Course* liste_equipes){
	unsigned int n = liste_equipes->compteur_nb_equipes;
	Equipe stockage;
	int j;
	unsigned int tour_max = liste_equipes->tour_max - 1;

	for (int i = 1; i < n; i++){
		stockage = liste_equipes->equipes[i];
		j = i;
		while (j > 0 && (liste_equipes->equipes[j-1].liste_temps[tour_max].temps > stockage.liste_temps[tour_max].temps)){
			liste_equipes->equipes[j] = liste_equipes->equipes[j - 1];
			j--;
		}
		liste_equipes->equipes[j] = stockage;
	}
}

void initialisation_temps(Equipe* equipe) {
	
	for (unsigned int j = 0; j < MAX_TOUR; j++) {
		equipe->liste_temps[j].temps = -1.;
		for (unsigned int k = 0; k < MAX_JOUEUR_PAR_EQUIPE; k++) {
			equipe->personnes[k].liste_temps[j].temps = -1.;
		}
	}
	
	
}



int main() {
	Course liste_equipes;
	liste_equipes.compteur_nb_equipes = 0;

	char mot[lgMot + 1];

	do {
		scanf("%s", &mot);
		if (strcmp(mot, "inscrire_equipe") == 0) {
			inscrire_equipe(&liste_equipes);
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
		if (strcmp(mot, "afficher_temps_equipes") == 0) {
			affichage_temps_equipes(&liste_equipes);
		}
		if (strcmp(mot, "definir_parcours") == 0) {
			unsigned int parcours;
			scanf("%u", &parcours);
			liste_equipes.tour_max = parcours;
		}
		if (strcmp(mot, "definir_nombre_epreuves") == 0) {
			unsigned int epreuves;
			scanf("%u", &epreuves);
			liste_equipes.epreuves_max = epreuves;
		}
	} while (strcmp(mot, "exit") != 0);
	exit(0);
}