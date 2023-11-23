#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc < 2) { // at least one command is required
        fprintf(stderr, "Invalid number of arguments\n");
        exit(EINVAL);  // exit with einval
    }
    int pipes[argc - 2][2];
    pid_t pids[argc - 1]; //keeping track of all pids
    for (int i = 0; i < argc - 2; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Pipe creation failed");
            exit(-1);
        }}
    for(int i = 0; i < argc - 1; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork failed");
            exit(-2);
        } else if (pid == 0) { // Child process
            if (i != 0) { // Not the first command
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i != argc - 2) {
                dup2(pipes[i][1], STDOUT_FILENO);}
            for (int j = 0; j < argc - 2; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            char *args[] = {argv[i + 1], NULL};
            execvp(args[0], args);
            perror("Exec Error");
            exit(-3);
        } else {
            pids[i] = pid;
        }}
    for (int i = 0; i < argc - 2; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    int exit_code = 0;
    for (int i = 0; i <argc-1; i++) {
        int status;
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            exit_code = WEXITSTATUS(status);
        }}
    exit(exit_code); // returns the exit code (0 if successful)
}
