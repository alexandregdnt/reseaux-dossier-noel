.SILENT:

COMP = gcc

all: admin cli ser

admin: data.h admin.c LibSerAG.o
	echo "Compilation de admin"
	${COMP} -o admin admin.c LibSerAG.o

LibSerAG.o: LibSerAG.c LibSerAG.h data.h
	echo "compilation de LibSerAG"
	${COMP} -c LibSerAG.c

cli: cli.c requeteAG.h data.h udplib.o
	echo "Compilation de client"
	${COMP} -o cli cli.c udplib.o $(LIBS)

ser: ser.c requeteAG.h data.h udplib.o LibSerAG.o
	echo "Compilation de serveur"
	${COMP} -o ser ser.c udplib.o LibSerAG.o $(LIBS)

udplib.o: Libraries/udplib/udplib.h Libraries/udplib/udplib.c
	echo "Compilation de udplib.o"
	${COMP} -c Libraries/udplib/udplib.c

clearall: clear cleardata clearlogs

clear:
	echo "Suppression des fichiers compilés"
	rm -rf *.o admin cli ser

clearlogs:
	echo "Suppression des fichiers de logs"
	rm -rf *.log

cleardata:
	echo "Suppression des fichiers de donnees"
	rm -rf VehiculesAG