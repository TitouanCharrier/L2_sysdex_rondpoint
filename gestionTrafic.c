#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>

#define ARG_MAX_SIZE 1024

typedef struct car {
  int priority;
  int time;
  char road;
}car;

sem_t round;

void printCar(car liste[], int taille) {
    for (int i = 0; i < taille; ++i) {
        printf("Voiture %d:\n", i);
        printf("Priorité : %d\n", liste[i].priority);
        printf("Temps : %d\n", liste[i].time);
        printf("Route : %c\n\n", liste[i].road);
    }
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

int parseCars(car* carList, char* args, int argSize) {

  int carNum = -1;

  for (int i=0; i<argSize; ++i) {
    
    // Si lettre nouvelle case
    if (isdigit(args[i]) == 0 ) {
      carNum ++;
      carList[carNum].time = 1;
      switch (args[i]) {
        default: 
          printf("ERREUR : Mauvaise entrée d'arguments");
          return;
        case 'a' :
          carList[carNum].road = 'a';
          carList[carNum].priority = 0;
          break;
        case 'r' :
          carList[carNum].road = 'r';
          carList[carNum].priority = 0;
          break;
        case 'c' :
          carList[carNum].road = 'c';
          carList[carNum].priority = 0;
          break;
        case 'e' :
          carList[carNum].road = 'e';
          carList[carNum].priority = 0;
          break;
        case 'j' :
          carList[carNum].road = 'j';
          carList[carNum].priority = 0;
          break;
        case 'A' :
          carList[carNum].road = 'a';
          carList[carNum].priority = 1;
          break;
        case 'R' :
          carList[carNum].road = 'r';
          carList[carNum].priority = 1;
          break;
        case 'C' :
          carList[carNum].road = 'c';
          carList[carNum].priority = 1;
          break;
        case 'E' :
          carList[carNum].road = 'e';
          carList[carNum].priority = 1;
          break;
        case 'J' :
          carList[carNum].road = 'j';
          carList[carNum].priority = 1;
          break;
      }
    }

    // Si chiffre assignation
    else if (isdigit(args[i]) != 0) {
      carList[carNum].time = atoi(&args[i]);
      while (isdigit(args[i]) != 0) {
        ++i;
      }
      i--;
    }
  }
  return carNum+1;
}

void* carFunc( void* arg) {
  int id = *((int *)arg);
  printf("La voiture %d Arrive\n", id);
  sem_wait(&round);
  printf("La voiture %d Traverse\n", id);
  sem_post(&round);
  printf("La voiture %d Sort\n", id);
  pthread_exit(NULL);
}

// ------------------------------------------------- main ----------------------------------------
int main(int argc, char *argv[]) {

  
  //---------------------------------- Prise en compte des arguments ------------------------------

  //Définition des variables principales
  int ord = 0;
  int pri = 0;
  int opt = 0; 
  int argSize = 0;
  char* args;
  int carNum;

  while ((opt = getopt(argc, argv, "hrsq:a:")) != -1) {
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

  // ------------------------------------------- Début Programme -------------------------------

  //Initialisation de la liste
  car* carList = malloc(argSize * sizeof(car*));

  //Remplissage de la liste
  carNum = parseCars(carList, args, argSize);

  //Initialisation du sémaphore
  sem_init(&round, 0, 1);

  //Initialisation de la liste des id
  int thread_id[carNum];

  //Initialisation des threads
  pthread_t threads[carNum];

  //creation des threads
  for (int i = 0; i < carNum; ++i) {
    thread_id[i] = i;
    pthread_create(&threads[i], NULL, carFunc, (void*)&thread_id[i]);
  }

  //attende de la fin des threads
  for (int i = 0; i < carNum; ++i) {
    pthread_join(threads[i], NULL);
  }




  printf("%s\n", args);
  printf("%d\n", argSize);
  printCar(carList, carNum);
}
