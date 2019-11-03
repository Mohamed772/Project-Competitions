/*Projet : Gestion d'une compétion de poursuite par équipe
Créé le : 07/10/2019
Auteurs : Mohamed BEN BELKACEM, Raphaël CATARINO, Manil RICHARD*/

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
} Mesure;// structure de la mesure du temps

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
} Equipe;// structure contenant les informations d'une équipe

typedef struct {
	Equipe equipes[MAX_EQUIPE];
	unsigned int compteur_nb_equipes;
	unsigned int tour_max;
	unsigned int epreuves_max;
	unsigned int epreuve_actuelle;
} Competition; // structure d�finissant toute la competition

// ensemble de tous les prototypes des fonctions du projet
void inscrire_equipe(Competition* liste_equipes);
void afficher_equipes(const Competition* liste_equipes);
void trouver_dossard(unsigned int dossard, unsigned int* num_equipe, unsigned int* num_patineur);
void enregistrement_temps(Competition* liste_equipes);
void affichage_temps(const Competition* liste_equipes);
unsigned int compare_tour_equipe(Equipe equipe);
float compare_temps_joueurs(Equipe equipe, int tour_actuel);
void affichage_temps_equipes(Competition* liste_equipes);
void detection_fin_parcours(Competition* liste_equipes);
void detection_fin_competition(Competition* liste_equipes);
void trier_equipes(Competition* liste_equipes);
void initialisation_temps(Equipe* equipe);

/*
 * Cette fonction enregistre le nom de l'équipe et des 3 patineurs et leurs dossards dans la variable liste_equipes.
	appelle la fonction initialisation_temps
 * \auteurs M. BEN BELKACEM, R. CATARINO, M. RICHARD
 * \param[in-out] liste_equipes qui va recevoir les informations d'une équipe e.
 * \param[scan] Variable intermédiaire e de type Equipe. Stocke les informations reçues.
 * \return void
 */
void inscrire_equipe(Competition* liste_equipes) {
	Equipe e; char mot[lgMot + 1]; static unsigned int dos = 101;
	initialisation_temps(&e);

	if (liste_equipes->compteur_nb_equipes <= MAX_EQUIPE) {
		scanf("%s", &mot);
		strcpy(e.pays, mot);
		for (int i = 0; i <= MAX_JOUEUR_PAR_EQUIPE - 1; ++i) {
			scanf("%s", &mot);
			strcpy(e.personnes[i].nom, mot);
			e.personnes[i].num_dossard = dos++;
			e.personnes[i].tours = 0;
			printf("inscription dossard %d\n", e.personnes[i].num_dossard);}
		liste_equipes->equipes[liste_equipes->compteur_nb_equipes] = e;
		liste_equipes->compteur_nb_equipes += 1;
	}
	else printf("Vous avez deja atteint le nombre d'equipe maximale");
}

/*
 * Cette fonction afficher le nom de l'équipe et des 3 patineurs et leurs dossards des 2 équipes de l'épreuve en cours.
 * \auteurs M. BEN BELKACEM, R. CATARINO, M. RICHARD
 * \param[in] liste_equipes
 * \fonction d'affichage, aucune variable modifiée ou renvoyée
 * \return void
 */
void afficher_equipes(const Competition* liste_equipes) {
	unsigned int i = liste_equipes->epreuve_actuelle * 2;

	for (unsigned int j = i; j < i + NB_EQUIPE_EPREUVE;j++) {
		printf("%s %s %u %s %u %s %u\n", liste_equipes->equipes[j].pays, liste_equipes->equipes[j].personnes[0].nom,
			liste_equipes->equipes[j].personnes[0].num_dossard, liste_equipes->equipes[j].personnes[1].nom,
			liste_equipes->equipes[j].personnes[1].num_dossard, liste_equipes->equipes[j].personnes[2].nom,
			liste_equipes->equipes[j].personnes[2].num_dossard);}
}

/*
 * Fonction auxiliaire : trouve le numéro de l'équipe et du patineur à partir du numéro de dossard
 * \auteurs M. BEN BELKACEM, R. CATARINO, M. RICHARD
 * \param[in] dossard : numéro du dossard envoyé
 * \param[in-out] num_equipe et num_patineur : numéros d'équipe et de patineur, permettant d'enregistrer un temps
 * \return void
 */
void trouver_dossard(unsigned int dossard, unsigned int* num_equipe, unsigned int* num_patineur) {
	dossard -= 100;
	if ((dossard % 3) == 0) {
		*num_equipe = (dossard / 3) - 1;
		*num_patineur = 2;}
	else {
		*num_equipe = (dossard / 3);
		*num_patineur = (dossard % 3) - 1;}
}

/*
 * Cette fonction enregistre le temps d'un patineur dans la variable liste_equipes, met à jour l'épreuve actuelle et appelle la fonction detection_fin_parcours
 * \auteurs M. BEN BELKACEM, R. CATARINO, M. RICHARD
 * \param[in-out] liste_equipes qui va recevoir un temps de patineur
 * \param[scan] Variables intermédiaires : num_equipe, num_patineur, temps
 * \return void
 */
void enregistrement_temps(Competition* liste_equipes) {
	unsigned int num_equipe = 0, num_patineur, dossard, num_tours;
	float temps;

	scanf("%u %u %f", &dossard, &num_tours, &temps);
	if (num_tours <= liste_equipes->tour_max) {
		trouver_dossard(dossard, &num_equipe, &num_patineur);
		liste_equipes->equipes[num_equipe].personnes[num_patineur].tours = num_tours;
		liste_equipes->equipes[num_equipe].personnes[num_patineur].liste_temps[num_tours - 1].temps = temps;}
	liste_equipes->equipes[num_equipe].tours = compare_tour_equipe(liste_equipes->equipes[num_equipe]);
	liste_equipes->equipes[num_equipe].liste_temps[num_tours - 1].temps = compare_temps_joueurs(liste_equipes->equipes[num_equipe], num_tours);
	detection_fin_parcours(liste_equipes);
	liste_equipes->epreuve_actuelle = (num_equipe / 2);
}

/*
 * Cette fonction affiche pour un numéro de dossard scanné :
	le pays, le tour, le nom et le temps du patineur à ce tour, pour chaque tour effectué
 * \auteurs M. BEN BELKACEM, R. CATARINO, M. RICHARD
 * \param[in] liste_equipe
 * \param[scan] dossard : numéro du dossard du patineur recherché
 * \fonction d'affichage, aucune variable modifiée ou renvoyée
 * \return void
 */
void affichage_temps(const Competition* liste_equipes) {
	unsigned int num_equipe, num_patineur, dossard, num_tours = 0, i = 0;

	scanf("%u", &dossard);
	trouver_dossard(dossard, &num_equipe, &num_patineur);
	num_tours = liste_equipes->equipes[num_equipe].personnes[num_patineur].tours;

	while (i < num_tours) {
		printf("%s %u %s %.1f\n", liste_equipes->equipes[num_equipe].pays,
			i + 1,
			liste_equipes->equipes[num_equipe].personnes[num_patineur].nom,
			liste_equipes->equipes[num_equipe].personnes[num_patineur].liste_temps[i].temps);
		i++;}
}

/*
 * Fonction auxiliaire : permet d'obtenir le dernier tour effectué par tous les patineurs d'une équipe
 * \auteurs M. BEN BELKACEM, R. CATARINO, M. RICHARD
 * \param[in] equipe de type Equipe : l'équipe dont on cherche le tour actuel
 * \return le numéro du tour du patineur le plus loin
 */
unsigned int compare_tour_equipe(Equipe equipe) {
	unsigned int a = 0; a = equipe.personnes[0].tours;
	unsigned int b = 0; b = equipe.personnes[1].tours;
	unsigned int c = 0; c = equipe.personnes[2].tours;

	if ((a <= b) && (a <= c)) return a;
	else if (b <= c) return b;
	else return c;
}

/*
 * Fonction auxiliaire : permet d'obtenir le pire temps effectué par tous les patineurs d'une équipe
 * \auteurs M. BEN BELKACEM, R. CATARINO, M. RICHARD
 * \param[in] equipe de type Equipe : l'équipe dont on cherche le temps, et tour_actuel : le tour qu'on recherche
 * \return le temps du pire patineur pour le tour demandé
 */
float compare_temps_joueurs(Equipe equipe, int tour_actuel) {
	tour_actuel--;
	float a = equipe.personnes[0].liste_temps[tour_actuel].temps;
	float b = equipe.personnes[1].liste_temps[tour_actuel].temps;
	float c = equipe.personnes[2].liste_temps[tour_actuel].temps;

	if ((a == -1.) || (b == -1.) || (c == -1.)) return -1.;
	if ((a >= b) && (a >= c)) return a;
	else if (b >= c) return b;
	else return c;
}

/*
 * Cette fonction afficher le temps des équipes de l'épreuve en cours à un tour donné. Si le temps n'est pas encore réalisé : affiche indisponible
 * \auteurs M. BEN BELKACEM, R. CATARINO, M. RICHARD
 * \param[in] liste_equipes
 * \param[scan] tour : numéro du tour recherché
 * \fonction d'affichage, aucune variable modifiée ou renvoyée
 * \return void
 */
void affichage_temps_equipes(Competition* liste_equipes) {
	unsigned int tour;
	scanf("%u", &tour);
	unsigned int i = liste_equipes->epreuve_actuelle*2;
	
	liste_equipes->equipes[i].liste_temps[tour - 1].temps = compare_temps_joueurs(liste_equipes->equipes[i], tour);
	liste_equipes->equipes[i + 1].liste_temps[tour - 1].temps = compare_temps_joueurs(liste_equipes->equipes[i + 1], tour);
	for (unsigned int j=i; j < i+NB_EQUIPE_EPREUVE; j++){
		if (liste_equipes->equipes[j].liste_temps[tour - 1].temps == -1) {
			printf("%s indisponible\n", liste_equipes->equipes[j].pays);
		}
		else {
			printf("%s %.1f\n", liste_equipes->equipes[j].pays, liste_equipes->equipes[j].liste_temps[tour - 1].temps);
		}
	}
	
}

/*
 * Cette fonction détecte la fin d'une poursuite puis affiche :
	"detection_fin_poursuite" puis les noms et temps des équipes de l'épreuve par ordre croissant de temps
	détecte aussi la fin de compétition en appelant la fonciton detection_fin_competition
 * \auteurs M. BEN BELKACEM, R. CATARINO, M. RICHARD
 * \param[in] liste_equipes
 * \fonction d'affichage, aucune variable modifiée ou renvoyée
 * \return void
 */
void detection_fin_parcours(Competition* liste_equipes) {
	unsigned int tour_max = liste_equipes->tour_max;
	static unsigned int compteur_fin_parcours=0;
	static unsigned char etat_parcours[16] = { 0 };

	for (unsigned int i = 0; i < liste_equipes->compteur_nb_equipes; i += 2) {
		if ((liste_equipes->equipes[i].tours == tour_max) && (liste_equipes->equipes[i + 1].tours == tour_max) && etat_parcours[i/2] == 0) {
			printf("detection_fin_poursuite\n");

			if (liste_equipes->equipes[i].liste_temps[tour_max - 1].temps <= liste_equipes->equipes[i + 1].liste_temps[tour_max - 1].temps){
				printf("%s %.1f\n", liste_equipes->equipes[i].pays, liste_equipes->equipes[i].liste_temps[tour_max - 1].temps);
				printf("%s %.1f\n", liste_equipes->equipes[i + 1].pays, liste_equipes->equipes[i + 1].liste_temps[tour_max - 1].temps);}

			else if (liste_equipes->equipes[i].liste_temps[tour_max - 1].temps > liste_equipes->equipes[i + 1].liste_temps[tour_max - 1].temps){
				printf("%s %.1f\n", liste_equipes->equipes[i + 1].pays, liste_equipes->equipes[i + 1].liste_temps[tour_max - 1].temps);
				printf("%s %.1f\n", liste_equipes->equipes[i].pays, liste_equipes->equipes[i].liste_temps[tour_max - 1].temps);}
			etat_parcours[i/2] = 1;
			compteur_fin_parcours++;
			liste_equipes->epreuve_actuelle++;
		}
	}
	if (liste_equipes->epreuves_max == compteur_fin_parcours) detection_fin_competition(liste_equipes);
}

/*
 * Cette fonction affiche :
	"detection_fin_competition" puis les noms et temps des équipes de toute la competition par odre croissant de temps
	appelle la fonction trier_equipes
 * \auteurs M. BEN BELKACEM, R. CATARINO, M. RICHARD
 * \param[in] liste_equipes
 * \fonction d'affichage, aucune variable modifiée ou renvoyée
 * \return void
 */
void detection_fin_competition(Competition* liste_equipes){
	unsigned int Competitions_max = liste_equipes->epreuves_max;
	unsigned int tour_max = liste_equipes->tour_max;
	trier_equipes(liste_equipes);
	printf("detection_fin_competition\n");

	for (unsigned int i = 0; i < (Competitions_max*NB_EQUIPE_EPREUVE); i++){
		printf("%s %.1f\n", liste_equipes->equipes[i].pays, liste_equipes->equipes[i].liste_temps[tour_max - 1].temps);
	}
}

/*
 * Fonction auxiliaire : permet de trier les équipes de liste_equipes par ordre croissant de temps
 * \auteurs M. BEN BELKACEM, R. CATARINO, M. RICHARD
 * \param[in-out] liste_equipes
 * \return void
 */
void trier_equipes(Competition* liste_equipes){
	unsigned int n = liste_equipes->compteur_nb_equipes;
	Equipe stockage;
	int j;
	unsigned int tour_max = liste_equipes->tour_max - 1;

	for (int i = 1; i < n; i++){
		stockage = liste_equipes->equipes[i];
		j = i;
		while (j > 0 && (liste_equipes->equipes[j-1].liste_temps[tour_max].temps > stockage.liste_temps[tour_max].temps)){
			liste_equipes->equipes[j] = liste_equipes->equipes[j - 1];
			j--;}
		liste_equipes->equipes[j] = stockage;
	}
}

/*
 * Fonction auxiliaire : permet d'initialiser tous les temps d'une équipe à -1
 * \auteurs M. BEN BELKACEM, R. CATARINO, M. RICHARD
 * \param[in-out] equipe de type Equipe : l'équipe qu'on vient d'inscrire
 * \return void
 */
void initialisation_temps(Equipe* equipe) {
	for (unsigned int j = 0; j < MAX_TOUR; j++) {
		equipe->liste_temps[j].temps = -1.;
		for (unsigned int k = 0; k < MAX_JOUEUR_PAR_EQUIPE; k++) {
			equipe->personnes[k].liste_temps[j].temps = -1.;
		}
	}
}


/*
 * Fonction main : scan les commandes de l'utilisateurs et appelle les fonctions principal
	arrête le programme lorsqu'il reçoit "exit"
 *\auteurs M. BEN BELKACEM, R. CATARINO, M. RICHARD
 */
int main() {
	Competition liste_equipes;
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
