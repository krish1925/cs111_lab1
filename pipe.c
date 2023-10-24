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

            dup2(pipes[i][1], STDOUT_FILENO);
            close(pipes[i][0]);
            close(pipes[i][1]);

            char *args[] = {argv[i + 1], NULL};
            execvp(args[0], args);
            perror("Exec Error");
            exit(EXIT_FAILURE);
        } else { // Parent process
            close(pipes[i][1]);
            if (i != 0) {
                close(pipes[i - 1][0]);
            }
        }
    }

    pid_t last_thread_pid = fork();
    if (last_thread_pid == -1) {
        perror("Fork Error");
        exit(EXIT_FAILURE);
    } else if (last_thread_pid == 0) {
        if (argc > 2) {
            dup2(pipes[argc - 2][0], STDIN_FILENO);
            close(pipes[argc - 2][0]);
            close(pipes[argc - 2][1]);
        }
        char *args[] = {argv[argc - 1], NULL};
        execvp(args[0], args);
        perror("Exec error");
        exit(EXIT_FAILURE);
    } else {
        close(pipes[argc - 2][0]);
    }

    // Wait for all child processes to finish.
    for (int i = 0; i < argc - 1; i++) {
        wait(NULL);
    }

    return 0;
}
