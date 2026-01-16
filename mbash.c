/* * FICHIER : mbash.c
 * DESCRIPTION : Version miniature de bash (SAE 3.03)
 * COMPILATION : gcc -o mbash mbash.c
 * USAGE : ./mbash
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLI 2048 // Taille maximale d'une ligne de commande

/**
 * Nettoie la chaîne saisie en supprimant le retour à la ligne '\n'
 * ajouté par fgets
 */
void trim_newline(char *s) {
    int len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
}

int main(int argc, char** argv) {
    char cmd[MAXLI];
    char cwd[MAXLI];

    printf("Bienvenue dans mbash ! 0.2\n");
    printf("Tapez 'exit' ou Ctrl-D pour quitter.\n");

    // Boucle de lecture interactive
    while (1) {
        //Affichage du prompt avec le répertoire courant (équivalent visuel de pwd)
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("mbash:%s$ ", cwd);
        } else {
            printf("mbash$ ");
        }

        // Lecture de la commande
        // fgets renvoie NULL si l'utilisateur fait Ctrl-D 
        if (fgets(cmd, MAXLI, stdin) == NULL) {
            printf("\ndeconnexion\n");
            break;
        }

        //Nettoyage de la saisie
        trim_newline(cmd);

        // Ignorer les entrées vides
        if (strlen(cmd) == 0) {
            continue;
        }

        //Gestion des commandes "Built-in" (Commandes internes)
        
        // Commande 'exit' pour quitter le shell 
        if (strcmp(cmd, "exit") == 0) {
            break;
        }

        // Commande 'cd' : Doit être gérée en interne par chdir() 
        // system("cd ...") ne marcherait pas car il s'exécute dans un processus séparé.
        if (strncmp(cmd, "cd ", 3) == 0) {
            char *path = cmd + 3; // Récupère le texte après "cd "
            if (chdir(path) != 0) {
                perror("mbash: cd"); // Affiche l'erreur si le dossier est invalide
            }
            continue; // Retour au début de la boucle
        }

        // Exécution des autres commandes (ls, pwd, etc.)
        /* * L'appel à system() permet de :
         * - Chercher la commande dans le PATH automatiquement 
         * - Gérer le caractère '&' pour l'arrière-plan nativement 
         * - Utiliser la syntaxe exacte de bash
         */
        system(cmd);
    }

    return 0;
}
