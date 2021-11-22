/******************************************
  Herman Vanstapel
  2017 Basé sur Fichiers
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data.h"
#include "LibSerHV.h"

// include pour les fonctions entrees sortie
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

static struct termios old, new;
 
//clrscr() function definition
// #include <stdlib.h>
void ClrScr(void)
{
    system("clear");
}

void Gotoxy(int x,int y)    
{
    printf("%c[%d;%df",0x1B,y,x);
}

/* Initialize new terminal i/o settings */
// #include <stdio.h>
// #include <stdlib.h>

void initTermios(int echo) 
{
  tcgetattr(0, &old); //grab old terminal i/o settings
  new = old; //make new settings same as old settings
  new.c_lflag &= ~ICANON; //disable buffered i/o
  new.c_lflag &= echo ? ECHO : ~ECHO; //set echo mode
  tcsetattr(0, TCSANOW, &new); //apply terminal io settings
}
 
/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}
 
/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
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
char Getch(void) 
{
  return getch_(0);
}
 
/* 
Read 1 character with echo 
getche() function definition.
*/
char GetchE(void) 
{
  return getch_(1);
}



// permet de supprimer le cr qui placé dans la chaine lors d'un fgets
void DelNewLine(char *Chaine)
{

 Chaine[strlen(Chaine)-1] = 0 ;
}



void MonPrintf(char* tempo, int espace,int taille )
{
 int Count ;
 printf("%s",tempo) ; 
 Count = espace-taille ;
 while ( Count>0 )
 {
       printf(" ");
       Count-- ;
 }
}

void AfficheEnteteVehiculeHV ()
{
 char Tampon[80] ;
 sprintf(Tampon,"%s","Ref" ) ;MonPrintf(Tampon,4,strlen(Tampon)) ;
 sprintf(Tampon,"%s","Marque") ;    MonPrintf(Tampon,30,strlen(Tampon)) ;
 sprintf(Tampon,"%s","Modele") ;    MonPrintf(Tampon,30,strlen(Tampon)) ;
 sprintf(Tampon,"%s","Quantite") ; MonPrintf(Tampon,6,strlen(Tampon)) ;
 printf("\n") ;
}

void AfficheVehiculeHV (struct VehiculeHV    *UnRecord)
{
 char Tampon[80] ;
 sprintf(Tampon,"%d",UnRecord->Reference ) ;  MonPrintf(Tampon,4,strlen(Tampon)) ;
 sprintf(Tampon,"%s",UnRecord->Marque ) ;    MonPrintf(Tampon,30,strlen(Tampon)) ;
 sprintf(Tampon,"%s",UnRecord->Modele ) ;    MonPrintf(Tampon,30,strlen(Tampon)) ;
 sprintf(Tampon,"%d",UnRecord->Quantite ) ; MonPrintf(Tampon,6,strlen(Tampon)) ;
 printf("\n") ;
}

void SaiSieVehiculeHV (int Reference, struct VehiculeHV  *UnRecord )
{
 char Tampon[80] ;

 printf("Reference :%d \n",Reference) ;
 UnRecord->Reference = Reference ; 
 printf("Saisie Marque :") ;
 fgets(UnRecord->Marque ,sizeof UnRecord->Marque ,stdin ) ;
 printf("Saisie Modele :") ;
 fgets(UnRecord->Modele ,sizeof UnRecord->Modele ,stdin ) ;
 printf("Saisie Quantite :") ;
 fgets(Tampon,sizeof Tampon,stdin ) ;
 UnRecord -> Quantite = atoi(Tampon) ;
 
 DelNewLine(UnRecord->Marque) ;
 DelNewLine(UnRecord->Modele) ;
 AfficheEnteteVehiculeHV () ;
 AfficheVehiculeHV (UnRecord) ;
 printf("-----------------------\n") ;
 return ;
}

int NombreVehiculesHV (char *NomFichier)
{
 FILE *sortie ;
 sortie = fopen(NomFichier,"r") ; /* Si le fichier existe, on le cree sinon on ajoute */
 if ( sortie == NULL )
    {
     fprintf(stderr,"Pas de fichier\n") ;
     return 0 ;
    }
 else
    fprintf(stderr,"Ouverture reussie \n") ;
 fseek(sortie, 0, SEEK_END ) ;
 return (ftell(sortie)/ sizeof(struct VehiculeHV  )) ;
}

void CreationAjoutVehiculeHV (char *NomFichier,struct VehiculeHV  *UnRecord )
{
 FILE *sortie ;
 char Redo ;
 int nbr ;

 sortie = fopen(NomFichier,"a") ; /* Si le fichier existe, on le cree sinon on ajoute */
 if ( sortie == NULL )
    {
     fprintf(stderr,"Echec Ouverture\n") ;
     exit(0) ;
    }
 else
    fprintf(stderr,"Ouverture reussie \n") ;

 
  printf("Position actuelle dans le fichier %ld\n",ftell(sortie)) ;
  nbr = fwrite(UnRecord,sizeof(struct VehiculeHV  ),1,sortie) ;
  fflush(sortie) ;
 fclose(sortie) ;
}



void AfficheFacture(struct FactureHV *UneFacture)
{
 char Tampon[80] ;
 sprintf(Tampon,"%d",UneFacture->NumeroFacturation ) ; MonPrintf(Tampon,4,strlen(Tampon)) ;
 sprintf(Tampon,"%s",UneFacture->NomClient) ; MonPrintf(Tampon,60,strlen(Tampon)) ;
 sprintf(Tampon,"%d",UneFacture-> Quantite) ; MonPrintf(Tampon,4,strlen(Tampon)) ;
 sprintf(Tampon,"%d",UneFacture->Reference) ; MonPrintf(Tampon,4,strlen(Tampon)) ;
 sprintf(Tampon,"%d",UneFacture->DateFacturation ) ; MonPrintf(Tampon,8,strlen(Tampon)) ;
 
 printf("\n") ;
}

void ListingVehiculesHV (char *NomFichier)
{
 struct VehiculeHV   UnRecord ;
 FILE *sortie ;
 char Tampon[80] ;
 int  Numero ;
 int  nbr ;

 sortie = fopen(NomFichier,"r") ; /* Si le fichier existe, on le cree sinon on ajoute */
 if ( sortie == NULL )
    {
     fprintf(stderr,"Echec Ouverture\n") ;
     exit(0) ;
    }
 else
    fprintf(stderr,"Ouverture reussie \n") ;

 AfficheEnteteVehiculeHV () ;
 nbr = fread(&UnRecord,sizeof(UnRecord),1,sortie) ;
 
 while ( !feof(sortie) )
 {
  fprintf(stderr,"Record lu %d et Position actuelle dans le fichier %ld\n",nbr,ftell(sortie)) ;
  AfficheVehiculeHV ( &UnRecord) ;
  nbr = fread(&UnRecord,sizeof(UnRecord),1,sortie) ;
 }
 fclose(sortie) ;
}

void ListingFacturesHV(char *NomFichier)
{
 struct FactureHV UneFacture ;
 FILE *sortie ;
 char Tampon[80] ;
 int  Numero ;
 int  nbr ;

 sortie = fopen(NomFichier,"r") ; /* Si le fichier existe, on le cree sinon on ajoute */
 if ( sortie == NULL )
    {
     fprintf(stderr,"Echec Ouverture\n") ;
     exit(0) ;
    }
 else
    fprintf(stderr,"Ouverture reussie \n") ;

 
 nbr = fread(&UneFacture,sizeof(struct FactureHV),1,sortie) ;
 
 while ( !feof(sortie) )
 {
  fprintf(stderr,"Record lu %d et Position actuelle dans le fichier %ld\n",nbr,ftell(sortie)) ;
  AfficheFacture( &UneFacture) ;
  nbr = fread(&UneFacture,sizeof(struct FactureHV ),1,sortie) ;
 }
 fclose(sortie) ;
}

int main()
{
 char Choix ;
 char Tampon[80] ;
 int res ;
 struct VehiculeHV  UnRecord ;
 int Numero ;
 while(1)
 {
  printf("----------------------\n") ;
  printf("1) Ajout              \n") ;
  printf("2) Vehicule           \n") ;  
  printf("3) Recherche          \n") ;   
  printf("4) Achat              \n") ;  
  printf("5) Factures           \n") ;
  printf("6) A propos           \n") ;
  printf("7) exit               \n") ; 
  printf("----------------------\n") ; 
  printf(">>") ; Choix=GetchE() ; printf("\n") ;
  switch(Choix)
  {
   case '1': 
             {
              struct VehiculeHV   UnRecord ;
 	     FILE *sortie ;
             char Redo ;
		
             Redo='y' ;
 	    while ( Redo=='Y' || Redo=='y')
 	    { 
             int Nombre ;
             Nombre= NombreVehiculesHV ("VehiculesHV") ;
  	     SaiSieVehiculeHV (Nombre+1, &UnRecord ) ;
  	     CreationAjoutVehiculeHV ("VehiculesHV",&UnRecord) ; 
  	     printf("Encoder un autre (Y/N) ?)") ;
  	     printf(">>") ; Redo=GetchE() ; printf("\n") ;
 	   }
 
	     break ;	
             }
   case '2': ListingVehiculesHV ("VehiculesHV") ;
             break ;
   case '3': 
   case '5': ListingFacturesHV("FactureHV") ;
	     break ;
   case '6': AProposServeurHV("V 1","Herman","Vanstapel") ;
             break ;
   
	
   case '7': exit(0) ; 
  }
 }
}
