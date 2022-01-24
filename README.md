# ProjetS3 - scurtin
# Installation doxygen

`$ sudo apt-get install doxygen`

Si vous obtenez un "dot" erreur lors de la génération de la documentation essayez:

`$ sudo apt-get install graphviz`

## usage

Deux programmes sont présent. Le premier dans le dossier `scrutin` et le second dans le dossier `verify`.
Les usages sont les mêmes pour les deux programmes.

| Commade | Description |
| ------ | ------ |
| make | Permet de compiler le programme dans le fichier `bin`. |
| make clean| Permet de néttoyer les fichiers `obj`et `bin`. |
| make zip | Permet de zip le programme. |
| make doxygen | Permet de générer la documentation doxygen .|

## Scrutin
> usage: scrutin -i|-d filename.csv [-o filename.txt] [-m methode]

> scrutin -i : vote par classement (ballot)
> scrutin -d : matrice de duel
> scrutin -o : log file
> scrutin -m : methode de scrutin

* uni1
* uni2
* cm
* cs;
* all (défaut)
    
> scrutin -h : help

## Verify

> usage: verify -g|-f filename.csv [-n nom] [-s clef]

> verify -g : ficher (ballot sans hash), génère un ficher avec hash

> verify -f : ficher (ballot avec hash)
> verify -n : Nom de l'élécteur
> verify -s : clef secrète

> verify -h : help