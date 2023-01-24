/*
  Projet minishell - Licence 3 Info - PSI 2022
 
  Nom : Vallée, Dubreuil
  Prénom : Léo, Alexandre
  Num. étudiant : 22003214, 22011105
  Groupe de projet : 6
  Date : 18/12/2022
 
  Fichier main.c : Interface du minishell.
  Auteur : Vallée, Dubreuil
  Dépendances : processus.h, parser.h, builtin.h
 
 */

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include "parser.h"

#include "processus.h"

#include "builtin.h"

int main(int argc, char * argv[]) {
  char cmdline[MAX_LINE_SIZE]; // buffer des lignes de commandes
  char * cmdtoks[MAX_CMD_SIZE]; // "mots" de la ligne de commandes
  processus_t processes[MAX_CMD_SIZE];
  processus_t * current;
  int i = 0;
  int cpt = 0; // calcul le nombre de tokens
  int prob = 0; // retour de la fct processus
  int cond_exit = 0; // pour stop
  while (cond_exit == 0) {
    cpt = 0;
    i = 0;
    // Initialisation
    memset(cmdline, '\0', MAX_LINE_SIZE);
    memset(cmdtoks, 0, MAX_CMD_SIZE * sizeof(char * ));

    // Initialiser les valeurs par défaut dans processes (stdin, stdout, stderr, ...)
    init_process(processes);
    // Afficher un prompt
    printf("$ ");

    // Lire une ligne dans cmdline - Attention fgets enregistre le \n final
    if (fgets(cmdline, MAX_LINE_SIZE, stdin) == NULL) break;
    cmdline[strlen(cmdline) - 1] = '\0';

    // Gérer la chaine saisit.
    clean(cmdline);
    separate_s(cmdline, ";!&|=><", MAX_LINE_SIZE);
    substenv(cmdline, MAX_LINE_SIZE);
    strcut(cmdline, ' ', cmdtoks, MAX_CMD_SIZE);

    // taille de commande
    i = 0;
    while (cmdtoks[i] != NULL) {
      cpt++;
      ++i;
    }
    parse_cmd(cmdtoks, processes, cpt);

    // Parcourir la structure et exécuter les commandes	
    current = processes;
    while (current -> path != NULL) {
      prob = exec_process(current);
      // Cas exit, erreur le prog ne veut pas stop
      if (prob == -2) {
        cond_exit = 1;
        break;
      }
      current++;
    }

    /*
    for (current=processes; current!=NULL; ) {
      exec_process(current);
      if (current->next!=NULL) {
	// Exécution inconditionnelle
	current=current->next;
	continue;
      }
      if (current->next_success!=NULL) {
	// Si la commande a réussi
	 current = current->next_success;
	continue;
      }
      if (current->next_failure!=NULL) {
	// Si la commande a échoué
	  current = current->next_failure;
      }
    }
    */
  }
  fprintf(stderr, "\nGood bye!\n");
  return 0;
}
