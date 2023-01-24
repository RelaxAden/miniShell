/*
  Projet minishell - Licence 3 Info - PSI 2022
 
  Nom : Vallée, Dubreuil
  Prénom : Léo, Alexandre
  Num. étudiant : 22003214, 22011105
  Groupe de projet : 6
  Date : 18/12/2022
 
  Parsing de la ligne de commandes utilisateur.
 
 */

#ifndef _PARSER_H
#define _PARSER_H

#define MAX_LINE_SIZE   1024
#define MAX_CMD_SIZE    256

/*
  Voir le TP sur les chaînes de caractères.
  Commentaires à compléter.
*/

int trim(char* str);
int clean(char* str);
int separate_s(char* str, char* s, size_t max);
int substenv(char* str, size_t max);

int strcut(char* str, char sep, char** tokens, size_t max);

// Optionnel
int strcut_s(char* str, char sep, char** tokens, size_t max);
#endif
