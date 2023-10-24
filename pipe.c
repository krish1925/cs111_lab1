#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Invalid number of arguments\n");
        exit(EXIT_FAILURE);
    }

    int pipes[argc - 1][2];

    for (int i = 0; i < argc - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < argc - 1; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child process
            if (i != 0) { // Not the first command
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][0]);
                close(pipes[i - 1][1]);
            }
            
            if (i != argc - 2) { // Not the last command
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            
            for (int j = 0; j < argc - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            char *args[] = {argv[i + 1], NULL};
            execvp(args[0], args);
            perror("Exec Error");
            exit(EXIT_FAILURE);
        } 
    }

    for (int i = 0; i < argc - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all child processes to finish.
    for (int i = 0; i < argc - 1; i++) {
        wait(NULL);
    }

    return 0;
}
