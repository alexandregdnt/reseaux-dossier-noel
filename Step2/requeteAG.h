#ifndef REQUETE
#define REQUETE

#include <stdio.h>

enum TypeRequeteAG {
    Question = 1,
    Achat = 2,
    Livraison= 3,
    OK = 4,
    Fail = 5
};

struct RequeteAG {
    enum TypeRequeteAG Type;
    int Numero; // Contient le numéro de la requete
    int NumeroFacture;
    int Date;
    int Reference; // la référence du film
    int Quantite;;
    int Prix;
    char Marque[30];
    char Modele[30];
    char NomClient[80];
};

void AfficheRequeteAG (FILE *fp, struct RequeteAG R) {
    fprintf(fp,">TypeRequete %d \n", R.Type );
    fprintf(fp," Reference %d \n", R.Numero );
    fprintf(fp," NumeroFacture %d \n", R.NumeroFacture );
    fprintf(fp," Date %d \n", R.Date );
    fprintf(fp," Reference %d \n", R.Reference );
    fprintf(fp," Places %d \n", R.Quantite );
    fprintf(fp," Prix %d \n", R.Prix );
    fprintf(fp," Marque %s \n", R.Marque );
    fprintf(fp," Modele %s \n", R.Modele );
    fprintf(fp," Client %s \n\n", R.NomClient );

    return;
}

#endif