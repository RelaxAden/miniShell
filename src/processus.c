/*
  Projet minishell - Licence 3 Info - PSI 2022
 
  Nom : Vallée, Dubreuil
  Prénom : Léo, Alexandre
  Num. étudiant : 22003214, 22011105
  Groupe de projet : 6
  Date : 18/12/2022
 
  Fichier processus.c : Gestion des processus (implémentation), exécution des commandes en parcourant la structure, initialisation de la structure.
  Auteur : Vallée, Dubreuil
  Dépendances : builtin.h, processus.h
 */

#include "processus.h"

#include "builtin.h"

#include <string.h>

#include <stdio.h>

#include <stdlib.h>

#include <sys/wait.h>

#include <fcntl.h>

#include <sys/types.h>

#include <sys/stat.h>

#define MAX_CMD_SIZE 256
#define MAX_LINE_SIZE 1024

/*
  Fonction exec_process
  Paramètre p : La structure de processus qui possède toute les commandes saisit par l'utilisateur.
  Retourne un entier, 0 dans le cas bien ou -2 si le cas exit ou retourne error qui peut être de 0 ou -1;
*/

int exec_process(processus_t * p) {
  int error = 0;
  p -> pid = fork();
  FILE *f;
  int i = 0;
  f = fopen("history","a");
  if (f==NULL) {
  fprintf(stderr,"Erreur lors de l'ouverture de l'history");
  return -1;
  }
  
  if (p -> pid == 0 && p -> wait != 0) { // fils
    if (is_builtin(p -> path)) {
      // cas 'cd' 'export' ...
      i=0;
        while (p->argv[i] != NULL) {
        fputs(p->argv[i],f);
        fputs(" ",f);
        i++;
        }
        fputs("\n",f);
        fclose(f);
      error = builtin(p);
      if (error == -1 && strcmp(p -> path, "exit") == 0) {
        return -2;
      } else {
        if (error == -1 && strcmp(p -> path, "cd") == 0) {
          write(p -> stderr, "Erreur cd\n", strlen("Erreur cd\n"));
        } else {
          if (error == -1 && strcmp(p -> path, "export") == 0) {
            write(p -> stderr, "Erreur export\n", strlen("Erreur export\n"));
          } else {
            if (error == -1 && strcmp(p -> path, "history") == 0) {
              write(p -> stderr, "Erreur history\n", strlen("Erreur history\n"));
            }
          }
        }
      }
      return error;
    } else {
      if (p -> redirect != NULL) {
        // cas >
        if (p -> redirect_double == 0) {
          // history
          i=0;
          while (p->argv[i] != NULL) {
            fputs(p->argv[i],f);
            fputs(" ",f);
            i++;
          }
          fputs("> ",f);
          fputs(p->redirect,f);
          fputs("\n",f);
          fclose(f);
          
          int fd = open(p -> redirect, O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
          dup2(fd, 1);
          close(fd);
          execvp(p -> path, p -> argv);
          return 0;
        } else {
         // history
      	  i=0;
          while (p->argv[i] != NULL) {
            fputs(p->argv[i],f);
            fputs(" ",f);
            i++;
          }
          fputs(">> ",f);
          fputs(p->redirect,f);
          fputs("\n",f);
          fclose(f);
          
          int fd = open(p -> redirect, O_WRONLY | O_APPEND | O_CREAT, 0644);
          dup2(fd, 1);
          close(fd);
          execvp(p -> path, p -> argv);
          return 0;
        }
      } else {
        // cas basique!
        i=0;
        while (p->argv[i] != NULL) {
        fputs(p->argv[i],f);
        fputs(" ",f);
        i++;
        }
        fputs("\n",f);
        fclose(f);
        error = execvp(p -> path, p -> argv);
        if (error == -1) write(p -> stderr, "Erreur commande\n", strlen("Erreur commande\n"));
        return 0;
      }
    }
  } else {
    if (p -> pid == 0 && p -> wait == 0) {
      // cas '&' 
      printf("Je suis le cas en arriere plan\n");
      i=0;
        while (p->argv[i] != NULL) {
        fputs(p->argv[i],f);
        fputs(" ",f);
        i++;
        }
        fputs("\n",f);
        fclose(f);
      error = execvp(p -> path, p -> argv);
      return error;

    } else {
      if (p -> wait == 1) {
        waitpid(-1, & p -> status, 0);
      }
    }
  }

  return 0;
}

/*
  Fonction init_process
  Paramètre p : La structure de processus qui possède toute les commandes saisit par l'utilisateur.
  Retourne un entier, 0.
*/

int init_process(processus_t * p) {

  int i = 0;
  while (p[i].path != NULL) {
    memset(p[i].argv, 0, MAX_CMD_SIZE * sizeof(char * ));
    i++;
  }
  for (int j = 0; j < i; j++) {
    p[j].path = NULL;
    p[j].redirect = NULL;
  }
  memset(p, 0, sizeof( * p));

  p -> stdout = 1;
  p -> stdin = 0;
  p -> stderr = 2;
  p -> redirect_double = 0;
  p -> fdclose[0] = 1;
  for (int i = 0; i < MAX_CMD_SIZE; i++) {
    p[i].wait = 1;

  }
  return 0;
}

/*
  Fonction parse_cmd
  Paramètre tokens : Un tableau qui possède tous les tokens.
  Paramètre processes : La structure de processus_t.
  Paramètre max : La taille du tableau de tokens
  Retourne un entier, 0.
*/

int parse_cmd(char * tokens[], processus_t * processes, size_t max) {

  int k = 0;
  int x = 0;
  int path_cond = 0;

  // Cas ou rien
  if (tokens[0][0] == 0) return 0;

  if (strcmp(tokens[max - 1], "; ") == 0 || strcmp(tokens[max - 1], ";") == 0) {
    // CAS ou on fini par ; ex : echo test; echo ok;
    max = max - 1;
  }

  for (int i = 0; i < max; i++) {

    if (path_cond == 0) {
      // 1er cas
      processes[k].path = tokens[i];
      path_cond = 1;
    }
    if (strcmp(tokens[i], ";") == 0) {
      // Cas ;
      k++;
      x = 0;
      path_cond = 0;
    } else {

      if ((strcmp(tokens[i], "&") == 0 || strcmp(tokens[i], "& ") == 0)) {
        // Cas && et &
        if (i + 1 == max) {
          // cas & et fin
          processes[k].wait = 0;
          k++;
          x = 0;
          path_cond = 0;

        } else {
          if (strcmp(tokens[i + 1], "&") == 0 || strcmp(tokens[i + 1], "& ") == 0) {
            // cas & &
            k++;
            x = 0;
            path_cond = 0;
            i++;
          } else {
            // cas & et autre chose 
            processes[k].wait = 0;
            k++;
            x = 0;
            path_cond = 0;
          }
        }

      } else {
        if (strcmp(tokens[i], ">") == 0) {
          if (strcmp(tokens[i + 1], ">") == 0) {
            // cas >>
            processes[k].redirect = tokens[i + 2];
            processes[k].redirect_double = 1;
            i = i + 2;
          } else {
            // cas redirection >
            processes[k].redirect = tokens[i + 1];
            processes[k].redirect_double = 0;
            i++;
          }

        } else {
          processes[k].argv[x] = tokens[i];
          x++;
        }

      }
    }
  }
  return 0;
}
