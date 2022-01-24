/**
 * @file balise.h
 * @author HERZBERG Dwayne
 * @brief ficher header de balice.c
 */

#ifndef BALISE_H
#define BALISE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "fonction.h"
#include "utils.h"

/** @brief longueur max d'une chaîne de caractères (mot). */
#define LONGMOTS 100
/** @brief Nombre de balise. */
#define NB_BALISES 4
/** @brief Position de l'option -g. */
#define OPT_NOM_FICHIER_G 0
/** @brief Position de l'option -f. */
#define OPT_NOM_FICHIER_F 1
/** @brief Position de l'option -n. */
#define OPT_NOM_ELECTEUR 2
/** @brief Position de l'option -s. */
#define OPT_SECRET_KEY 3

/** @brief Message d'erreur, format du fichier. */
#define ERROR_FILE_FORMAT "Le fichier spécifié n'est pas au bon format.\n"
/** @brief Message d'erreur, existence du fichier ou accessibilité. */
#define ERROR_FILE_ACCESS "Le fichier spécifié introuvable ou droit d'accès inssufisant.\n"
/** @brief Message d'erreur, paramètre de balisé manquant. */
#define ERROR_PARAMETER "Balise sans paramètre.\n"
/** @brief Message d'erreur, balise inconnu. */
#define ERROR_BALISE_UNKNOW "Balie non reconnu.\n"
/** @brief Message d'erreur, le fichier ne contient pas de hash. */
#define ERROR_HASH_NOT_FOUND "Le fichier ne contient pas de hash.\n"
/** @brief Message d'erreur, Le fichier contient déjà les hashs. */
#define ERROR_HASH_FOUND "Le fichier contient déjà les hashs.\n"
/** @brief Message d'erreur, le fichier existe déjà. */
#define ERROR_FILE_EXIST "Le fichier que vous cherchez a générer existe déjà.\n"

void help();

void extension(char* chaine, char* extension);

bool file_exist(char* filename);

bool have_hash(char* filename);

void check_parametre(char **tab_balises, int balise);

void to_tab_balise(char **tab_balises, int balise, short *i, int argc, char **argv);

void check_option(char **tab_balises);

void getop(char **tab_balises, int argc, char **argv);

#endif
