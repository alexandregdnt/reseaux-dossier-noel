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
#define VEHICULES_FILE "VehiculesAG"
#define FACTURES_FILE "FacturesAG"

void sysCls();
void gotoXY(int, int);
void initTermios(int);
void resetTermios();
char getch_(int);
char getchE();

static struct termios old, new;

int main() {
    char Choix;
    char Tampon[80];
    struct VehiculeAG UnVehicule;
    struct FactureAG UneFacture;

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
        Choix = getchE();
        printf("\n");

        switch (Choix) {
            case '1': {
                char redo = 'y';

                while (redo == 'Y' || redo == 'y') {
                    int nombre;
                    nombre = nombreVehiculesAG(VEHICULES_FILE);
                    saisieVehiculeAG(nombre + 1, &UnVehicule);
                    creationAjoutVehiculeAG(VEHICULES_FILE, &UnVehicule);
                    printf("Encoder un autre (Y/N) ?)");
                    printf(">> ");
                    redo = getchE();
                    printf("\n");
                }
                break;
            }
            case '2':
                listingVehiculesAG(VEHICULES_FILE);
                break;
            case '3': {
                int reference = 0;

                printf("Entrez la reference du vehicule desire : \n>> ");
                fflush(stdin);
                fgets(Tampon, sizeof Tampon, stdin);
                reference = atoi(Tampon);

                if (rechercheVehiculeAG(VEHICULES_FILE, reference, &UnVehicule) == 1) {
                    afficheEnteteVehiculeAG();
                    afficheVehiculeAG(&UnVehicule);
                } else {
                    printf("Rien a ete trouve \n");
                }

                break;
            }
            case '4': {
                int nombre;
                nombre = nombreFacturesAG(FACTURES_FILE);
                saisieFactureAG(nombre + 1, &UneFacture);

                if (rechercheVehiculeAG(VEHICULES_FILE, UneFacture.Reference, &UnVehicule) == 1) {
                    printf("Vehicule trouve \n");
                    printf("Marque: %s, Modele: %s, Qte: %d, Transmission: %s \n", UnVehicule.Marque, UnVehicule.Modele, UnVehicule.Quantite, UnVehicule.Transmission);
                    if (UneFacture.Quantite <= UnVehicule.Quantite) {
                        // Mettre à jour le vehicule en question
                        UnVehicule.Quantite = UnVehicule.Quantite - UneFacture.Quantite;
                        if (modifierVehiculeAG(VEHICULES_FILE, &UnVehicule) == 1) {
                            creationAjoutFactureAG(FACTURES_FILE, &UneFacture);
                            printf("Reservation effectuee \n");
                        } else {
                            printf("Erreur lors de la reservation \n");
                        }
                    } else {
                        printf("Quantite commande trop grande ! \n");
                    }
                } else {
                    printf("Aucun vehicule trouve \n");
                }

                break;
            }
            case '5':
                listingFacturesAG(FACTURES_FILE);
                break;
            case '6':
                aProposServeurAG("v0.4.1", "Alexandre", "Grodent");
                break;
            case '7':
                exit(0);
            default:
                break;
        }
    }

    close(flog);
}

//sysCls() function definition
// #include <stdlib.h>
void sysCls() {
    system("clear");
}

void gotoXY(int x, int y) {
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
getchE() function definition.
*/
char getchE(void) {
    return getch_(1);
}
