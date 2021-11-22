#ifdef DATAH

#else


#define DATAH

struct VehiculeHV { 
         int  Reference ;
	 char Marque[30] ;
   char Modele[30] ;
	 int  Quantite ;
         char Date[10] ;
	} ;

struct FactureHV
       {
         int NumeroFacturation ;
         char NomClient[40] ;
	 int DateFacturation ;
   int Quantite ;
	 int Reference ;
	} ;  	
#endif
