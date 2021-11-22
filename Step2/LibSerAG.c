#include "LibSerAG.h"

void AProposServeurAG (char *Version,char *Nom1,char* Nom2)
{
 printf("Version : %s \n",Version );
 printf("Nom1 : %s \n",Nom1 );
 printf("Nom2 : %s \n",Nom2 );
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