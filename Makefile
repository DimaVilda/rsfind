rsfind: main.o functions.o parcours.o
	gcc parcours.o functions.o main.o -o rsfind
   
main.o: main.c head.h
	gcc -c main.c
	
functions.o: functions.c head.h
	gcc -c functions.c
	
parcours.o: parcours.c head.h
	gcc -c parcours.c