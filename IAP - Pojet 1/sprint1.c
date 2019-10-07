#include "stdlib.h"
#include "stdio.h"
#include <string>

#define MAX_EQUIPE 3
#define DOSSARD 100
#define lgMot 50

#pragma warning(disable : 4996)

int compteur = 1;

struct Personne
{
	char nom;
	unsigned int num_dossard;
};

struct Equipe
{
	Personne personnes[MAX_EQUIPE];
	char pays;
};

Equipe inscrire_equipe(Equipe* e) {

	Equipe nouvelle_equipe;
	nouvelle_equipe.pays = e->pays;

	Personne p1;
	p1.nom = e->personnes[0].nom;
	p1.num_dossard = DOSSARD + compteur;
	printf("inscription dossard %d\n", p1.num_dossard);
	++compteur;

	nouvelle_equipe.personnes[0] = p1;

	return nouvelle_equipe;
}

int main() {

	char mot[lgMot + 1];

	do {
		scanf("%s", &mot);
		if (strcmp(mot, "inscrire_equipe") == 0) {
			Equipe e;
			scanf("%s", &mot);
			e.pays = *mot;

			scanf("%s", mot);
			e.personnes[0].nom = *mot;

			Equipe equipe = inscrire_equipe(&e);
		}

	} while (strcmp(mot, "exit") != 0);
	exit(0);

}