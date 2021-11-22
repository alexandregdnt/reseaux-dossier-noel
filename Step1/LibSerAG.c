#include "LibSerAG.h"

void AProposServeurAG (char *Version, char *Prenom, char* Nom)
{
 printf("Version : %s \n", Version);
 printf("Prenom : %s \n", Prenom);
 printf("Nom : %s \n", Nom);
}


int RechercheAG (char *NomFichier, int Reference, struct VehiculeAG *UnRecord) {
    FILE *fp;

    fp = fopen(NomFichier, "r+");
    fread(UnRecord, sizeof(*UnRecord), 1, fp);

    while (!feof(fp) && UnRecord->Reference != Reference) {
        fread(UnRecord, sizeof(*UnRecord), 1, fp);
    }
    fclose(fp);

    return UnRecord->Reference == Reference;
}