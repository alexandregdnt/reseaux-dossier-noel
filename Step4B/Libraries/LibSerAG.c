#include "LibSerAG.h"

/* ========== VÉHICULES ========== */
int nombreVehiculesAG(char *NomFichier) {
    FILE *sortie;
    sortie = fopen(NomFichier, "r"); /* Si le fichier existe, on le cree sinon on ajoute */
    if (sortie == NULL) {
        fprintf(stderr, "Pas de fichier\n");
        return 0;
    } else
        fprintf(stderr, "Ouverture reussie \n");
    fseek(sortie, 0, SEEK_END);
    return (ftell(sortie) / sizeof(struct VehiculeAG));
}
void saisieVehiculeAG(int Reference, struct VehiculeAG *UnVehicule) {
    char Tampon[80];

    printf("Reference: %d \n", Reference);
    UnVehicule->Reference = Reference;
    printf("Saisie Marque: ");
    fgets(UnVehicule->Marque, sizeof UnVehicule->Marque, stdin);
    strcpy(UnVehicule->Marque, toCapitalize(UnVehicule->Marque));
    printf("Saisie Modele: ");
    fgets(UnVehicule->Modele, sizeof UnVehicule->Modele, stdin);
    strcpy(UnVehicule->Modele, toCapitalize(UnVehicule->Modele));
    printf("Saisie Transmission: ");
    fgets(UnVehicule->Transmission, sizeof UnVehicule->Transmission, stdin);
    strcpy(UnVehicule->Transmission, toCapitalize(UnVehicule->Transmission));
    printf("Saisie Quantite: ");
    fgets(Tampon, sizeof Tampon, stdin);
    UnVehicule->Quantite = atoi(Tampon);

    DelNewLine(UnVehicule->Marque);
    DelNewLine(UnVehicule->Modele);
    DelNewLine(UnVehicule->Transmission);
    afficheEnteteVehiculeAG();
    afficheVehiculeAG(UnVehicule);
    printf("-----------------------\n");
    return;
}
void creationAjoutVehiculeAG(char *NomFichier, struct VehiculeAG *UnVehicule) {
    FILE *sortie;

    sortie = fopen(NomFichier, "a"); /* Si le fichier existe, on le cree sinon on ajoute */
    if (sortie == NULL) {
        fprintf(stderr, "Echec Ouverture\n");
        exit(0);
    } else
        fprintf(stderr, "Ouverture reussie \n");


    printf("Position actuelle dans le fichier %ld\n", ftell(sortie));
    fwrite(UnVehicule, sizeof(struct VehiculeAG), 1, sortie);
    fflush(sortie);
    fclose(sortie);
}
int rechercheVehiculeAG (char *NomFichier, int Reference, struct VehiculeAG *UnVehicule) {
    FILE *fp;
    fp = fopen(NomFichier, "r+");

    if (fp != NULL) {
        fread(UnVehicule, sizeof(*UnVehicule), 1, fp);

        while (!feof(fp) && UnVehicule->Reference != Reference) {
            fread(UnVehicule, sizeof(*UnVehicule), 1, fp);
        }
    } else {
        return -1;
    }

    fclose(fp);
    return UnVehicule->Reference == Reference;
}
int modifierVehiculeAG (char *NomFichier, struct VehiculeAG *UnVehicule) {
    struct VehiculeAG UnVehiculeTmp;
    struct VehiculeAG *pTmp = &UnVehiculeTmp;
    FILE *fp;
    fp = fopen(NomFichier, "r+");

    if (fp != NULL) {
        fread(pTmp, sizeof(struct VehiculeAG), 1, fp);

        while (!feof(fp) && pTmp->Reference != UnVehicule->Reference) {
            fread(pTmp, sizeof(struct VehiculeAG), 1, fp);
        }

        fseek(fp, -sizeof(struct VehiculeAG), SEEK_CUR);
        fwrite(UnVehicule, sizeof(struct VehiculeAG), 1, fp);
    } else {
        return -1;
    }

    fflush(fp);
    fclose(fp);

#ifdef DEBUG
    printf("\n");
    afficheEnteteVehiculeAG();
    if (pTmp != NULL) {
        printf("Vehicule temporaire \n");
        afficheVehiculeAG(pTmp);
        printf("\n");
    }
    printf("Nouveau vehicule \n");
    afficheVehiculeAG(UnVehicule);
    printf("\n");
#endif

    return pTmp->Reference == UnVehicule->Reference;
}

void afficheVehiculeAG(struct VehiculeAG *UnVehicule) {
    char Tampon[80];
    sprintf(Tampon, "%d", UnVehicule->Reference);
    MonPrintf(Tampon, 4, strlen(Tampon));
    sprintf(Tampon, "%s", UnVehicule->Marque);
    MonPrintf(Tampon, 30, strlen(Tampon));
    sprintf(Tampon, "%s", UnVehicule->Modele);
    MonPrintf(Tampon, 30, strlen(Tampon));
    sprintf(Tampon, "%s", UnVehicule->Transmission);
    MonPrintf(Tampon, 30, strlen(Tampon));
    sprintf(Tampon, "%d", UnVehicule->Quantite);
    MonPrintf(Tampon, 6, strlen(Tampon));
    printf("\n");
}
void afficheEnteteVehiculeAG() {
    char Tampon[80];
    sprintf(Tampon, "%s", "Ref");
    MonPrintf(Tampon, 4, strlen(Tampon));
    sprintf(Tampon, "%s", "Marque");
    MonPrintf(Tampon, 30, strlen(Tampon));
    sprintf(Tampon, "%s", "Modele");
    MonPrintf(Tampon, 30, strlen(Tampon));
    sprintf(Tampon, "%s", "Transmission");
    MonPrintf(Tampon, 30, strlen(Tampon));
    sprintf(Tampon, "%s", "Quantite");
    MonPrintf(Tampon, 6, strlen(Tampon));
    printf("\n\n");
}
void listingVehiculesAG(char *NomFichier) {
    struct VehiculeAG UnVehicule;
    FILE *sortie;
    int nbr;

    sortie = fopen(NomFichier, "r"); /* Si le fichier existe, on le cree sinon on ajoute */
    if (sortie == NULL) {
        fprintf(stderr, "Echec Ouverture\n");
        exit(0);
    } else
        fprintf(stderr, "Ouverture reussie \n");

    afficheEnteteVehiculeAG();
    nbr = fread(&UnVehicule, sizeof(UnVehicule), 1, sortie);

    while (!feof(sortie)) {
        fprintf(stderr, "Record lu %d et Position actuelle dans le fichier %ld\n", nbr, ftell(sortie));
        afficheVehiculeAG(&UnVehicule);
        nbr = fread(&UnVehicule, sizeof(UnVehicule), 1, sortie);
    }
    fclose(sortie);
}

/* ========== FACTURES ========== */
int nombreFacturesAG(char *NomFichier) {
    FILE *sortie;
    sortie = fopen(NomFichier, "r"); /* Si le fichier existe, on le cree sinon on ajoute */

    if (sortie == NULL) {
        fprintf(stderr, "Pas de fichier\n");
        return 0;
    } else
        fprintf(stderr, "Ouverture reussie \n");

    fseek(sortie, 0, SEEK_END);
    return (ftell(sortie) / sizeof(struct FactureAG));
}
void saisieFactureAG(int NumeroFacturation, struct FactureAG *UneFacture) {
    char Tampon[80];

    printf("NumeroFacturation: %d \n", NumeroFacturation);
    UneFacture->NumeroFacturation = NumeroFacturation;
    UneFacture->DateFacturation = 0;
    printf("Saisie Nom du client: ");
    fgets(UneFacture->NomClient, sizeof UneFacture->NomClient, stdin);
    strcpy(UneFacture->NomClient, toCapitalize(UneFacture->NomClient));
    printf("Saisie Reference: ");
    fgets(Tampon, sizeof Tampon, stdin);
    UneFacture->Reference = atoi(Tampon);
    printf("Saisie Quantite: ");
    fgets(Tampon, sizeof Tampon, stdin);
    UneFacture->Quantite = atoi(Tampon);

    DelNewLine(UneFacture->NomClient);
    afficheEnteteFactureAG();
    afficheFactureAG(UneFacture);
    printf("-----------------------\n");
}
void creationAjoutFactureAG(char *NomFichier, struct FactureAG *UneFacture) {
    FILE *sortie;

    sortie = fopen(NomFichier, "a"); /* Si le fichier existe, on le cree sinon on ajoute */
    if (sortie == NULL) {
        fprintf(stderr, "Echec Ouverture\n");
        exit(0);
    } else
        fprintf(stderr, "Ouverture reussie \n");

    // printf("Position actuelle dans le fichier %ld\n", ftell(sortie));
    fwrite(UneFacture, sizeof(struct FactureAG), 1, sortie);

    fflush(sortie);
    fclose(sortie);
}

void afficheEnteteFactureAG() {
    char Tampon[80];
    sprintf(Tampon, "%s", "Num_Fact");
    MonPrintf(Tampon, 10, strlen(Tampon));
    sprintf(Tampon, "%s", "Nom_Client");
    MonPrintf(Tampon, 45, strlen(Tampon));
    sprintf(Tampon, "%s", "Quantite");
    MonPrintf(Tampon, 10, strlen(Tampon));
    sprintf(Tampon, "%s", "Ref");
    MonPrintf(Tampon, 4, strlen(Tampon));
    sprintf(Tampon, "%s", "Date");
    MonPrintf(Tampon, 8, strlen(Tampon));

    printf("\n\n");
}
void afficheFactureAG(struct FactureAG *UneFacture) {
    char Tampon[80];
    sprintf(Tampon, "%d", UneFacture->NumeroFacturation);
    MonPrintf(Tampon, 10, strlen(Tampon));
    sprintf(Tampon, "%s", UneFacture->NomClient);
    MonPrintf(Tampon, 45, strlen(Tampon));
    sprintf(Tampon, "%d", UneFacture->Quantite);
    MonPrintf(Tampon, 10, strlen(Tampon));
    sprintf(Tampon, "%d", UneFacture->Reference);
    MonPrintf(Tampon, 4, strlen(Tampon));
    sprintf(Tampon, "%d", UneFacture->DateFacturation);
    MonPrintf(Tampon, 8, strlen(Tampon));

    printf("\n");
}
void listingFacturesAG(char *NomFichier) {
    struct FactureAG UneFacture;
    FILE *sortie;
    int nbr;

    sortie = fopen(NomFichier, "r"); /* Si le fichier existe, on le cree sinon on ajoute */
    if (sortie == NULL) {
        fprintf(stderr, "Echec Ouverture\n");
        exit(0);
    } else
        fprintf(stderr, "Ouverture reussie \n");


    nbr = fread(&UneFacture, sizeof(struct FactureAG), 1, sortie);

    while (!feof(sortie)) {
        fprintf(stderr, "Record lu %d et Position actuelle dans le fichier %ld\n", nbr, ftell(sortie));
        afficheFactureAG(&UneFacture);
        nbr = fread(&UneFacture, sizeof(struct FactureAG), 1, sortie);
    }
    fclose(sortie);
}

/* ========== AUTRES FONCTIONS ========== */
// permet de supprimer le cr qui placé dans la chaine lors d'un fgets
void DelNewLine(char *Chaine) {
    Chaine[strlen(Chaine) - 1] = 0;
}
void MonPrintf(char *tempo, int espace, int taille) {
    int Count;
    printf("%s", tempo);
    Count = espace - taille;
    while (Count > 0) {
        printf(" ");
        Count--;
    }
}
void aProposServeurAG (const char *Version, const char *Prenom, const char* Nom) {
    printf("Version : %s \n", Version);
    printf("Prenom : %s \n", Prenom);
    printf("Nom : %s \n", Nom);
}
char *toCapitalize(const char *str) {
    char *tmpstr = malloc(strlen(str) + 1);

    for (int i = 0; str[i] != '\0'; i++) {
        if (i == 0 || str[i-1] == ' ' || str[i-1] == '-') {
            tmpstr[i] = toupper(str[i]);
        } else {
            tmpstr[i] = tolower(str[i]);
        }
    }
    return tmpstr;
}