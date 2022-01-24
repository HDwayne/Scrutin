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
    fprintf(stdout, "usage: verify -g|-f \033[4mfilename\033[0m.csv [-n \033[4mnom\033[0m] [-s \033[4mclef\033[0m]\n");
    fprintf(stdout, "verify -g : ficher (ballot sans hash), génère un ficher avec hash\n\n");
    fprintf(stdout, "verify -f : ficher (ballot avec hash)\n");
    fprintf(stdout, "verify -n : Nom de l'élécteur\n");
    fprintf(stdout, "verify -s : clef secrète\n\n");

    fprintf(stdout, "verify -h : help\n");
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
 * @param filename Nom du fichier.
 * @return true, si le fichier existe et est accéssible en lecture et écriture.
 * @return false, si le fichier n'existe pas ou que le fichier n'est pas accessible en lecture/ecriture.
 */
bool file_exist(char * filename){
    if (!((!access(filename, F_OK )) && (!access(filename, R_OK )) && (!access(filename, W_OK )))){
        return false;
    }
    return true;
}


/**
 * @brief Vérifie si le ficier précisé contient la colonne hash.
 * 
 * @param filename Nom du fichier.
 * @return true, si la colonne hash est contenu dans le fichier.
 * @return false, si la colonne hash n'est pas contenu dans le fichier.
 */
bool have_hash(char* filename){
    FILE *file = fopen(filename, "r"); // Ouverture du fichier.
	check_open_file(file);

    char* ligne = generate_list_char(MAX);
	fgets(ligne, MAX, file);

	char* token = generate_list_char(LONGMOTS);
    strcpy(token, strtok(ligne, ","));
    
    fclose(file);
    free(ligne);
    
    if (strcmp(token, "hash")!=0){
        free(token);
        return false;
    }

    free(token);
    return true;
}


/**
 * @brief Ajoute un préfixe a une chaine de caractère
 * 
 * @param tab_balises Liste de mots. Chaque mot correspond au paramètre d'une balise.
 * @param prefixe Chaine de caractère représentant le prefix.
 * @param option Entier correspondant à l'option à vérifier.
 */
void add_prefix(char **tab_balises, char* prefixe, int option){
    char* name_file_hash = generate_list_char(50);
    strcpy(name_file_hash, prefixe);
    strcat(name_file_hash, tab_balises[option]);
    strcpy(tab_balises[option], name_file_hash);
    free(name_file_hash);
}


/**
 * @brief Vérifie si le paramètre spécifié par l'utilisateur pour l'option spécifiée est valide.
 * 
 * @param tab_balises Liste de mots. Chaque mot correspond au paramètre d'une balise.
 * @param option Entier correspondant à l'option à vérifier.
 */
void check_parametre(char **tab_balises, int option){
    switch (option){
        case OPT_NOM_FICHIER_G: // Cas pour l'option générer un fichier avec hash
            strcpy(tab_balises[OPT_NOM_FICHIER_F], tab_balises[OPT_NOM_FICHIER_G]); // garde en mémoire le nom du fichier sans "hash_"
            add_prefix(tab_balises, "hash_", OPT_NOM_FICHIER_G); // Ajoute le préfixe "hash_" au nom du fichier à générer

            extension(tab_balises[option], ".csv");
            if (file_exist(tab_balises[option])){ // Ne pas corrompre un fichier déjà existant
                fprintf(stderr, ERROR_FILE_EXIST);
                exit(EXIT_FAILURE);
            }
            if (have_hash(tab_balises[OPT_NOM_FICHIER_F])){ // Ne pas générer un fichier de hash à partir d'un fichier qui en contient déjà
                fprintf(stderr, ERROR_HASH_FOUND);
                exit(EXIT_FAILURE);
            }            
            break;
        case OPT_NOM_FICHIER_F: // Cas pour l'option nom du fichier
            extension(tab_balises[option], ".csv");
            if (!file_exist(tab_balises[option])){ // Si le fichier spécifié n'existe pas
                fprintf(stderr, ERROR_FILE_ACCESS);
                exit(EXIT_FAILURE);
            }
            if (!have_hash(tab_balises[option])){ // Si le fichier spécifié ne contient pas de hash
                fprintf(stderr, ERROR_HASH_NOT_FOUND);
                exit(EXIT_FAILURE);
            }
            break;
        
        case OPT_NOM_ELECTEUR: // Cas pour l'option nom de l'élécteur
            break;
        
        case OPT_SECRET_KEY: // Cas pour la clef secrète 
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
 * @brief Vérifie l'ensemble des paramètres. Et demande les paramètres non spécifié.
 * 
 * @param tab_balises Liste de mots. Chaque mot correspond au paramètre d'une balise.
 */
void check_option(char **tab_balises){
    if (strcmp(tab_balises[OPT_NOM_FICHIER_G], "")==0){ // Si option -g alors le reste est inutile.
        if (strcmp(tab_balises[OPT_NOM_FICHIER_F], "")==0){
            // si le nom du fichier n'est pas précisé, alors le demander
            fprintf(stdout, "Merci de renseigner le nom du fichier: ");
            fgets(tab_balises[OPT_NOM_FICHIER_F], LONGMOTS, stdin);
            tab_balises[OPT_NOM_FICHIER_F][strcspn(tab_balises[OPT_NOM_FICHIER_F], "\n")] = 0; // Supprimer le '\n'
            check_parametre(tab_balises, OPT_NOM_FICHIER_F);
        }
        
        if (strcmp(tab_balises[OPT_NOM_ELECTEUR], "")==0){
            // si le nom de l'élécteur n'est pas précisé, alors le demander
            fprintf(stdout, "Merci de renseigner le nom de l'élécteur: ");
            fgets(tab_balises[OPT_NOM_ELECTEUR], LONGMOTS, stdin);
            tab_balises[OPT_NOM_ELECTEUR][strcspn(tab_balises[OPT_NOM_ELECTEUR], "\n")] = 0; // Supprimer le '\n'
            check_parametre(tab_balises, OPT_NOM_ELECTEUR);
        }

        if (strcmp(tab_balises[OPT_SECRET_KEY], "")==0){
            // si la clef secrète n'est pas précisé, alors la demander
            fprintf(stdout, "Merci de renseigner la clef secrète: ");
            fgets(tab_balises[OPT_SECRET_KEY], LONGMOTS, stdin);
            tab_balises[OPT_SECRET_KEY][strcspn(tab_balises[OPT_SECRET_KEY], "\n")] = 0; // Supprimer le '\n'
            check_parametre(tab_balises, OPT_SECRET_KEY);
        }
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
    for (short i = 1; i < argc; i++){ // Pour tout les arguments
        if (strcmp(argv[i],"-h")==0 || strcmp(argv[i],"-help")==0){ // option -h
            help();
        } else if (strcmp(argv[i],"-g")==0){ // option -g
            to_tab_balise(tab_balises, OPT_NOM_FICHIER_G, &i, argc, argv);
        } else if (strcmp(argv[i],"-f")==0){ // option -f
            to_tab_balise(tab_balises, OPT_NOM_FICHIER_F, &i, argc, argv);
        } else if (strcmp(argv[i],"-n")==0){ // option -n
            to_tab_balise(tab_balises, OPT_NOM_ELECTEUR, &i, argc, argv);
        } else if (strcmp(argv[i],"-s")==0){ // option -s
            to_tab_balise(tab_balises, OPT_SECRET_KEY, &i, argc, argv);
        } else { // option non reconnu
            fprintf(stderr, ERROR_BALISE_UNKNOW);
            exit(EXIT_FAILURE);
        }
    }
    check_option(tab_balises); // Vérifie les options.
}