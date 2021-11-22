
# cphex\makefile

LIBS=
all:	admin	LibSerAG.o


LibSerAG.o:	LibSerAG.c	LibSerAG.h	data.h
	echo "compilation de LibSerHV"
	gcc -c LibSerAG.c

admin:	data.h	admin.c	LibSerAG.o
	echo "Compilation de admin"
	gcc -o admin	admin.c	LibSerAG.o

