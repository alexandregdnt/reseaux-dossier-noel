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
    struct VehiculeAG UnRecordV;
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
        printf("CreateSockets %d\n", Desc);

    tm = sizeof(struct RequeteAG);
    rc = ReceiveDatagram(Desc, &UneRequete, tm, &sor);
    if (rc == -1)
        die("ReceiveDatagram");
    else {
        printf("bytes recus:%d - Reference: %d\n", rc, UneRequete.Reference);
        AfficheRequeteAG(stderr, UneRequete);
    }

    //printf("Type recu %d\n", UneRequete.Type);
    /* attention l'enum peut être codé en short */
    /* reponse avec psos */

    if (RechercheAG("VehiculesAG", UneRequete.Reference, &UnRecordV) == 1) {
        UneRequete.Type = OK;
        UneRequete.Numero = 0;
        UneRequete.NumeroFacture = 0;
        UneRequete.Date = 0;
        UneRequete.Quantite = 0;
        UneRequete.Prix = 0;
        strcpy(UneRequete.Marque, UnRecordV.Marque);
        strcpy(UneRequete.Modele, UnRecordV.Modele);
        strcpy(UneRequete.Transmission, UnRecordV.Transmission);
        strcpy(UneRequete.NomClient, "/");
    } else {
        UneRequete.Type = Fail;
        fprintf(stderr, "Rien a ete trouve \n");
    }

    //strcat(UneRequete.Message, " Client");
    rc = SendDatagram(Desc, &UneRequete, sizeof(struct RequeteAG), &sor);
    if (rc == -1) {
        fprintf(stdout, "response: 0 \n");
        die("SendDatagram:");
    } else {
        printf("bytes envoyes:%d\n", rc);
        fprintf(stdout, "response: 1 \n");
        AfficheRequeteAG(stderr, UneRequete);
    }

    close(flog);
    close(Desc);
}

void die(char *s) {
    perror(s);
    exit(1);
}