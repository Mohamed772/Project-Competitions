#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma warning(disable : 4996)

void trouver_dossard(unsigned int dossard, unsigned int* num_equipe, unsigned int* num_patineur) {
	dossard -= 100;

	if ((dossard % 3) == 0) {
		*num_equipe = (dossard / 3) - 1;
		*num_patineur = (dossard % 3)+2;
	}
	else {
		*num_equipe = (dossard / 3);
		*num_patineur = (dossard % 3) - 1;
	}
}

int main() {
	unsigned int dossard=0;
	unsigned int num_equipe;
	unsigned int num_patineur;

	do {
		scanf("%u", &dossard);
		trouver_dossard(dossard, &num_equipe, &num_patineur);
		printf("%u %u %u", dossard, num_equipe, num_patineur);
	} while ( dossard!= 30 );
		exit(0);
}