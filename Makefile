# Makefile pour TP4.
# Adaptez ce fichier au besoin.

# Options standard.
#OPTIONS = -Wall

# L'option -g permet de générer les infos de débogage.
# Décommentez la ligne suivante si vous désirez utiliser un débogueur.
#OPTIONS = -g -O0 -Wall

# Les options -O, -O1, -O2, -O3 permettent d'optimiser le code binaire produit.
# Décommentez la ligne suivante si avant la remise finale
OPTIONS = -O2 -Wall

all: tp4

# Syntaxe : cible : dépendance1 dépendance2 ...
# Ensuite, la ou les ligne(s) débutant par une tabulation (\t) donne les commandes pour construire une cible
tp4: tp4.cpp succ.o date.o pointst.o
	g++ $(OPTIONS) -o tp4 tp4.cpp date.o succ.o pointst.o

date.o: date.h date.cpp
	g++ $(OPTIONS) -c -o date.o date.cpp

pointst.o: pointst.h pointst.cpp
	g++ $(OPTIONS) -c -o pointst.o pointst.cpp

succ.o: succ.h succ.cpp date.h pointst.h
	g++ $(OPTIONS) -c -o succ.o succ.cpp

testdate : testdate.cpp date.o
	g++ $(OPTIONS) -o testdate testdate.cpp date.o

clean :
	rm -f *.o
	rm -f tp4 testdate
	rm -f *~

