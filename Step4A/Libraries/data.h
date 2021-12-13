#ifndef DATAH
#define DATAH

struct VehiculeAG {
    int  Reference;
    char Marque[30];
    char Modele[30];
    char Transmission[10];
    int  Quantite;
    char Date[10];
};

struct FactureAG{
    int NumeroFacturation;
    char NomClient[40];
    int DateFacturation;
    int Reference;
    int Quantite;
};

#endif
