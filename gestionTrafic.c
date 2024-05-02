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
  int id;
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

int compareTime(const void *a, const void *b) {
    const car *carA = (const car *)a;
    const car *carB = (const car *)b;
    return carA->time - carB->time;
}

void printCarList(car* carList, int carNum) {
    for (int i=0; i<carNum; ++i) {
      printf("V%d ", carList[i].id);
    }
    printf("\n");
}

int parseCars(car* carList, char* args, int argSize) {
  int carNum = -1;

  for (int i=0; i<argSize; ++i) {
    
    // Si lettre nouvelle case
    if (isdigit(args[i]) == 0 ) {
      carNum ++;
      carList[carNum].time = 1;
      carList[carNum].id = carNum;
      printf("La voiture %d Arrive\n", carNum+1);
      switch (args[i]) {
        default: 
          printf("ERREUR : Mauvaise entrée d'arguments");
          return 0;
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
      --i;
    }
  }
  return carNum+1;
}

int* sortCarList(car* carList, int* order, int carNum, int ord, int pri) {
  
  // 1 0
  if (ord && !pri) {
    for(int i=0; i<carNum; ++i) {
      order[i] = carList[i].id;
    }
    return order;
  }
  // 1 1  
  else if (ord && pri) {
    int orderCount = 0;
    for(int i=0; i<carNum; ++i) {
      if (carList[i].priority) {
        order[orderCount] = carList[i].id;
        ++orderCount;
      }
    }
    for(int i=0; i<carNum; ++i) {
      if (!carList[i].priority) {
        order[orderCount] = carList[i].id;
        ++orderCount;
      }
    }
    return order;
  }
  // 0 1
  else if (!ord && pri) {
    int orderCount = 0;
    qsort(carList, carNum, sizeof(car), compareTime);
    for(int i=0; i<carNum; ++i) {
      if (carList[i].priority) {
        order[orderCount] = carList[i].id;
        ++orderCount;
      }
    }
    for(int i=0; i<carNum; ++i) {
      if (!carList[i].priority) {
        order[orderCount] = carList[i].id;
        ++orderCount;
      }
    }
    return order;
  }
  // O O 
  else if (!ord && !pri) {
    int orderCount = 0;
    qsort(carList, carNum, sizeof(car), compareTime);
    for(int i=0; i<carNum; ++i) {
      order[orderCount] = carList[i].id;
      ++orderCount;
    }
    return order;
  }
  exit(EXIT_FAILURE);

}

void* carFunc( void* arg) {
  int id = *((int *)arg);
  sem_wait(&round);
  printf("La voiture %d Traverse\n", id+1);
  sem_post(&round);
  printf("La voiture %d Sort\n", id+1);
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

  while ((opt = getopt(argc, argv, "hso:p:a:")) != -1) {
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
        break;

      case 'a':
        argSize = strlen(optarg);
        args = malloc((argSize + 1) * sizeof(char)); 
        strcpy(args, optarg);
        break;
    }

  }
  
  if (ord > 1 || ord < 0 || pri < 0 || pri > 1 || !argSize) {
    displayHelp();
    return 0;
  }

  // ------------------------------------------- Début Programme -------------------------------

  //Initialisation de la liste
  car* carList = malloc(argSize * sizeof(car));
  
  //Remplissage de la liste
  carNum = parseCars(carList, args, argSize);
  if (!carNum) return 1;

  //Copie de la Liste
  car* saveList = malloc(carNum * sizeof(car));
  for (int i = 0; i < carNum; i++) {
        saveList[i] = carList[i];
    }
  //Initialisation de la liste d'ordre
  int* order = malloc(carNum * sizeof(int));

  //Remplissage de la liste d'ordre
  sortCarList(carList, order, carNum, ord, pri);

  //Initialisation du sémaphore
  sem_init(&round, 0, 1);

  //Initialisation des threads
  pthread_t threads[carNum];

  //creation des threads
  for (int i = 0; i < carNum; ++i) {
    pthread_create(&threads[i], NULL, carFunc, (void*)&order[i]);
  }

  //attende de la fin des threads
  for (int i = 0; i < carNum; ++i) {
    pthread_join(threads[i], NULL);
  }
  
  printf("\n");
  printf("Diagramme de Gantt :\n");
  for (int i=0; i<carNum; ++i) {
    for(int j=0; j<saveList[order[i]].time; ++j) {
      printf("V%d|", order[i]+1);
    }
  }
  printf("\n");

  free(carList);
  free(args);
  free(order);
  exit(EXIT_SUCCESS);
}
