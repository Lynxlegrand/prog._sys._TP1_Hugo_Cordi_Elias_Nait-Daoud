#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define PROMPT "\n \nenseash % "

int main() {
    char input[256];
    char prompt_buffer[64];
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
            // Child process
            char *args[256];
            char *token = strtok(input, " ");
            int i = 0;

            while (token != NULL) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;

            if (execvp(args[0], args) == -1) {
                perror("Erreur lors de l'exécution de la commande");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            int status;

            // Get the start time
            struct timespec start_time, end_time;
            clock_gettime(CLOCK_MONOTONIC, &start_time);

            waitpid(pid, &status, 0);

            // Get the end time after the child process finishes
            clock_gettime(CLOCK_MONOTONIC, &end_time);

            // Calculate the elapsed time in milliseconds
            long long elapsed_time_ms = (end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_nsec - start_time.tv_nsec) / 1000000;

            // Child process status
            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                snprintf(prompt_buffer, sizeof(prompt_buffer), "\nenseash [exit:%d|%lldms] %% ", exit_code, elapsed_time_ms);
            } else if (WIFSIGNALED(status)) {
                int signal_number = WTERMSIG(status);
                snprintf(prompt_buffer, sizeof(prompt_buffer), "\nenseash [sign:%d|%lldms] %% ", signal_number, elapsed_time_ms);
            } else {
                snprintf(prompt_buffer, sizeof(prompt_buffer), "%s", PROMPT);
            }
        }
    }

    return 0;
}
