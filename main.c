#include "head.h"

char* path;
int type;

void treat(char* chemin, int type){

    switch(type){
        case SPECIAL:
            files = realloc (files, sizeof (char*) * ++numFichier);
            char* fichier = malloc(strlen(chemin)+1);
            strcpy(fichier,chemin);
            files[numFichier-1] = fichier;
            break;
        case PRINT:
            if(*(chemin + strlen(chemin)-1) != 46){
                files = realloc (files, sizeof (char*) * ++numFichier);
                char* fichier = malloc(strlen(chemin)+1);
                strcpy(fichier,chemin);
                files[numFichier-1] = fichier;
            }
            break;
    }
}

int main(int argc, char **argv){
    numFichier = 0;
    files = NULL;
    affichages = NULL;
    path = malloc(100);
    if(argc < 2) path =".";
    if(argc > 1) path = argv[1];
    type = SPECIAL;
    treat(path,type);
    if(*(path + strlen(path)-1) == '/' && strlen(path)>1) *(path + strlen(path)-1) = 0;
    type = PRINT;
    parcourir_repertoire(path, type, 0);
    files = realloc (files, sizeof (char*) * ++numFichier);
    affichages = malloc(sizeof(char*)*numFichier);
    for(int i =0; i < numFichier;i++){
        affichages[i] = malloc(100);
    }

    files[numFichier-1] = NULL; numFichier--;
    int nbrAffichages = 0; int numeroAffichage = 0;
    for (int j = 1; j < argc ; ++j) if(!strcmp(argv[j], "--print") || !strcmp(argv[j], "-l") || !strcmp(argv[j], "--exec")) nbrAffichages++;
    if(nbrAffichages == 0 ) {
        print(1,0);
    }
    if (argc > 2){
        int i = 2;
        while(i < argc){
            if(argv[i][0] == '-' ){
                if(strcmp(argv[i]+1, "-print") == 0){
                    type = PRINT;
                    if(nbrAffichages > 0) {print(++numeroAffichage == 1 ? 1 : 0 , 0);}
                }
                else if(strcmp(argv[i]+1, "-name") == 0){
                    type = NAME;
                    char* name = argv[++i];
                    search(name);
                }
                else if(strcmp(argv[i]+1, "t") == 0){
                    type = T;
                    char* name = argv[++i];
                    chercheExpress(name);
                }
                else if (strcmp(argv[i]+1, "l") == 0){
                    type = L;
                    if(nbrAffichages > 0) {print(++numeroAffichage == 1 ? 1 : 0 , 1 );}
                }
                else if (strcmp(argv[i]+1, "i") == 0){
                    type = I;
                    image();
                }
                else if(strcmp(argv[i]+1, "-exec") == 0){
                    commande = argv[++i];
                    type = EXEC;
                    if(strlen(commande)>0){
                        if(nbrAffichages > 0) {print(++numeroAffichage == 1 ? 1 : 0 , 2 );}
                    } else {
                        lireAffichage();
                    }
                }
            }

            i++;
        }
    }

    lireAffichage();
    return 0;
}
