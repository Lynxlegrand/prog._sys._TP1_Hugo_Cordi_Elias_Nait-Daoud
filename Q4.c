#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define PROMPT "\n \nenseash % "

int main() {
    char input[256];
    char prompt_buffer[64]; // Pour stocker le prompt dynamique
    snprintf(prompt_buffer, sizeof(prompt_buffer), "%s", PROMPT);

    write(STDOUT_FILENO, "$ ./enseash", 11);
    write(STDOUT_FILENO, "\nBienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.", 58);

    while (1) {
        write(STDOUT_FILENO, prompt_buffer, strlen(prompt_buffer));

        // Read input
        ssize_t bytes_read = read(STDIN_FILENO, input, sizeof(input) - 1);
        if (bytes_read <= 0) {
            write(STDOUT_FILENO, "\nBye bye...\n", 12);
            break;
        }

        // Clear input
        input[bytes_read - 1] = '\0';

        // 'exit' prompt
        if (strcmp(input, "exit") == 0) {
            write(STDOUT_FILENO, "Bye bye...\n", 11);
            break;
        }

        // 'fortune' prompt
        if (strcmp(input, "fortune") == 0) {
            write(STDOUT_FILENO, "Today is what happened to yesterday.\n", 37);

            // Get the current date and time
            time_t now = time(NULL);
            if (now == -1) {
                perror("Erreur lors de la récupération de l'heure");
                continue;
            }

            char time_str[128];
            struct tm *local_time = localtime(&now);
            if (local_time == NULL) {
                perror("Erreur lors de la conversion de l'heure");
                continue;
            }

            strftime(time_str, sizeof(time_str), "%a %b %d %H:%M:%S %Z %Y", local_time);
            write(STDOUT_FILENO, time_str, strlen(time_str));
            write(STDOUT_FILENO, "\n", 1);

            // Remettre le prompt à l'état initial
            snprintf(prompt_buffer, sizeof(prompt_buffer), "%s", PROMPT);
            continue;
        }

        // Create a child process to execute the command
        pid_t pid = fork();
        if (pid < 0) {
            perror("Erreur lors du fork");
            continue;
        }

        if (pid == 0) {
            // Processus enfant : découpage de l'entrée en commande et arguments
            char *args[256];
            char *token = strtok(input, " ");
            int i = 0;

            while (token != NULL) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;

            // Exécution de la commande
            if (execvp(args[0], args) == -1) {
                perror("Erreur lors de l'exécution de la commande");
                exit(EXIT_FAILURE);
            }
        } else {
            // Processus parent : Attente de la fin du processus enfant
            int status;
            waitpid(pid, &status, 0);

            // Gestion du statut du processus enfant
            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                snprintf(prompt_buffer, sizeof(prompt_buffer), "\nenseash [exit:%d] %% ", exit_code);
            } else if (WIFSIGNALED(status)) {
                int signal_number = WTERMSIG(status);
                snprintf(prompt_buffer, sizeof(prompt_buffer), "\nenseash [sign:%d] %% ", signal_number);
            } else {
                snprintf(prompt_buffer, sizeof(prompt_buffer), "%s", PROMPT);
            }
        }
    }

    return 0;
}
