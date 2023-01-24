/*
  Projet minishell - Licence 3 Info - PSI 2022
 
  Nom : Vallée, Dubreuil
  Prénom : Léo, Alexandre
  Num. étudiant : 22003214, 22011105
  Groupe de projet : 6
  Date : 18/12/2022
 
  Fichier parser.c : Parsing de la ligne de commandes utilisateur (implémentation).
  Auteur : Vallée, Dubreuil
  Dépendances : 

 */

#include <stddef.h>

#include <string.h>

#include <stdio.h>

#include <stdlib.h>

/*
  Fonction trim
  Paramètre str : La chaine rentrée par l'utilisateur.
  Retourne un entier, 0 dans ce cas
*/

int trim(char * str) {
  return 0;
}

/*
  Fonction clean
  Paramètre str : La chaine rentrée par l'utilisateur.
  Retourne un entier, 0 dans ce cas bien ou -1 si NULL
*/

int clean(char * str) {
  // Cette fonction permet de trim(str) et clean(str)
  if (str == NULL) return -1;
  char temp[strlen(str)];
  memset(temp, 0, (strlen(str)) * sizeof(char));
  int k = 0;
  for (int i = 0; i < strlen(str); i++) {
    if (str[i] != ' ' || (str[i] == ' ' && str[i + 1] != ' ') || (str[i] != ' ' && str[i + 1] == '\0')) {
      temp[k] = str[i];
      k++;
    }
  }
  temp[k] = '\0';
  if (temp[0] == ' ') {
    strcpy(str, temp + 1);
  } else {
    strcpy(str, temp);
  }
  return 0;
}

/*
  Fonction separate_s
  Paramètre str : La chaine rentrée par l'utilisateur.
  Paramètre s : Un tableau de caractère à séparer.
  Paramètre max : La taille max de la chaine possible.
  Retourne un entier, 0 dans ce cas normal ou -1 si NULL.
*/

int separate_s(char * str, char * s, size_t max) {
  if (str == NULL) return -1;
  char temp[max];
  int k = 0;
  int cond = 0;
  int taille = strlen(str);
  int taille_s = strlen(s);
  memset(temp, '\0', max);
  for (int i = 0; i < taille; i++) {

    for (int j = 0; j < taille_s; j++) {

      if (s[j] == str[0] && i == 0) {
        temp[k] = str[0];
        k++;
        temp[k] = ' ';
        k++;
        cond = 1;
        continue;
      }
      if (str[i + 1] == s[j]) {

        if (str[i] == ' ') {
          temp[k] = str[i];
          k++;
        } else {
          temp[k] = str[i];
          k++;
          temp[k] = ' ';
          k++;
        }

        cond = 1;
        continue;
      }
      if (str[i] == s[j] && str[i + 1] != ' ') {
        temp[k] = str[i];
        k++;
        temp[k] = ' ';
        k++;
        cond = 1;
        continue;
      }
    }

    if (cond != 1) {
      temp[k] = str[i];
      k++;
    }
    cond = 0;
    // condition d'arrêt
    if (strlen(temp) >= max) break;
  }
  strcpy(str, temp);
  return 0;
}

/*
  Fonction substenv
  Paramètre str : La chaine rentrée par l'utilisateur.
  Paramètre max : La taille max de la chaine possible.
  Retourne un entier, 0 dans ce cas normal ou -1 si NULL.
*/

int substenv(char * str, size_t max) {
  if (str == NULL) return -1;
  char test[max];
  char variable_env[max];
  const char * path;
  int k = 0;
  int j = 0;
  int x = 0;
  int cond = 0;
  int i;
  int size = strlen(str);
  for (i = 0; i < size; i++) {
  
    if (str[i] == '$') {
      cond = 1;
      j = i + 1;
      while (str[j] != ' ') {
        if (j == size) break;
        variable_env[x] = str[j];
        x++;
        j++;
      }
      path = getenv(variable_env);
      if (path == NULL) {
        test[k] = '$';
        k++;
        for (int z = 0; z < strlen(variable_env); z++) {
          if (strlen(test) > max) break;
          test[k] = variable_env[z];
          k++;
        }
      } else {
        for (int z = 0; z < strlen(path); z++) {
          if (strlen(test) > max) break;
          test[k] = path[z];
          k++;
        }
      }
      memset(variable_env, 0, max * sizeof(char));
      i = j;
      x = 0;
    }
    // cas d'arrêt
    if (strlen(test) > max) break;
    
    test[k] = str[i];
    k++;
  }
  if (cond == 0) {
    return 0;
  } else {
    memset(str, 0, max * sizeof(char));
    for (i = 0; i < k; i++) {
      str[i] = test[i];
    }
    return 0;
  }
}

/*
  Fonction strcut
  Paramètre str : La chaine rentrée par l'utilisateur.
  Paramètre sep : Le caractère qui va séparer la chaine en tokens donc ici ' '.
  Paramètre tokens : Un tableau double dimension qui possède chaque tokens.
  Paramètre max : La taille max de la chaine possible.
  Retourne un entier, 0 dans ce cas normal.
*/

int strcut(char * str, char sep, char ** tokens, size_t max) {
  int k = 1;
  size_t strsize = strlen(str);

  tokens[0] = str;

  for (int i = 0; i < strsize && k < max; i++) {
    if (str[i] == sep && (i + 1) != strsize) {
      str[i] = '\0';
      tokens[k] = str + i + 1;
      k++;
    }
  }
  tokens[k] = NULL;

  return 0;
}

// Optionnel
int strcut_s(char * str, char sep, char ** tokens, size_t max) {
  return 0;
}
