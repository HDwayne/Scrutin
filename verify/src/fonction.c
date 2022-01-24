/**
 * @file fonction.c
 * @author HERZBERG Dwayne
 * @brief Ensemble des fonctions permettant la vérification de vote.
 */

#include "fonction.h"


/**
 * @brief Cherche l'équivalent binaire de chaine nombre hexadécimal.
 * 
 * @param elem caractère hexadécimal.
 * @return char*, Chaine de caractère correspondant aux quatre bits binaires.
 */
char* equivalent(char elem){
	switch(elem){
		case '0':
			return "0000";
			break;
		case '1':
			return "0001";
			break;
		case '2':
			return "0010";
			break;
		case '3':
			return "0011";
			break;
		case '4':
			return "0100";
			break;
		case '5':
			return "0101";
			break;
		case '6':
			return "0110";
			break;
		case '7':
			return "0111";
			break;
		case '8':
			return "1000";
			break;
		case '9':
			return "1001";
			break;
		case 'A':
		case 'a':
			return "1010";
			break;
		case 'B':
		case 'b':
			return "1011";
			break;
		case 'C':
		case 'c':
			return "1100";
			break;
		case 'D':
		case 'd':
			return "1101";
			break;
		case 'E':
		case 'e':
			return "1110";
			break;
		case 'F':
		case 'f':
			return "1111";
			break;
		default:
			exit(EXIT_FAILURE);
			break;
	}
}


/**
 * @brief Convertie une chaine hex en bin.
 * 
 * @param resultat Liste de nombre binaire (sortie).
 * @param hex Liste de nombre binaire.
 * @return unsigned* 
 */
unsigned char* hex_to_binary(unsigned char* resultat, char* hex){
	int j=0;
	char* chaine=generate_list_char(5);
	for(int i=0; i < strlen(hex);i++){		
		strcpy(chaine, equivalent(hex[i])); // Cherche l'equivalent bin d'un nombre hex.
		int k=0;
		for (int i = j; i < j+4; i++){
			resultat[i]=chaine[k]-48; // Copie les quatre bits équivalant au nombre hexadécimal.
			k++;
		}
		j+=4;
	}
	return resultat;

	// unsigned char* bin = malloc(((strlen(hashRes)*4)+1)*sizeof(unsigned char));
	// hex_to_binary(bin, hashRes);
}


/**
 * @brief Génère un hash.
 * 
 * @param hashRes Chaine de caractère qui contiendra le hash.
 * @param name Chaine de caractère qui correspond au nom de l'élécteur.
 * @param key Chaine de caratère qui représente la clef privé de l'élécteur.
 * @return char*, hash.
 */
char* hash(char *hashRes, char* name, char* key){
	char * item = generate_list_char(100); // contiendra la chaîne à hasher

	strcpy(item, name); // Place le nom au debut de l'item
	strcat(item, key); // Concatène la clef au nom dans l'item.
	sha256ofString((BYTE *)item, hashRes); // hashRes contient maintenant le hash de l'item
	
	free(item);
	return hashRes;
}


/**
 * @brief Génère une clef aléatoire.
 * 
 * @param size Entier correspondant à la taille de la clef (taille - 1).
 * @param key Chaine de caractère qui contiendra la clef.
 * @return char* 
 */
char* generate_key(int size, char* key){
	//  Ensemble des caractères possibles par catégorie.
    char nombre[] = "0123456789";
    char lettre[] = "abcdefghijklmnoqprstuvwyzx";
    char LETTRE[] = "ABCDEFGHIJKLMNOQPRSTUYWVZX";
    char symbole[] = "<@[#{&*?=)+]^}(:>";
    int choix;

    for (int i = 0; i < size-1; i++) {
    	choix = rand() % 4; // Séléctionner la catégorie.
        if (choix == 1) {
            key[i] = lettre[rand() % strlen(lettre)]; // Lettre aléatoire
        } else if (choix == 2) {
            key[i] = symbole[rand() % strlen(symbole)]; // Symbole aléatoire
        } else if (choix == 3) {
            key[i] = nombre[rand() % strlen(nombre)]; // Nombre aléatoire
        } else {
            key[i] = LETTRE[rand() % strlen(LETTRE)]; // Lettre Maj aléatoire
        }
    }
	key[size-1]='\0';
	return key;
}


/**
 * @brief Génere un fichier csv contenant les hash et clef privé des élécteurs.
 * 
 * @param tab_balises Liste de mots. Chaque mot correspond au paramètre d'une balise.
 */
void addhash(char **tab_balises){
    srand(time(NULL)); // Initialise la fonction srand sur le temps actuel.

	// Ouverture des fichiers.

	FILE *file = fopen(tab_balises[OPT_NOM_FICHIER_F], "r"); // Ouvrire le fichier csv.
	check_open_file(file);

	FILE *file_hash = fopen(tab_balises[OPT_NOM_FICHIER_G], "a+"); // générer le fichier csv avec hash.
	check_open_file(file_hash);

	// Initialisation des variables.

	char* ligne = generate_list_char(MAX);
	char* name = generate_list_char(LONGMOTS);
	char* hashRes = generate_list_char(SHA256_BLOCK_SIZE*2 + 1);
	char* key = generate_list_char(25);
	char* lignetemp = generate_list_char(MAX);

	// Lecture et écriture des fichiers.

	fgets(ligne, MAX, file); // lis l'en tête du fichier sans hash.
	fprintf(file_hash, "hash,clef prive,%s", ligne); // écrit dans le fichier générer l'ensembles des colonnes en y ajoutant "hash,clef prive"
	while (fgets(ligne, MAX, file)){ // Tant qu'il y a des votes.
		strcpy(lignetemp, ligne); // sauvegarder la ligne.
		ligne[strcspn(ligne, "\n")] = 0; // supprimer le '\n' a la fin de la ligne.

		generate_key(25, key); // générer une clef aléatoire de taille 25 (24 + '\0').

		strcpy(name, strtok(ligne, ",")); // Extraire le nom de l'élécteur.
		for (int i = 0; i < 3; i++){
			strcpy(name, strtok(NULL, ","));
		}
		
		hash(hashRes, name, key); // Générer le hash de l'élécteur.

		fprintf(file_hash, "%s,%s,%s", hashRes, key, lignetemp); // Ecrire le résultat a la fin du fichier générer.
	}

	// Libération de la mémoire et fermeture des fichiers.

	free(name);
	free(hashRes);
	free(key);
	free(lignetemp);
	free(ligne);

	fclose(file);
	fclose(file_hash);
}


/**
 * @brief Affiche prorement la ligne de vote trouvé.
 * 
 * @param file 
 * @param ligne Chaine de caractère correspondant à la ligne du vote.
 */
void afficher(FILE *file, char* ligne){
	// Inititaliser les variables.

	char* tete = generate_list_char(MAX);
	char** mtete = generate_matrice_char(1, LONGMOTS);
    char* token = generate_list_char(LONGMOTS); // en tête
    char* token2 = generate_list_char(LONGMOTS); // vote
	int col=0;

	rewind(file); // Se placer au début du fichier.
	fgets(tete, MAX, file); // récupérer l'en tête.

	tete[strcspn(tete, "\n")] = 0; // supprimer le '\n' a la fin de la ligne.
	ligne[strcspn(ligne, "\n")] = 0;

    token = strtok(tete, ",");
    while (token!=NULL){ // Lecture de chaque colonne tant qu'il y a existance.
		strcpy(mtete[col], token); // Sauvegarder le nom de la colonne dans mtete.
        token = strtok(NULL, ",");
		col+=1;
		mtete = (char **) realloc(mtete, (col+1)*sizeof(char*));
		mtete[col] = generate_list_char(LONGMOTS);
    }

	// Afficher le vote trouvé.

	fprintf(stdout, " ┏╸\n");

    token2 = strtok(ligne, ",");
	for (int i = 0; i < col; i++){
		fprintf(stdout, " ┃ %s : %s\n", mtete[i], token2);
        token2 = strtok(NULL, ",");
	}

	fprintf(stdout, " ┗╸\n");
	
	// Lébérer la mémoire.

	free(token);
	free(token2);
	free(tete);
	for (int i = 0; i < col; i++){
		free(mtete[i]);
	}
	free(mtete);
}


/**
 * @brief Fonction qui cherche dans le fichier contenant les hash, s'il y a un hash qui correspond.
 * 
 * @param tab_balises Liste de mots. Chaque mot correspond au paramètre d'une balise.
 * @param hashRes Chaine de caractère correspondant au hash.
 */
void find_in_file(char **tab_balises, char *hashRes){
	FILE *file = fopen(tab_balises[OPT_NOM_FICHIER_F], "r");
	check_open_file(file);

	char *ligne = generate_list_char(MAX);
	char *lignetemp = generate_list_char(MAX);
	char *token = generate_list_char(LONGMOTS);
	bool find=false;

	fgets(ligne, 5000, file);
	while (!find && fgets(ligne, 5000, file)){ // Tant qu'il y a des lignes && que le vote n'est pas trouvé.
		strcpy(lignetemp, ligne);

		strcpy(token, strtok(ligne, ","));
		if (strcmp(token,hashRes)==0){ // si le hash du vote correspond a celui souhaité.
			find=true;
		}
	}

	if (find) { // Si vote correspond alors afficher.
		afficher(file, lignetemp);
	} else {
		fprintf(stdout, " ┏╸\n ┃ Aucun vote ne correspond aux informations spécifiés.\n ┗╸\n");
	}

	free(ligne);
	free(lignetemp);
	free(token);
	fclose(file);
}

