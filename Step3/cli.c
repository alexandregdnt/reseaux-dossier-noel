/*---------------------------------------------------------------
   Alexandre Grodent
------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "Libraries/udplib/udplib.h"
#include "Libraries/requeteAG.h"

#define CLIENT_LOG_FILE "logClient.log"

void die(char *);

int main(int argc, char *argv[]) {
    int rc;
    int Desc;
    int tm;
    char Tampon[80];

    int flog;
    flog = open(CLIENT_LOG_FILE, O_CREAT | O_RDWR, 0644);
    dup2(flog, 2);

    u_long IpSocket, IpServer;
    u_short PortSocket, PortServer;

    struct sockaddr_in sthis; /* this ce programme */
    struct sockaddr_in sos; /* s = serveur */
    struct sockaddr_in sor; /* r = remote */
    struct RequeteAG UneRequete;

    memset(&sthis, 0, sizeof(struct sockaddr_in));
    memset(&sos, 0, sizeof(struct sockaddr_in));
    memset(&sor, 0, sizeof(struct sockaddr_in));

    if (argc != 5) {
        printf("cli client portc serveur ports\n");
        exit(1);
    }

    /* Récupération IP & port   */
    IpSocket = inet_addr(argv[1]);
    PortSocket = atoi(argv[2]);

    IpServer = inet_addr(argv[3]);
    PortServer = atoi(argv[4]);

    // Desc = CreateSockets(&psoo,&psos,,atoi(argv[2]),argv[3],atoi(argv[4]));
    Desc = creer_socket(SOCK_DGRAM, &IpSocket, PortSocket, &sthis);

    if (Desc == -1)
        die("CreateSockets:");
    else
        printf("CreateSockets %d\n", Desc);

    sos.sin_family = AF_INET;
    sos.sin_addr.s_addr = IpServer;
    sos.sin_port = htons(PortServer);

    int redo = 1;
    while (redo == 1) {

        printf("----------------------                  \n");
        printf("Quel est le type de votre requete ?     \n");
        printf("1) Question                             \n");
        printf("2) Achat                                \n");
        printf("3) Quitter                              \n");
        printf(">> ");

        fflush(stdin);
        fgets(Tampon, sizeof Tampon, stdin);
        int choix = atoi(Tampon);

        switch (choix) {
            case Question: {
                UneRequete.Type = Question;

                printf("----------------------  \n");
                do {
                    printf("Reference : ");
                    fflush(stdin);
                    fgets(Tampon, sizeof Tampon, stdin);
                    UneRequete.Reference = atoi(Tampon);
                } while (UneRequete.Reference <= 0);
                printf("----------------------  \n");

                UneRequete.Numero = 0;
                UneRequete.NumeroFacture = 0;
                UneRequete.Date = 0;
                UneRequete.Quantite = 0;
                UneRequete.Prix = 0;
                strcpy(UneRequete.Marque, "/");
                strcpy(UneRequete.Modele, "/");
                strcpy(UneRequete.Transmission, "/");
                strcpy(UneRequete.NomClient, "/");

                printf("----------------------  \n");
                // printf("Verification des donnees et confirmation de l'envoi \n");
                // AfficheRequeteAG(stdout, UneRequete);
                // printf("Les informations sont elles correcte ? \n");
                printf("Souhaitez-vous envoyer cette requete ? (Y/N) \n");
                printf(">> ");

                fflush(stdin);
                char confirm = getchar();
                printf("----------------------  \n");

                if (confirm == 'y' || confirm == 'Y') {
                    rc = SendDatagram(Desc, &UneRequete, sizeof(struct RequeteAG), &sos);

                    if (rc == -1)
                        die("SendDatagram");
                    else
                        printf("Envoi de %d bytes \n", rc);

                    memset(&UneRequete, 0, sizeof(struct RequeteAG));
                    tm = sizeof(struct RequeteAG);

                    rc = ReceiveDatagram(Desc, &UneRequete, tm, &sor);
                    if (rc == -1)
                        die("ReceiveDatagram");
                    else {
                        printf("bytes recus:%d \n", rc);
                        printf("Marque: %s - Modele: %s \n", UneRequete.Marque, UneRequete.Modele);
                        AfficheRequeteAG(stderr, UneRequete);
                    }
                }

                break;
            }

                // En Attendant la mise en place d'achat
                /*
            case Achat:
                UneRequete.Type = Achat;
                break;
                 */

            case 3:
                redo = 0;
                break;

            default:
                redo = 1;
        }
    }

    close(flog);
    close(Desc);
}

void die(char *s) {
    perror(s);
    exit(1);
}