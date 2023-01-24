/*
  Projet minishell - Licence 3 Info - PSI 2022
 
  Nom : Vallée, Dubreuil
  Prénom : Léo, Alexandre
  Num. étudiant : 22003214, 22011105 
  Groupe de projet : 6
  Date : 18/12/2022
 
  Fichier builtin.c : Gestion des commandes internes du minishell (implémentation) comme 'cd', 'export' ou 'exit'.
  Auteur : Vallée, Dubreuil
  Dépendances : processus.h
 */

#include "processus.h"

#include <string.h>

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

/*
  Fonction is_builtin
  Paramètre cmd : La chaine qui est égal à la commande saisit par l'utilisateur.
  Retourne un entier, 1 dans ce cas trouvé ou 0 si pas trouvé.
*/

int is_builtin(const char * cmd) {
  if (strcmp(cmd, "cd") == 0) {
    return 1;
  } else {
    if (strcmp(cmd, "exit") == 0) {
      return 1;
    } else {
      if (strcmp(cmd, "export") == 0) {
        return 1;
      } else {
      	if (strcmp(cmd, "history") == 0) {
        return 1;
        } else {
         return 0;
      	}
      }
    }
  }
}

/*
  Fonction cd
  Paramètre path : La chaine qui est égal au argument de la commande cd.
  Paramètre fderr : entier
  Retourne un entier, 0 dans le cas bien ou -1 si ne marche pas.
*/

int cd(const char * path, int fderr) {
  int error = 0;

  error = chdir(path);
  if (error == -1) { // Erreur
    //write(fderr, "Erreur cd\n", strlen("Erreur cd\n")); // Avec le message d’erreur adéquat
    return -1;
  }
  return error;
}

/*
  Fonction export
  Paramètre var : La chaine qui est égal au 1er argument de la commande export.
  Paramètre value : La chaine qui est égal au 2e argument de la commande export.
  Paramètre fderr : entier
  Retourne un entier, 0 dans le cas bien ou -1 si ne marche pas.
*/

int export (const char *var,const char * value, int fderr) {
  int error=0;
  error = setenv(var, value, 1);
  if (error == -1) {
    return -1;
  }
  return error;
}

/*
  Fonction exit
  Paramètre ret : Entier d'exit.
  Paramètre fderr : entier
  Retourne un entier : -1.
*/

int exit_shell(int ret, int fderr) {
  printf("Arret du minishell code erreur : %d \n", ret);
  return -1;
}
int history() {
int error = 0;
char *cmd[]={"cat","/home/leo/Documents/S5/shell-Vallee_Dubreuil/history",NULL};
execvp("cat",cmd);
return -1;
}
/*
  Fonction builtin
  Paramètre proc : Structure de processus qui possède toute les commandes.
  Retourne un entier : 0 dans le cas pas trouvé et sinon retourne l'entier de la fonction trouvée.
*/

int builtin(processus_t * proc) {

  if (strcmp(proc -> path, "cd") == 0) {
    return cd(proc -> argv[1], proc -> stderr);
  }

  if (strcmp(proc -> path, "export") == 0) {
    return export (proc -> argv[1], proc -> argv[3], proc -> stderr);
  }

  if (strcmp(proc -> path, "exit") == 0) {
    return exit_shell(1, proc -> stderr);
  }
  
  if (strcmp(proc -> path, "history") == 0) {
    return history(1, proc -> stderr);
  }
  return 0;
}
