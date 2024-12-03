#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


#define PROMPT "\n \nenseash % "

int main() {
    char input[256];
    write(STDOUT_FILENO, "$ ./enseash", 11);
    write(STDOUT_FILENO, "\nBienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.", 58);

    while (1) {
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

        // Read input
        ssize_t bytes_read = read(STDIN_FILENO, input, sizeof(input) - 1);
        if (bytes_read <= 0) {
            write(STDOUT_FILENO, "\nBye bye...\n", 12);
            break;
        }

        // Clear input
        input[bytes_read - 1] = '\0'; 

        // 'fortune' prompt
        if (strcmp(input, "fortune") == 0) {
            write(STDOUT_FILENO, "Today is what happened to yesterday.", 40);
            write(STDOUT_FILENO, PROMPT, strlen(PROMPT));


            // get hour
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
            write(STDOUT_FILENO, "\n", 1);
            write(STDOUT_FILENO, time_str, strlen(time_str));
            

            continue;
        }

        // Create a child process to execute the command
        pid_t pid = fork();
        if (pid < 0) {
            perror("Erreur lors du fork");
            continue;
        }

        if (pid == 0) {
            // Child process: Split the input into command and arguments
            char *args[256];
            char *token = strtok(input, " ");
            int i = 0;

            while (token != NULL) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;

            // Execute the command using execlp
            if (execlp(args[0], args[0], args[1], args[2], args[3], args[4], NULL) == -1) {
                perror("Erreur lors de l'exécution de la commande");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process: Wait for the child to finish
            int status;
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}
