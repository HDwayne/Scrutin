/**
 * @file balise.h
 * @author HERZBERG Dwayne
 * @brief ficher header de balise.c
 */

#ifndef BALISE_H
#define BALISE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/** @brief Nombre de balise. */
#define NB_BALISES 4
/** @brief Position de l'option -i. */
#define OPT_I 0
/** @brief Position de l'option -d. */
#define OPT_D 1
/** @brief Position de l'option -o. */ 
#define OPT_O 2
/** @brief Position de l'option -m. */
#define OPT_M 3

/** @brief Message d'erreur, format du fichier. */
#define ERROR_FILE_FORMAT "Le fichier spécifié n'est pas au bon format.\n"
/** @brief Message d'erreur, existence du fichier ou accessibilité. */
#define ERROR_FILE_ACCESS "Le fichier spécifié introuvable ou droit d'accès insuffisant.\n"
/** @brief Message d'erreur, méthode invalide. */
#define ERROR_METHOD "La méthode spécifié est invalide.\n"
/** @brief Message d'erreur, paramètre de balisé manquant. */
#define ERROR_PARAMETER "Balise sans paramètre.\n"
/** @brief Message d'erreur, balise inconnu. */
#define ERROR_BALISE_UNKNOW "Balise non reconnu.\n"
/** @brief Message d'erreur, conflit entre balise. */
#define ERROR_BALISE_CONFLICT "Balise non compatible ensemble.\n"
/** @brief Message d'erreur, balise non spécifié. */
#define ERROR_BALISE_REQUIRE "Balise obligatoire absente.\n"
/** @brief Message d'erreur, méthode indisponible. */
#define ERROR_METHODE_INDISPONIBLE "Methode indisponible pour l'option -d.\n"

void help();

void extension(char * chaine, char * extension);

void file_exist(char * filename);

void check_methode(char * chaine);

void check_parametre(char **tab_balises, int balise);

void to_tab_balise(char **tab_balises, int balise, short *i, int argc, char **argv);

void check_option(char **tab_balises);

void getop(char **tab_balises, int argc, char **argv);

#endif
