#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define PROMPT "\nenseash % "

int main() {
    char input[256];
    write(STDOUT_FILENO, "$ ./enseash", 11);
    write(STDOUT_FILENO, "\nBienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.", 58);

    while (1) {
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

        // read user input
        ssize_t bytes_read = read(STDIN_FILENO, input, sizeof(input) - 1);
        if (bytes_read <= 0) {
            write(STDOUT_FILENO, "\nBye bye...\n", 12);
            break;
        }

        input[bytes_read - 1] = '\0'; 
		
		
       

        // Fork to create the child process
        pid_t pid = fork();
        if (pid < 0) {
            perror("Erreur lors du fork");
            continue;
        }

        if (pid == 0) {
            // Child process : execute the command
            char *command = strtok(input, " "); // get the command
            char *arg = strtok(NULL, " ");     // get the argument

            if (arg != NULL) {
                execlp(command, command, arg, NULL); // command with argument
            } else {
                execlp(command, command, NULL); // simple command
            }

            perror("Erreur lors de l'exécution de la commande"); 
            exit(EXIT_FAILURE);
        } else {
            // Parent process : wait untill the child process end
            int status;
            waitpid(pid, &status, 0);

            // fortune command (date + hour)
            if (strcmp(input, "fortune") == 0) {
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
                write(STDOUT_FILENO, "$ ./enseash", 11);
                write(STDOUT_FILENO, "\n", 2);
                write(STDOUT_FILENO, time_str, strlen(time_str));
                write(STDOUT_FILENO, "\n", 1);
            }
        }
    }

    return 0;
}
