#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define PROMPT "\n \nenseash % "

int main() {
    char input[256];
    write(STDOUT_FILENO, "$ ./enseash", 11);
    write(STDOUT_FILENO, "\nBienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.", 58);

    while (1) {
        // Afficher le prompt
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

        // Lire l'entrée utilisateur
        ssize_t bytes_read = read(STDIN_FILENO, input, sizeof(input) - 1);
        if (bytes_read <= 0) {
            write(STDOUT_FILENO, "\nBye bye...\n", 12);
            break;
        }

        // Nettoyer l'entrée
        input[bytes_read - 1] = '\0'; // Remplace '\n' par '\0'

        // Gestion de la commande 'exit'
        if (strcmp(input, "exit") == 0) {
            write(STDOUT_FILENO, "Bye bye...\n", 11);
            break;
        }

        // Gestion de la commande 'fortune'
        if (strcmp(input, "fortune") == 0) {
            write(STDOUT_FILENO, "Today is what happened to yesterday.", 40);
            write(STDOUT_FILENO, PROMPT, strlen(PROMPT));


            // Obtenir l'heure actuelle
            time_t now = time(NULL);
            if (now == -1) {
                perror("Erreur lors de la récupération de l'heure");
                continue;
            }

            // Convertir l'heure en chaîne lisible
            char time_str[128];
            struct tm *local_time = localtime(&now);
            if (local_time == NULL) {
                perror("Erreur lors de la conversion de l'heure");
                continue;
            }

            strftime(time_str, sizeof(time_str), "%a %b %d %H:%M:%S %Z %Y", local_time);
            write(STDOUT_FILENO, "\n", 1);
            write(STDOUT_FILENO, time_str, strlen(time_str));
            

            continue;
        }

        // Commande non reconnue
        write(STDOUT_FILENO, "Commande inconnue.\n", 20);
    }

    return 0;
}
