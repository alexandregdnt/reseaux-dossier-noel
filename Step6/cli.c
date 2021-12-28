/*---------------------------------------------------------------
   Alexandre Grodent
------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include "Libraries/udplib/udplib.h"
#include "Libraries/requeteAG.h"
#include "Libraries/LibSerAG.h"

#define CLIENT_LOG_FILE "logClient.log"

void die(char *);
static void signal_handler(int sig) {};

int main(int argc, char *argv[]) {
    int rc;
    int Desc;
    int tm;
    char Tampon[80];

    int numRequete;
    unsigned int timeInterval;
    struct sigaction act;

    int flog;
    flog = open(CLIENT_LOG_FILE, O_CREAT | O_RDWR, 0644);
    dup2(flog, 2);

    u_long IpSocket, IpServer;
    u_short PortSocket, PortServer;

    struct sockaddr_in sthis; /* this ce programme */
    struct sockaddr_in sos; /* s = serveur */
    struct sockaddr_in sor; /* r = remote */
    struct RequeteAG UneRequeteE, UneRequeteR;

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

    numRequete = 0;
    while (1) {
        act.sa_handler = signal_handler;
        act.sa_flags = 0;
        sigemptyset(&act.sa_mask);

        sigaction (SIGALRM, &act, NULL);

        UneRequeteE.Numero = numRequete;
        UneRequeteE.NumeroFacture = 0;
        UneRequeteE.Date = 0;
        UneRequeteE.Reference = 0;
        UneRequeteE.Quantite = 0;
        UneRequeteE.Prix = 0;
        strcpy(UneRequeteE.Marque, "/");
        strcpy(UneRequeteE.Modele, "/");
        strcpy(UneRequeteE.Transmission, "/");
        strcpy(UneRequeteE.NomClient, "/");

        int reask = 1;
        while (reask == 1) {
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
                    UneRequeteE.Type = Question;

                    printf("----------------------  \n");
                    do {
                        printf("Reference : ");
                        fflush(stdin);
                        fgets(Tampon, sizeof Tampon, stdin);
                        UneRequeteE.Reference = atoi(Tampon);
                    } while (UneRequeteE.Reference <= 0);
                    printf("----------------------  \n");

                    reask = 0;
                    break;
                }

                case Achat: {
                    struct FactureAG UneFacture;
                    UneRequeteE.Type = Achat;
                    saisieFactureAG(0, &UneFacture);

                    strcpy(UneRequeteE.NomClient, UneFacture.NomClient);
                    UneRequeteE.Date = UneFacture.DateFacturation;
                    UneRequeteE.Reference = UneFacture.Reference;
                    UneRequeteE.Quantite = UneFacture.Quantite;

                    reask = 0;
                    break;
                }

                case 3:
                    close(flog);
                    close(Desc);
                    exit(0);
                    break;

                default:
                    reask = 1;
            }
        }

        printf("Souhaitez-vous envoyer cette requete ? (Y/N) \n");
        printf(">> ");

        fflush(stdin);
        char confirm = getchar();
        printf("----------------------  \n");

        if (confirm == 'y' || confirm == 'Y') {
            redo:                       // Quand on veut retransmettre un message suite à une expiration de l'alarme ou timeout, on fait un branchement ici.
            timeInterval = 9;           // On fixe un délai de 9 secondes pour le timeout et l'appel de la fonction alarm provoquera ce timeout dans 9 secondes.
            alarm(timeInterval);        // J'envoie le message 0, je déclenche le timer.

            rc = SendDatagram(Desc, &UneRequeteE, sizeof(struct RequeteAG), &sos);

            if (rc == -1)
                die("SendDatagram");
            else
                printf("bytes envoyes: %d - num req: %d \n", rc, UneRequeteE.Numero);

            while (1) {
                memset(&UneRequeteR, 0, sizeof(struct RequeteAG));
                tm = sizeof(struct RequeteAG);

                rc = ReceiveDatagram(Desc, &UneRequeteR, tm, &sor);
                if (rc <= 0) {
                    // die("ReceiveDatagram");
                    perror("error sur receive: \n");
                    fprintf(stderr, "rc %d errno:%d \n", rc, errno);
                    printf("rc %d errno:%d \n", rc, errno);
                    goto redo;
                } else {
                    // printf("[DEBUG] Num req envoyee: %d - Num req recue: %d - Num requete (compteur): %d \n", UneRequeteE.Numero, UneRequeteR.Numero, numRequete);
                    if (UneRequeteR.Numero != UneRequeteE.Numero) {
                        printf("doublon ! \n");
                    } else {
                        break;
                    }
                }
            }
            // fin de l'alarme
            alarm(0);
            printf("bytes lus:%d \n", rc);
            printf("Marque: %s - Modele: %s \n", UneRequeteR.Marque, UneRequeteR.Modele);

            fprintf(stderr, "bytes recus: %d - num req: %d \n", rc, UneRequeteR.Numero);
            afficheRequeteAG(stderr, UneRequeteR);
            fprintf(stderr, "\n\n");

            if (UneRequeteE.Type == Achat) {
                if (UneRequeteR.Type == OK) {
                    printf("Achat Reussi Facture: %d \n", UneRequeteR.NumeroFacture);
                    fprintf(stderr, "Success: La facture (%d) a ete cree ! \n", UneRequeteR.NumeroFacture);
                } else {
                    fprintf(stderr, "Erreur: Le vehicule n'existe pas ou la quantite entree est trop elevee ! \n");
                }
            }
        }

        numRequete++;
        sleep(5);
    }

    close(flog);
    close(Desc);
}

void die(char *s) {
    perror(s);
    exit(1);
}