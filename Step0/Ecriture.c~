/******************************************
  Herman Vanstapel
  2014 Basé sur Fichiers
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

void DelNewLine(char *Chaine)
{
 Chaine[strlen(Chaine)-1] = 0 ;
}

char ReadChar()
{
 char Tampon[80] ;
 fgets(Tampon,sizeof Tampon,stdin ) ;
 return Tampon[0] ;
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


void SaiSieRecord(struct Record *UnRecord )
{
 char Tampon[80] ;
 printf("Saisie Refernce :") ;
 fgets(Tampon,sizeof Tampon,stdin ) ;
 UnRecord -> Reference = atoi(Tampon) ;
 printf("Saisie Acheté :") ;
 fgets(Tampon,sizeof Tampon,stdin ) ;
 UnRecord -> Achete = atoi(Tampon) ;
 printf("Saisie Libellé :") ;
 fgets(UnRecord->Memo,sizeof UnRecord->Memo,stdin ) ;
 DelNewLine(UnRecord->Memo) ;
 printf("%d\n",UnRecord->Reference ) ;
 printf("%d\n",UnRecord->Achete ) ;
 printf("%s\n",UnRecord->Memo) ;
 UnRecord->Restant=UnRecord->Achete ;
 printf("-----------------------\n") ;
 return ;
}



void CreationAjoutFichier(char *NomFichier)
{
struct Record UnRecord ;
 FILE *sortie ;
 char Redo ;

 sortie = fopen(NomFichier,"a") ; /* Si le fichier existe, on le cree sinon on ajoute */
 if ( sortie == NULL )
    {
     fprintf(stderr,"Echec Ouverture\n") ;
     exit(0) ;
    }
 else
    fprintf(stderr,"Ouverture reussie \n") ;
 setvbuf(sortie, (char *)NULL, _IOLBF, 0) ; /* ceci supprime la bufferisation */
 Redo='y' ;
 while ( Redo=='Y' || Redo=='y')
 {
  int nbr ;
  printf("Position actuelle dans le fichier %ld\n",ftell(sortie)) ;
  SaiSieRecord(&UnRecord ) ;
  nbr = fwrite(&UnRecord,sizeof(UnRecord),1,sortie) ;
  fflush(sortie) ;
  fprintf(stderr,"%d Bytes écrits\n",nbr) ; /* affiche le nombre de record et pas de bytes */
  printf("Encoder un autre (Y/N) ?)") ;
  Redo=ReadChar() ;
 }
 fclose(sortie) ;
}

void AfficheRecord(struct Record *UnRecord)
{
 char Tampon[80] ;
 sprintf(Tampon,"%d",UnRecord->Reference ) ;  MonPrintf(Tampon,4,strlen(Tampon)) ;
 sprintf(Tampon,"%d",UnRecord->Achete ) ; MonPrintf(Tampon,5,strlen(Tampon)) ;
 sprintf(Tampon,"%s",UnRecord->Memo) ;    MonPrintf(Tampon,50,strlen(Tampon)) ;
 sprintf(Tampon,"%d",UnRecord->Restant) ; MonPrintf(Tampon,4,strlen(Tampon)) ;
 printf("\n") ;
}

void Listing(char *NomFichier)
{
 struct Record UnRecord ;
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

 
 nbr = fread(&UnRecord,sizeof(UnRecord),1,sortie) ;
 
 while ( !feof(sortie) )
 {
  fprintf(stderr,"Record lu %d et Position actuelle dans le fichier %ld\n",nbr,ftell(sortie)) ;
  AfficheRecord( &UnRecord) ;
  nbr = fread(&UnRecord,sizeof(UnRecord),1,sortie) ;
 }
 fclose(sortie) ;
}


main()
{
 char Choix ;
 char Tampon[80] ;
 int res ;
 struct Record UnRecord ;
 int Numero ;
 while(1)
 {
  printf("----------------------\n") ;
  printf("1) Ajout              \n") ;
  printf("2) Listing            \n") ;  
  printf("3) Recherche          \n") ;  
  printf("4) exit               \n") ; 
  printf("----------------------\n") ; 
  Choix=ReadChar() ;
  switch(Choix)
  {
   case '1': CreationAjoutFichier("Stock") ; 
	     break ;	
   case '2': Listing("Stock") ;
             break ;
   case '3': printf("Saisie numero :") ;
             fgets(Tampon,sizeof Tampon,stdin ) ;
             Numero = atoi(Tampon) ;
	     res = Recherche("Stock",Numero,&UnRecord) ;
	     if ( res == 1 )
		AfficheRecord(&UnRecord) ;	
             break ;			
   case '4': exit(0) ; 
  }
 }
}
