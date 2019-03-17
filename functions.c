#include "head.h"
#include <grp.h>
#include <pwd.h>
#include <time.h>


char** split1(char* chaine){
    char ** res  = NULL;
    char *  p    = strtok (chaine, " ");
    int n = 0;
    while (p) {
        res = realloc (res, sizeof (char*) * ++n);
        res[n-1] = p;
        p = strtok (NULL, " ");
    }
    res = realloc (res, sizeof (char*) * (n+1));
    res[n] = 0;
    return res;
}

char** split(char* chaine, char* delim){
    char ** res  = NULL;
    char *  p    = strtok (chaine, delim);
    int n = 0;
    while (p) {
        res = realloc (res, sizeof (char*) * ++n);
        res[n-1] = p;
        p = strtok (NULL, delim);
    }
    res = realloc (res, sizeof (char*) * (n+1));
    res[n] = 0;
    return res;
}

int nom_correspond(const char *chemin, const char *nom){
    char *fileName;
    fileName = strrchr(chemin, '/');
    if(fileName!=NULL){
        fileName += 1;
        if (strstr(nom,"*") != NULL){
            char *fileExtension; char *other;
            if (strstr(fileName,".") != NULL && strcmp(fileName,".") != 0){
                other = strrchr(fileName, '.');
                fileExtension =strchr(nom, '*')+1;
                if (strstr(fileExtension,other) != NULL) return 1;
            }
        }else return !strcmp(fileName,nom);
    }
    return 0;
}

void ls_l(const char *chemin,char ** resultat){
    struct stat fileStat;
    stat(chemin,&fileStat);
    struct group *grp;
    struct passwd *pwd;
    char* fichier = malloc(50);
    char tmp[50];
    strcpy(fichier,(S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    strcat(fichier,(fileStat.st_mode & S_IRUSR) ? "r" : "-");
    strcat(fichier,(fileStat.st_mode & S_IWUSR) ? "w" : "-");
    strcat(fichier,(fileStat.st_mode & S_IXUSR) ? "x" : "-");
    strcat(fichier,(fileStat.st_mode & S_IRGRP) ? "r" : "-");
    strcat(fichier,(fileStat.st_mode & S_IWGRP) ? "w" : "-");
    strcat(fichier,(fileStat.st_mode & S_IXGRP) ? "x" : "-");
    strcat(fichier,(fileStat.st_mode & S_IROTH) ? "r" : "-");
    strcat(fichier,(fileStat.st_mode & S_IWOTH) ? "w" : "-");
    strcat(fichier,(fileStat.st_mode & S_IXOTH) ? "x" : "-");
    //sprintf(tmp,". %hu",fileStat.st_nlink);
    strcat(fichier,tmp);
    pwd = getpwuid(fileStat.st_uid);
    sprintf(tmp," %s",pwd->pw_name);
    strcat(fichier,tmp);
    grp = getgrgid(fileStat.st_gid);
    sprintf(tmp," %s",grp->gr_name);
    strcat(fichier,tmp);
    //sprintf(tmp," %lld",fileStat.st_size);
    strcat(fichier,tmp);
    sprintf(tmp," %.12s",4+ctime(&fileStat.st_mtime));
    strcat(fichier,tmp);
    sprintf(tmp,"%s",chemin);
    strcat(fichier,tmp);
    strcpy(*resultat,fichier);
}

/*void execu(char ***cmd,char** resultat) {
    int fd[2];
    pid_t pid;
    pipe(fd);
    int statut;
    char character;
    
    if ((pid =fork())==0) {
        close(fd[0]);
        dup2(fd[1],1);
        execvp((*cmd[0]), *cmd);
        
    } else {
        
        waitpid(pid, &statut, 0);
        close(fd[1]);
        int i = 0;
        while(read(fd[0],&character,1)){
            (*resultat)[i++] = character;
        }
        (*resultat)[i-1] = 0;
        close(fd[0]);
    }
}*/
void execu(char ***cmd){
    int fd[2];
    pid_t pid;
    int fdd = 0;
    while (*cmd != NULL) {
        pipe(fd);
        if ((pid = fork()) == -1) {
            perror("fork");
            exit(1);
        }
        else if (pid == 0) {
            dup2(fdd, 0);
            if (*(cmd + 1) != NULL) {
                dup2(fd[1], 1);
            }
            close(fd[0]);
            execvp((*cmd)[0], *cmd);
            exit(1);
        }
        else {
            wait(NULL);
            close(fd[1]);
            fdd = fd[0];
            cmd++;
        }
    }
}
 
void execu2(char **cmd){
    int fd[2];
    pid_t pid;
    int fdd = 0;
    int k = 0;
    while (cmd[k] != NULL) {
        pipe(fd);
        if ((pid = fork()) == -1) {
            perror("fork");
            exit(1);
        }
        else if (pid == 0) {
            dup2(fdd, 0);
            if (cmd[k+1] != NULL) {
                dup2(fd[1], 1);
            }
            close(fd[0]);
            execvp(cmd[0], cmd);
            exit(1);
        }
        else {
            wait(NULL);
            close(fd[1]);
            fdd = fd[0];
            k++;
        }
    }

}


void executer(char* input, char** resultat){
    char* maCommande = malloc(50);
    strcpy(maCommande,commande);
    //char** cmd = split(maCommande,"|");
    
    if (strchr(maCommande,'|')!=NULL){
        char** cmd = split(maCommande,"|");
        char** cmd2 = NULL;
        char* second = NULL;
        bool flag = false;
        bool param = false;
        int z = 0;
        for (z = 0; cmd[z]!=NULL; z++){} // размер z
         cmd2 = split1(cmd[0]);
         int k = 0;
         for(; cmd2[k] != NULL; ++k){
             if (strchr(cmd2[k],'-')!=NULL){
                 param = true;
             }
         }
        if (param){
            char *ls[] = {cmd2[0],cmd2[1],input,NULL};
            cmd2 = split1(cmd[1]);
            char *cat[] = {cmd2[0],cmd2[1],NULL};
            if (z == 3){
                cmd2 = split1(cmd[2]);
                char *cat2[] = {cmd2[0],cmd2[1],NULL};
                char **cmdRez[] = {ls,cat,cat2,NULL};
                execu(cmdRez);
                free(maCommande);
            }
           else if (z==4){
                cmd2 = split1(cmd[2]);
                char *cat2[] = {cmd2[0],cmd2[1],NULL};
                cmd2 = split1(cmd[3]);
                char *cat3[] = {cmd2[0],cmd2[1],NULL};
                char **cmdRez[] = {ls,cat,cat2,cat3,NULL};
                execu(cmdRez);
                free(maCommande);
            }
            else{
            char **cmdRez[] = {ls,cat, NULL};
            execu(cmdRez);
            free(maCommande);
            }
        }else if(!param){
            char *ls[] = {cmd2[0],input,NULL};
            cmd2 = split1(cmd[1]);
            char *cat[] = {cmd2[0],cmd2[1],NULL};
            if (z == 3){
                cmd2 = split1(cmd[2]);
                char *cat2[] = {cmd2[0],cmd2[1],NULL};
                char **cmdRez[] = {ls,cat,cat2,NULL};
                execu(cmdRez);
                free(maCommande);
            }
            else if (z==4){
                cmd2 = split1(cmd[2]);
                char *cat2[] = {cmd2[0],cmd2[1],NULL};
                cmd2 = split1(cmd[3]);
                char *cat3[] = {cmd2[0],cmd2[1],NULL};
                char **cmdRez[] = {ls,cat,cat2,cat3,NULL};
                execu(cmdRez);
                free(maCommande);
            }
            else{
                char **cmdRez[] = {ls,cat, NULL};
                execu(cmdRez);
                free(maCommande);
            }
        }

    }else{
        char** cmd = split1(maCommande);
        int k = 0;
        bool flag = false;
        while(cmd[k]!=NULL){
            if (strcmp(cmd[k],"{}")==0){
                cmd[k] = input;
                flag = true;
            }
            k++;
        }
        if (!flag){
            cmd[k] = input;
        }
       execu2(cmd);
       free(maCommande);
    }
}

void parametreToFunc(int fonction,char* input, char** resultat){
    if(fonction == 0) strcpy(*resultat,input);
    else if(fonction == 1) ls_l(input,resultat);
    else executer(input,resultat);
}

void lireAffichage (){
    for (int i = 0; i < numFichier; ++i) {
        if(files[i] != NULL)printf("%s\n",affichages[i]);
    }
}

void print(int firstTime, int fonction){
    for (int i = 0; i < numFichier; ++i) {
        if(files[i] != NULL){
            if(firstTime){
                parametreToFunc(fonction,files[i],&(affichages[i]));
            } else {
                int n = strlen(affichages[i]);
                *(affichages[i]+n) = '\n';
                char* pointeur = affichages[i]+n+1;
                parametreToFunc(fonction,files[i],&pointeur);
            }
        } else {
            affichages[i] = NULL;
        }
    }
}


void search(char* name){
    for (int i = 0; i < numFichier; ++i) {
        if(!nom_correspond(files[i],name)) files[i] = NULL;
    }
}

int expressDansFichier(char* fichier,char* express){
    int fp = open(fichier, O_RDONLY);
    int n = strlen(express);
    char buf[n];
    while (read(fp,buf,n)==n){
        lseek(fp,1-n,SEEK_CUR);
        if (!strcmp(buf,express)) return 1;
    }
    close(fp);
    return 0;
    
}


void chercheExpress(char* express){
    for (int i = 0; i < numFichier; ++i) {
        if(files[i] != NULL){
            if(!expressDansFichier(files[i],express)) files[i] = NULL;
        }
    }
}

/*int isImage(char* fichier){
 void *handle;
 void* (*magic_open)(int);
 void (*magic_close)(void*);
 char* (*magic_file)(void*,char*);
 int (*magic_load)(void*,char*);
 handle = dlopen("libmagic.so", RTLD_LAZY | RTLD_DEEPBIND);
 magic_open = (void* (*)(int)) dlsym(handle, "magic_open");
 magic_close = (void (*)(void*)) dlsym(handle, "magic_close");
 magic_file = (char* (*)(void*,char*)) dlsym(handle, "magic_file");
 magic_load = (int (*)(void*,char*)) dlsym(handle, "magic_load");
 dlsym(handle,"MAGIC_MIME_TYPE");
 void* cookie = (*magic_open)(16);
 (*magic_load)(cookie,NULL);
 char* info = (*magic_file)(cookie, fichier);
 (*magic_close)(0);
 dlclose(handle);
 return strncmp(info,"image/",6);
 //return 0;
 }*/

void image(){
    /* for (int i = 0; i < numFichier; ++i) {
     if(files[i] != NULL){
     if(isImage(files[i])) files[i] = NULL;
     }
     }*/
}
