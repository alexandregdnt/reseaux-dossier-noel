/*--------------------------------------
    Alexandre Grodent
----------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "Libraries/udplib/udplib.h"
#include "Libraries/LibSerAG.h"
#include "Libraries/requeteAG.h"

#define SERVER_LOG_FILE "logServer.log"
#define VEHICULES_FILE "VehiculesAG"
#define FACTURES_FILE "FacturesAG"

void die(char *);

int main(int argc, char *argv[]) {
    int rc;
    int Desc;
    struct sockaddr_in sthis; /* this ce programme */
    struct sockaddr_in sos; /* s = serveur */
    struct sockaddr_in sor; /* r = remote */

    u_long IpSocket;
    u_short PortSocket;

    int tm;
    struct VehiculeAG UnVehicule;
    struct FactureAG UneFacture;
    struct RequeteAG UneRequete;

    int flog;
    flog = open(SERVER_LOG_FILE, O_CREAT | O_RDWR, 0644);
    dup2(flog, 2);

    memset(&sthis, 0, sizeof(struct sockaddr_in));
    memset(&sos, 0, sizeof(struct sockaddr_in));
    memset(&sor, 0, sizeof(struct sockaddr_in));

    printf("Ceci est le serveur\n");
    if (argc != 3) {
        printf("ser ser port cli\n");
        exit(1);
    }

    /* Récupération IP & port   */
    IpSocket = inet_addr(argv[1]);
    PortSocket = atoi(argv[2]);
    // Desc = CreateSockets(&psoo,&psos,,atoi(argv[2]),argv[3],atoi(argv[4]));
    Desc = creer_socket(SOCK_DGRAM, &IpSocket, PortSocket, &sthis);

    if (Desc == -1)
        die("CreateSockets:");
    else
        printf("CreateSockets %d \n", Desc);

    int nbRequetes = 0;
    while (1) {
        tm = sizeof(struct RequeteAG);
        rc = ReceiveDatagram(Desc, &UneRequete, tm, &sor);
        if (rc == -1)
            die("ReceiveDatagram");
        else {
            printf("bytes recus:%d - Reference: %d \n", rc, UneRequete.Reference);
            afficheRequeteAG(stderr, UneRequete);
        }
        printf("Received packet from %s:%d \n", inet_ntoa(sor.sin_addr), ntohs(sor.sin_port));
        fprintf(stderr, "Received packet from %s:%d \n", inet_ntoa(sor.sin_addr), ntohs(sor.sin_port));

        printf("Recherche vehicule serveur (reponse): %d \n", rechercheVehiculeAG(VEHICULES_FILE, UneRequete.Reference, &UnVehicule));
        printf("Reference: %d \n", UneRequete.Reference);
        if (rechercheVehiculeAG(VEHICULES_FILE, UneRequete.Reference, &UnVehicule) == 1) {
            switch (UneRequete.Type) {
                case Question: {
                    UneRequete.Type = OK;
                    UneRequete.Numero = nbRequetes;
                    UneRequete.NumeroFacture = 0;
                    UneRequete.Date = 0;
                    UneRequete.Quantite = 0;
                    UneRequete.Prix = 0;
                    strcpy(UneRequete.Marque, UnVehicule.Marque);
                    strcpy(UneRequete.Modele, UnVehicule.Modele);
                    strcpy(UneRequete.Transmission, UnVehicule.Transmission);
                    strcpy(UneRequete.NomClient, "/");

                    break;
                }

                case Achat: {
                    UneRequete.Type = OK;
                    UneRequete.Numero = nbRequetes;
                    UneRequete.NumeroFacture = nombreFacturesAG(FACTURES_FILE);
                    UneRequete.Date = 0;
                    UneRequete.Prix = 0;
                    strcpy(UneRequete.Marque, UnVehicule.Marque);
                    strcpy(UneRequete.Modele, UnVehicule.Modele);
                    strcpy(UneRequete.Transmission, UnVehicule.Transmission);

                    UneFacture.NumeroFacturation = UneRequete.NumeroFacture;
                    strcpy(UneFacture.NomClient, UneRequete.NomClient);
                    UneFacture.DateFacturation = UneRequete.Date;
                    UneFacture.Reference = UneRequete.Reference;
                    UneFacture.Quantite = UneRequete.Quantite;

                    if (UneFacture.Quantite <= UnVehicule.Quantite) {
                        // Mettre à jour le vehicule en question
                        UnVehicule.Quantite = UnVehicule.Quantite - UneFacture.Quantite;
                        if (modifierVehiculeAG(VEHICULES_FILE, &UnVehicule) == 1) {
                            creationAjoutFactureAG(FACTURES_FILE, &UneFacture);
                            printf("Reservation effectuee \n");
                        } else {
                            UneRequete.Type = Fail;
                            printf("Erreur lors de la reservation \n");
                        }
                    } else {
                        UneRequete.Type = Fail;
                        printf("Quantite commande trop grande ! \n");
                    }
                    break;
                }

                default: {
                    UneRequete.Type = Fail;
                    fprintf(stderr, "Type de requete inconnu \n");
                }
            }
        } else {
            UneRequete.Type = Fail;
            fprintf(stderr, "Rien a ete trouve \n");
        }

        //strcat(UneRequete.Message, " Client");
        rc = SendDatagram(Desc, &UneRequete, sizeof(struct RequeteAG), &sor);
        if (rc == -1) {
            die("SendDatagram:");
        } else {
            printf("bytes envoyes:%d\n", rc);
            afficheRequeteAG(stderr, UneRequete);
        }

        nbRequetes++;
    }

    close(flog);
    close(Desc);
}

void die(char *s) {
    perror(s);
    exit(1);
}