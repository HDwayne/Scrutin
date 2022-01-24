/**
 * @file balise.c
 * @author HERZBERG Dwayne
 * @brief Gestion des paramètres et options lors de l'exécution du programme.
 */

#include "balise.h"


/**
 * @brief Afficher l'ensemble des usages du programme.
 */
void help(){
    fprintf(stdout, "usage: scrutin -i|-d \033[4mfilename\033[0m.csv [-o \033[4mfilename\033[0m.txt] [-m \033[4mmethode\033[0m]\n");
    fprintf(stdout, "scrutin -i : vote par classement (ballot)\n");
    fprintf(stdout, "scrutin -d : matrice de duel\n");
    fprintf(stdout, "scrutin -o : log file\n");
    fprintf(stdout, "scrutin -m : methode de scrutin\n\n");

    fprintf(stdout, "    > uni1\n");
    fprintf(stdout, "    > uni2\n");
    fprintf(stdout, "    > cm\n");
    fprintf(stdout, "    > cs\n");
    fprintf(stdout, "    > all (défaut)\n\n");

    fprintf(stdout, "scrutin -h : help\n");
    exit(EXIT_SUCCESS);
}


/**
 * @brief Vérifie si le nom du fichier entré possède la bonne extension.
 * 
 * @param chaine Chaine de caractère contenant le nom du fichier et son extension (test.txt).
 * @param extension Chaine de caractère correspondant à l'extension souhaitée.
 */
void extension(char * chaine, char * extension){
    char* ext = strrchr(chaine, '.');
    if (ext == NULL || strcmp(ext,extension)!=0){
        fprintf(stderr, ERROR_FILE_FORMAT);
        exit(EXIT_FAILURE);
    }
}


/**
 * @brief Vérifie si le fichier spécifié existe et est accessible en lecture et écriture.
 * 
 * @param filename Chaine de caractère correspondant au nom du fichier.
 */
void file_exist(char * filename){
    if (!((!access(filename, F_OK )) && (!access(filename, R_OK )) && (!access(filename, W_OK )))){
        fprintf(stderr, ERROR_FILE_ACCESS);
        exit(EXIT_FAILURE);
    }
}


/**
 * @brief Vérifie si la méthode spécifiée en paramètre est valide.
 * 
 * @param chaine Chaine de caractère correspondant à la méthode à vérifier.
 */
void check_methode(char * chaine){
    if (!(strcmp(chaine, "uni1") == 0 || \
        strcmp(chaine, "uni2") == 0 || \
        strcmp(chaine, "cm") == 0 || \
        strcmp(chaine, "cs") == 0 || \
        strcmp(chaine, "all") == 0 || \
        strcmp(chaine, "") == 0))
    {
        fprintf(stderr, ERROR_METHOD);
        exit(EXIT_FAILURE);
    }
}


/**
 * @brief Vérifie si le paramètre spécifié par l'utilisateur pour l'option spécifiée est valide.
 * 
 * @param tab_balises Liste de mots. Chaque mot correspond au paramètre d'une balise.
 * @param option Entier correspondant à l'option à vérifier.
 */
void check_parametre(char **tab_balises, int option){
    switch (option){
        case OPT_I: // Cas pour l'option ballot
        case OPT_D: // Cas pour l'option matrice de duel
            extension(tab_balises[option], ".csv");
            file_exist(tab_balises[option]);
            break;
        
        case OPT_O: // Cas pour l'option sortie log
            extension(tab_balises[option], ".txt");
            break;
        
        case OPT_M: // Cas pour l'option choix de méthode. 
            check_methode(tab_balises[option]);
            break;
        
        default:
            break;
    }
}


/**
 * @brief Entre le paramètre correspondant à l'option spécifiée par l'utilisateur dans "tab_balises" si celle-ci est valide.
 * 
 * @param tab_balises Liste de mots. Chaque mot correspond au paramètre d'une balise.
 * @param balise Entier correspondant à l'emplacement de l'option dans la liste.
 * @param ind Pointeur, Entier correspondant à l'indice/position dans la liste d'arguments à argv.
 * @param argc 
 * @param argv 
 */
void to_tab_balise(char **tab_balises, int balise, short *ind, int argc, char **argv){
    if (*ind+1 <= argc-1){
        strcpy(tab_balises[balise], argv[*ind+1]);
        *ind = *ind + 1;
        check_parametre(tab_balises, balise);
    } else { 
        fprintf(stderr, ERROR_PARAMETER);
        exit(EXIT_FAILURE); 
    }
}


/**
 * @brief Vérifie la compatibilité des paramètres.
 * 
 * @param tab_balises Liste de mots. Chaque mot correspond au paramètre d'une balise.
 */
void check_option(char **tab_balises){
    if (strcmp(tab_balises[OPT_D], "")==0 && strcmp(tab_balises[OPT_I], "")==0){
        // Vérifie qu'il y a au moins une des deux options i/d
        fprintf(stderr, ERROR_BALISE_REQUIRE);
        exit(EXIT_FAILURE);
    }
    
    if (!((strcmp(tab_balises[OPT_D], "")==0 && strcmp(tab_balises[OPT_I], "")!=0) \
        || (strcmp(tab_balises[OPT_I], "")==0 && strcmp(tab_balises[OPT_D], "")!=0))){
        // Vérifie que seules des deux balises d et i est spécifié.
        fprintf(stderr, ERROR_BALISE_CONFLICT);
        exit(EXIT_FAILURE);
    }

    if (strcmp(tab_balises[OPT_D], "")!=0 && (strcmp(tab_balises[OPT_M], "uni1")==0 || strcmp(tab_balises[OPT_M], "uni2")==0)){
        // Si matrice duel entrée alors uni1 est uni2 non compatible.
        fprintf(stderr, ERROR_METHODE_INDISPONIBLE);
        exit(EXIT_FAILURE);
    }
    
    if (strcmp(tab_balises[OPT_M], "")==0){
        // Si aucune méthode spécifiée alors all est mis pas défaut.
        strcpy(tab_balises[OPT_M], "all");
    }
}


/**
 * @brief Fonction principale d'analyse des options renseignées.
 * 
 * @param tab_balises Liste de mots. Chaque mot correspond au paramètre d'une balise.
 * @param argc 
 * @param argv 
 */
void getop(char **tab_balises, int argc, char **argv){
    if (argc == 1){ // si il n'y a pas d'arguments
        help();
    } else {
        for (short i = 1; i < argc; i++){ // Pour tout les arguments
            if (strcmp(argv[i],"-h")==0 || strcmp(argv[i],"-help")==0){ // option -h
                help();
            } else if (strcmp(argv[i],"-i")==0){ // option -i
                to_tab_balise(tab_balises, OPT_I, &i, argc, argv);
            } else if (strcmp(argv[i],"-d")==0){ // option -d
                to_tab_balise(tab_balises, OPT_D, &i, argc, argv);
            } else if (strcmp(argv[i],"-o")==0){ // option -o
                to_tab_balise(tab_balises, OPT_O, &i, argc, argv);
            } else if (strcmp(argv[i],"-m")==0){ // option -m
                to_tab_balise(tab_balises, OPT_M, &i, argc, argv);
            } else { // option non reconnu
                fprintf(stderr, ERROR_BALISE_UNKNOW);
                exit(EXIT_FAILURE);
            }
        }
        check_option(tab_balises); // Vérifie les options renseignées.
    }
}