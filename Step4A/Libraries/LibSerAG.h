#ifndef LIBAG
#define LIBAG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data.h"

/* ========== VÉHICULES ========== */
int nombreVehiculesAG(char *);
void saisieVehiculeAG(int, struct VehiculeAG *);
void creationAjoutVehiculeAG(char *, struct VehiculeAG *);
int rechercheVehiculeAG(char *, int, struct VehiculeAG *);
int modifierVehiculeAG(char *, struct VehiculeAG *);

void afficheEnteteVehiculeAG();
void afficheVehiculeAG(struct VehiculeAG *);
void listingVehiculesAG(char *);

/* ========== FACTURES ========== */
int nombreFacturesAG(char *);
void saisieFactureAG(int, struct FactureAG *);
void creationAjoutFactureAG(char *, struct FactureAG *);

void afficheEnteteFactureAG();
void afficheFactureAG(struct FactureAG *);
void listingFacturesAG(char *);

/* ========== AUTRES FONCTIONS ========== */
void DelNewLine(char *);
void MonPrintf(char *, int, int);
void aProposServeurAG (char *, char *, char*);

#endif