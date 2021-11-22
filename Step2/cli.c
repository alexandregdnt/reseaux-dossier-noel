/*---------------------------------------------------------------
   Alexandre Grodent
------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "Libraries/udplib/udplib.h"
#include "requeteAG.h"

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

    int redo;
    do {
        redo = 0;

        printf("----------------------                  \n");
        printf("Quel est le type de votre requete ?     \n");
        printf("1) Question                             \n");
        printf("2) Achat                                \n");
        printf("3) Livraison                            \n");
        printf("4) OK                                   \n");
        printf("5) Fail                                 \n");
        printf(">> ");

        fgets(Tampon, sizeof Tampon, stdin);
        UneRequete.Type = atoi(Tampon);

        switch (UneRequete.Type) {
            case Question:
            case Achat:
            case Livraison:
            case OK:
            case Fail:
                break;

            default:
                redo = 1;
        }
    } while (redo == 1);

    printf("----------------------  \n");
    do {
        printf("Numero de la requete : ");
        fgets(Tampon, sizeof Tampon, stdin);
        UneRequete.Numero = atoi(Tampon);
    } while (UneRequete.Numero <= 0);
    do {
        printf("Numero de facture : ");
        fgets(Tampon, sizeof Tampon, stdin);
        UneRequete.NumeroFacture = atoi(Tampon);
    } while (UneRequete.NumeroFacture <= 0);
    do {
        printf("Date : ");
        fgets(Tampon, sizeof Tampon, stdin);
        UneRequete.Date = atoi(Tampon);
    } while (UneRequete.Date <= 0);
    do {
        printf("Reference : ");
        fgets(Tampon, sizeof Tampon, stdin);
        UneRequete.Reference = atoi(Tampon);
    } while (UneRequete.Reference <= 0);
    do {
        printf("Quantite : ");
        fgets(Tampon, sizeof Tampon, stdin);
        UneRequete.Quantite = atoi(Tampon);
    } while (UneRequete.Quantite <= 0);
    do {
        printf("Prix : ");
        fgets(Tampon, sizeof Tampon, stdin);
        UneRequete.Prix = atoi(Tampon);
    } while (UneRequete.Prix <= 0);

    printf("Marque : ");
    fgets(UneRequete.Marque, sizeof UneRequete.Marque, stdin);

    printf("Modele : ");
    fgets(UneRequete.Modele, sizeof UneRequete.Modele, stdin);

    printf("Nom du client : ");
    fgets(UneRequete.NomClient, sizeof UneRequete.NomClient, stdin);
    printf("----------------------  \n");

    printf("----------------------  \n");
    printf("Verification des donnees et confirmation de l'envoi \n");
    AfficheRequeteAG(stdout, UneRequete);
    printf("Les informations sont elles correcte ? \n");
    printf("Souhaitez-vous envoyer cette requete ? (Y/N) \n");
    printf(">> ");
    char confirm = getchar();
    printf("----------------------  \n");

    if (confirm == 'y' || confirm == 'Y') {
        rc = SendDatagram(Desc, &UneRequete, sizeof(struct RequeteAG), &sos);

        if (rc == -1)
            die("SendDatagram");
        else
            fprintf(stderr, "Envoi de %d bytes\n", rc);

        memset(&UneRequete, 0, sizeof(struct RequeteAG));
        tm = sizeof(struct RequeteAG);

        rc = ReceiveDatagram(Desc, &UneRequete, tm, &sor);
        if (rc == -1)
            die("ReceiveDatagram");
        else {
            fprintf(stderr, "bytes recus:%d\n", rc);
            AfficheRequeteAG(stderr, UneRequete);
        }
    }

    close(flog);
    close(Desc);
}

void die(char *s) {
    perror(s);
    exit(1);
}