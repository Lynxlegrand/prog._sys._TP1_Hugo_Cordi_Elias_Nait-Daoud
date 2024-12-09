#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define PROMPT "\nenseash % "

int main() {
    char input[256];
    char prompt_buffer[64]; // For storing the dynamic prompt
    snprintf(prompt_buffer, sizeof(prompt_buffer), "%s", PROMPT);

    write(STDOUT_FILENO, "$ ./enseash", 11);
    write(STDOUT_FILENO, "\nWelcome to the ENSEA Shell.\nTo quit, type 'exit'.", 50);

    while (1) {
        write(STDOUT_FILENO, prompt_buffer, strlen(prompt_buffer));

        // Read user input
        ssize_t bytes_read = read(STDIN_FILENO, input, sizeof(input) - 1);
        if (bytes_read <= 0) {
            write(STDOUT_FILENO, "\nBye bye...\n", 48);
            break;
        }

        // Clear input
        input[bytes_read - 1] = '\0';

        // 'exit' command
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
            char *command = strtok(input, " "); // Get the command
            char *arg = strtok(NULL, " ");     // Get the argument

            // Execute the command using execlp
            if (arg != NULL) {
                execlp(command, command, arg, NULL); // Command with argument
            } else {
                execlp(command, command, NULL); // Simple command
            }

            perror("Error executing the command");
            exit(EXIT_FAILURE);
        } else {
            // Parent process: wait for the child process to finish
            int status;
            waitpid(pid, &status, 0);

            // If the command is 'fortune', display the date and time
            if (strcmp(input, "fortune") == 0) {
                time_t now = time(NULL);
                if (now == -1) {
                    perror("Error getting the time");
                    continue;
                }

                char time_str[128];
                struct tm *local_time = localtime(&now);
                if (local_time == NULL) {
                    perror("Error converting the time");
                    continue;
                }

                strftime(time_str, sizeof(time_str), "%a %b %d %H:%M:%S %Z %Y", local_time);
                write(STDOUT_FILENO, "$ ./enseash", 11);
                write(STDOUT_FILENO, "\n", 2);
                write(STDOUT_FILENO, time_str, strlen(time_str));
                write(STDOUT_FILENO, "\n", 1);
            }

            // Update prompt based on child process exit status
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
