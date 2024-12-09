#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define PROMPT "\nenseash % "

int main() {
    char input[256];
    char prompt_buffer[64]; // Buffer for the dynamic prompt
    snprintf(prompt_buffer, sizeof(prompt_buffer), "%s", PROMPT);

    write(STDOUT_FILENO, "$ ./enseash", 11);
    write(STDOUT_FILENO, "\nWelcome to the ENSEA Shell.\nTo quit, type 'exit'.", 50);

    while (1) {
        // Display the prompt
        write(STDOUT_FILENO, prompt_buffer, strlen(prompt_buffer));

        // Read user input
        ssize_t bytes_read = read(STDIN_FILENO, input, sizeof(input) - 1);
        if (bytes_read <= 0) {
            write(STDOUT_FILENO, "\nBye bye...\n", 12);
            break;
        }

        // Clean up user input (replace '\n' with '\0')
        input[bytes_read - 1] = '\0';

        // "exit" command to quit
        if (strcmp(input, "exit") == 0) {
            write(STDOUT_FILENO, "Bye bye...\n", 11);
            break;
        }

        // Fork to create the child process
        pid_t pid = fork();
        if (pid < 0) {
            perror("Error during fork");
            continue;
        }

        if (pid == 0) {
            // Child process: execute the command
            char *args[64]; // Array to store the command and its arguments
            int i = 0;

            // Split input into arguments
            char *token = strtok(input, " ");
            while (token != NULL && i < 63) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL; // Null-terminate the array

            execvp(args[0], args); // Execute the command
            perror("Error executing the command"); // If execvp fails
            exit(EXIT_FAILURE);
        } else {
            // Parent process: measure execution time and wait for the child process
            int status;
            struct timespec start_time, end_time;

            // Record start time
            clock_gettime(CLOCK_MONOTONIC, &start_time);

            // Wait for the child process to finish
            waitpid(pid, &status, 0);

            // Record end time
            clock_gettime(CLOCK_MONOTONIC, &end_time);

            // Calculate elapsed time in milliseconds
            long long elapsed_time_ms =
                (end_time.tv_sec - start_time.tv_sec) * 1000 +
                (end_time.tv_nsec - start_time.tv_nsec) / 1000000;

            // Special handling for "fortune" command
            if (strcmp(input, "fortune") == 0) {
                time_t now = time(NULL);
                if (now != -1) {
                    char time_str[128];
                    struct tm *local_time = localtime(&now);
                    if (local_time != NULL) {
                        strftime(time_str, sizeof(time_str), "%a %b %d %H:%M:%S %Z %Y", local_time);
                        write(STDOUT_FILENO, "$ ./enseash\n", 12);
                        write(STDOUT_FILENO, time_str, strlen(time_str));
                        write(STDOUT_FILENO, "\n", 1);
                    }
                }
            }

            // Update the prompt dynamically based on exit status or signal
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
