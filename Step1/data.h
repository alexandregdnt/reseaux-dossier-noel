#ifndef DATAH
#define DATAH

struct VehiculeAG {
    int  Reference;
    char Marque[30];
    char Modele[30];
    char Transmition[10];
    int  Quantite;
    char Date[10];
};

struct FactureAG{
    int NumeroFacturation;
    char NomClient[40];
    int DateFacturation;
    int Quantite;
    int Reference;
};

#endif
