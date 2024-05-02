#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <ctype.h>

#define ARG_MAX_SIZE 1024

typedef struct car {
  int priority;
  int time;
}car;

// fonction de débug générée par ia 
// génère un fichier de débug appelé debug.log
// le fichier contient l'historique des événements
void int_log(const char *message, int value) {
    FILE *file = fopen("debug.log", "a"); // Ouvre le fichier en mode write
    if (file != NULL) {
        fprintf(file, "%s %d\n", message, value); // Écrit le message et l'entier dans le fichier
        fclose(file); // Ferme le fichier
    } else {
        printf("Impossible d'ouvrir le fichier de journalisation des débogages.\n");
    }
}

void debug_log(const char* message) {
  int_log(message, -100);
}



void displayHelp() {
  printf("Usage: ./gestionTrafic [OPTIONS]...\n L2 project sysdex\n\n");
  printf("Options:\n"); 
  printf("\t-h, --help affiche cette aide et quitte\n"); 
  printf("\t-s, --student affiche le nom des étudiants\n");
  printf("\t-o, --ordonnanceur O = [SJF] / 1 = [FCFS] default: 0\n");
  printf("\t-p, --priorité 0 = [pas de priorité] / 1 = [priorité] default: 0\n");
  printf("\t-a, --arguments suite de chiffres et de lettres \n");
}

int islegit(char arg) {
  if (arg == 'A' 
      || arg == 'a'
      || arg == 'R'
      || arg == 'r'
      || arg == 'C'
      || arg == 'c'
      || arg == 'e'
      || arg == 'E'
      || arg == 'J'
      || arg == 'j') 
  {
    return 1;
  }
  else return 0;
}


void parseCars(car* carList, char* args, int argSize, char previous) {
  for (int i=0; i<argSize; ++i) {
    
    //detect digits
    int digit = 0;
    if (isdigit(args[i]) == 0) {
      switch (previous) {
        default: 
          debug_log("ERREUR : Mauvaise entrée d'arguments");
          return;
        case 'a' :
          carList[0][num_count++]->time = atoi(&str[i]);
          break;
        case 'r' :
          carList[1][num_count++]->time = atoi(&str[i]);
          break;
        case 'c' :
          carList[2][num_count++]->time = atoi(&str[i]);
          break;
        case 'e' :
          carList[3][num_count++]->time = atoi(&str[i]);
          break;
        case 'j' :
          carList[4][num_count++]->time = atoi(&str[i]);
          break;
      }
      while (isdigit(str[i])) {
        i++;
      }

    }
    else if (islegit(args[i])) {
      debug_log("car cheked");
      debug_log(args[i]);
      if (isupper(args[i])) 

    }


        
  }
}

int main(int argc, char *argv[]) {

  
  //------------------------
  //---------- Prise en compte des arguments ------------------------------

  //Définition des variables principales
  int ord = 0;
  int pri = 0;
  int opt = 0; 
  int argSize = 0;
  char* args;

  debug_log("début du programme ----------------------------------------------------------------");

  while ((opt = getopt(argc, argv, "hrsq:a:")) != -1) {
    debug_log("enter the while arg");
    int_log("arg is : ", opt);
    switch (opt) {
      case 'h' :
        displayHelp();
        return 0;
    
      case 's' :
        printf("Work done by Alexy Bechade and Titouan Charrier (AKA les meilleurs)\n");
        return 0;

      case 'o' :
        ord = atoi(optarg);
        break;

      case 'p':
        pri = atoi(optarg);

      case 'a':
        argSize = strlen(optarg);
        args = malloc(argSize * sizeof(char));
        sprintf(args,optarg);

    }

  }
  
  if (ord > 1 || ord < 0 || pri < 0 || pri > 1) {
    displayHelp();
    return 0;
  }
  printf("%s\n", args);
  printf("%d\n", argSize);
}
