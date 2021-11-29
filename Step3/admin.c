/******************************************
  Alexandre Grodent 2104
  2021 - Informatique de gestion B2
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "Libraries/data.h"
#include "Libraries/LibSerAG.h"

// include pour les fonctions entrees sortie
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define ADMIN_LOG_FILE "logAdmin.log"

void ClrScr();
void Gotoxy(int, int);
void initTermios(int);
void resetTermios();
char getch_(int);
char GetchE();
void DelNewLine(char *);
void MonPrintf(char *, int, int);
void CreationAjoutVehiculeAG(char *, struct VehiculeAG *);
void SaiSieVehiculeAG(int, struct VehiculeAG *);
int NombreVehiculesAG(char *);
void AfficheVehiculeAG(struct VehiculeAG *);
void AfficheEnteteVehiculeAG();
void AfficheFacture(struct FactureAG *);
void ListingVehiculesAG(char *);
void ListingFacturesAG(char *);

static struct termios old, new;

int main() {
    char Choix;
    char Tampon[80];
    struct VehiculeAG UnRecord;

    int flog;
    flog = open(ADMIN_LOG_FILE, O_CREAT | O_RDWR, 0644);
    dup2(flog, 2);

    while (1) {
        printf("----------------------\n");
        printf("1) Ajout              \n");
        printf("2) Vehicule           \n");
        printf("3) Recherche          \n");
        printf("4) Achat              \n");
        printf("5) Factures           \n");
        printf("6) A propos           \n");
        printf("7) exit               \n");
        printf("----------------------\n");
        printf(">> ");
        Choix = GetchE();
        printf("\n");

        switch (Choix) {
            case '1': {
                char Redo = 'y';

                while (Redo == 'Y' || Redo == 'y') {
                    int Nombre;
                    Nombre = NombreVehiculesAG("VehiculesAG");
                    SaiSieVehiculeAG(Nombre + 1, &UnRecord);
                    CreationAjoutVehiculeAG("VehiculesAG", &UnRecord);
                    printf("Encoder un autre (Y/N) ?)");
                    printf(">> ");
                    Redo = GetchE();
                    printf("\n");
                }
                break;
            }
            case '2':
                ListingVehiculesAG("VehiculesAG");
                break;
            case '3': {
                int reference = 0;

                printf("Entrez la reference du vehicule desie : ");
                fflush(stdin);
                fgets(Tampon, sizeof Tampon, stdin);
                reference = atoi(Tampon);

                if (RechercheAG("VehiculesAG", reference, &UnRecord) == 1) {
                    AfficheEnteteVehiculeAG();
                    AfficheVehiculeAG(&UnRecord);
                } else {
                    printf("Rien a ete trouve");
                }

                break;
            }
            case '4':
                break;
            case '5':
                ListingFacturesAG("FactureAG");
                break;
            case '6':
                AProposServeurAG("v0.1.0", "Alexandre", "Grodent");
                break;
            case '7':
                exit(0);
            default:
                break;
        }
    }

    close(flog);
}

//clrscr() function definition
// #include <stdlib.h>
void ClrScr() {
    system("clear");
}

void Gotoxy(int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}

/* Initialize new terminal i/o settings */
// #include <stdio.h>
// #include <stdlib.h>

void initTermios(int echo) {
    tcgetattr(0, &old); //grab old terminal i/o settings
    new = old; //make new settings same as old settings
    new.c_lflag &= ~ICANON; //disable buffered i/o
    new.c_lflag &= echo ? ECHO : ~ECHO; //set echo mode
    tcsetattr(0, TCSANOW, &new); //apply terminal io settings
}

/* Restore old terminal i/o settings */
void resetTermios(void) {
    tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) {
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}

/*
Read 1 character without echo 
getch() function definition.
*/
char Getch(void) {
    return getch_(0);
}

/*
Read 1 character with echo 
getche() function definition.
*/
char GetchE(void) {
    return getch_(1);
}

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

void CreationAjoutVehiculeAG(char *NomFichier, struct VehiculeAG *UnRecord) {
    FILE *sortie;

    sortie = fopen(NomFichier, "a"); /* Si le fichier existe, on le cree sinon on ajoute */
    if (sortie == NULL) {
        fprintf(stderr, "Echec Ouverture\n");
        exit(0);
    } else
        fprintf(stderr, "Ouverture reussie \n");


    printf("Position actuelle dans le fichier %ld\n", ftell(sortie));
    fwrite(UnRecord, sizeof(struct VehiculeAG), 1, sortie);
    fflush(sortie);
    fclose(sortie);
}

void SaiSieVehiculeAG(int Reference, struct VehiculeAG *UnRecord) {
    char Tampon[80];

    printf("Reference :%d \n", Reference);
    UnRecord->Reference = Reference;
    printf("Saisie Marque :");
    fgets(UnRecord->Marque, sizeof UnRecord->Marque, stdin);
    printf("Saisie Modele :");
    fgets(UnRecord->Modele, sizeof UnRecord->Modele, stdin);
    printf("Saisie Transmission :");
    fgets(UnRecord->Transmission, sizeof UnRecord->Transmission, stdin);
    printf("Saisie Quantite :");
    fgets(Tampon, sizeof Tampon, stdin);
    UnRecord->Quantite = atoi(Tampon);

    DelNewLine(UnRecord->Marque);
    DelNewLine(UnRecord->Modele);
    DelNewLine(UnRecord->Transmission);
    AfficheEnteteVehiculeAG();
    AfficheVehiculeAG(UnRecord);
    printf("-----------------------\n");
    return;
}

int NombreVehiculesAG(char *NomFichier) {
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

void AfficheVehiculeAG(struct VehiculeAG *UnRecord) {
    char Tampon[80];
    sprintf(Tampon, "%d", UnRecord->Reference);
    MonPrintf(Tampon, 4, strlen(Tampon));
    sprintf(Tampon, "%s", UnRecord->Marque);
    MonPrintf(Tampon, 30, strlen(Tampon));
    sprintf(Tampon, "%s", UnRecord->Modele);
    MonPrintf(Tampon, 30, strlen(Tampon));
    sprintf(Tampon, "%s", UnRecord->Transmission);
    MonPrintf(Tampon, 30, strlen(Tampon));
    sprintf(Tampon, "%d", UnRecord->Quantite);
    MonPrintf(Tampon, 6, strlen(Tampon));
    printf("\n");
}

void AfficheEnteteVehiculeAG() {
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

void AfficheFacture(struct FactureAG *UneFacture) {
    char Tampon[80];
    sprintf(Tampon, "%d", UneFacture->NumeroFacturation);
    MonPrintf(Tampon, 4, strlen(Tampon));
    sprintf(Tampon, "%s", UneFacture->NomClient);
    MonPrintf(Tampon, 60, strlen(Tampon));
    sprintf(Tampon, "%d", UneFacture->Quantite);
    MonPrintf(Tampon, 4, strlen(Tampon));
    sprintf(Tampon, "%d", UneFacture->Reference);
    MonPrintf(Tampon, 4, strlen(Tampon));
    sprintf(Tampon, "%d", UneFacture->DateFacturation);
    MonPrintf(Tampon, 8, strlen(Tampon));

    printf("\n");
}

void ListingVehiculesAG(char *NomFichier) {
    struct VehiculeAG UnRecord;
    FILE *sortie;
    int nbr;

    sortie = fopen(NomFichier, "r"); /* Si le fichier existe, on le cree sinon on ajoute */
    if (sortie == NULL) {
        fprintf(stderr, "Echec Ouverture\n");
        exit(0);
    } else
        fprintf(stderr, "Ouverture reussie \n");

    AfficheEnteteVehiculeAG();
    nbr = fread(&UnRecord, sizeof(UnRecord), 1, sortie);

    while (!feof(sortie)) {
        fprintf(stderr, "Record lu %d et Position actuelle dans le fichier %ld\n", nbr, ftell(sortie));
        AfficheVehiculeAG(&UnRecord);
        nbr = fread(&UnRecord, sizeof(UnRecord), 1, sortie);
    }
    fclose(sortie);
}

void ListingFacturesAG(char *NomFichier) {
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
        AfficheFacture(&UneFacture);
        nbr = fread(&UneFacture, sizeof(struct FactureAG), 1, sortie);
    }
    fclose(sortie);
}
