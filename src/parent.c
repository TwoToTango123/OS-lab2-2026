#include "utils.h"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef CHILD_PATH
#define CHILD_PATH "./child"
#endif

static pid_t spawn_child(int in_fd, int out_fd, const int *close_fds, size_t n_close) {
    pid_t pid = fork();
    if (pid == -1) {
        die("fork");
    }
    if (pid > 0) {
        return pid;
    }

    if (dup2(in_fd, STDIN_FILENO) == -1) {
        perror("dup2 stdin");
        _exit(EXIT_FAILURE);
    }
    if (dup2(out_fd, STDOUT_FILENO) == -1) {
        perror("dup2 stdout");
        _exit(EXIT_FAILURE);
    }

    close(in_fd);
    close(out_fd);
    for (size_t i = 0; i < n_close; i++) {
        close(close_fds[i]);
    }

    execl(CHILD_PATH, "child", (char *)NULL);

    perror("execl");
    _exit(EXIT_FAILURE);
}

static char *read_filename(const char *prompt) {
    printf("%s", prompt);
    fflush(stdout);

    char *name = NULL;
    size_t cap = 0;
    ssize_t len = getline(&name, &cap, stdin);
    if (len == -1) {
        free(name);
        if (ferror(stdin)) {
            die("getline");
        }
        fprintf(stderr, "parent: expected a file name, got end of input\n");
        exit(EXIT_FAILURE);
    }
    if (name[len - 1] == '\n') {
        name[len - 1] = '\0';
    }
    return name;
}

static int open_output(const char *name) {
    int fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        die(name);
    }
    return fd;
}

static int wait_child(pid_t pid, const char *name) {
    int status;
    while (waitpid(pid, &status, 0) == -1) {
        if (errno != EINTR) {
            perror("waitpid");
            return -1;
        }
    }
    if (WIFEXITED(status)) {
        printf("parent: %s (PID=%d) exited with code %d\n", name, pid, WEXITSTATUS(status));
        return WEXITSTATUS(status) == 0 ? 0 : -1;
    }
    if (WIFSIGNALED(status)) {
        printf("parent: %s (PID=%d) killed by signal %d\n", name, pid, WTERMSIG(status));
    }
    return -1;
}

int main(void) {
    printf("parent: PID=%d\n", getpid());

    char *name1 = read_filename("File for child1 (lines <= 10 chars): ");
    char *name2 = read_filename("File for child2 (lines > 10 chars): ");

    int file1 = open_output(name1);
    int file2 = open_output(name2);
    free(name1);
    free(name2);

    int pipe1[2];
    int pipe2[2];
    if (pipe(pipe1) == -1) {
        die("pipe1");
    }
    if (pipe(pipe2) == -1) {
        die("pipe2");
    }

    int close_for_child1[] = { pipe2[0], pipe2[1], pipe1[1], file2 };
    int close_for_child2[] = { pipe1[0], pipe1[1], pipe2[1], file1 };
    pid_t child1 = spawn_child(pipe1[0], file1, close_for_child1, 4);
    printf("parent: started child1, PID=%d\n", child1);
    pid_t child2 = spawn_child(pipe2[0], file2, close_for_child2, 4);
    printf("parent: started child2, PID=%d\n", child2);

    close(pipe1[0]);
    close(pipe2[0]);
    close(file1);
    close(file2);

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        die("signal");
    }

    int exit_code = EXIT_SUCCESS;
    char *line = NULL;
    size_t cap = 0;
    ssize_t len;
    while ((len = getline(&line, &cap, stdin)) != -1) {
        size_t text_len = (size_t)len;
        if (line[len - 1] == '\n') {
            text_len--;
        }
        int target = utf8_length(line, text_len) > LENGTH_THRESHOLD ? pipe2[1] : pipe1[1];
        if (write_all(target, line, (size_t)len) == -1) {
            perror("parent: write to pipe");
            exit_code = EXIT_FAILURE;
            break;
        }
    }
    if (ferror(stdin)) {
        perror("parent: getline");
        exit_code = EXIT_FAILURE;
    }
    free(line);

    close(pipe1[1]);
    close(pipe2[1]);

    if (wait_child(child1, "child1") != 0) {
        exit_code = EXIT_FAILURE;
    }
    if (wait_child(child2, "child2") != 0) {
        exit_code = EXIT_FAILURE;
    }
    return exit_code;
}
