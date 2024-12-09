#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PROMPT "\nenseash % "

int main() {
    char input[256];
    write(STDOUT_FILENO, "$ ./enseash", 11);
    write(STDOUT_FILENO, "\nBienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n", 58);

    while (1) {
        // write the prompt
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

        // Reat user input
        ssize_t bytes_read = read(STDIN_FILENO, input, sizeof(input) - 1);
        if (bytes_read <= 0) {
            write(STDOUT_FILENO, "\nBye bye...\n", 12);
            break;
        }

        // Clear the input
        input[bytes_read - 1] = '\0'; // Remplace '\n' par '\0'

        // 'exit' prompt
        if (strcmp(input, "exit") == 0) {
            write(STDOUT_FILENO, "Bye bye...\n", 11);
            break;
        }
    }

    return 0;
}
