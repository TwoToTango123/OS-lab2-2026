// Родительский процесс: создаёт два дочерних процесса и распределяет между ними
// строки пользователя. Длиной <= LENGTH_THRESHOLD идут в pipe1 (child1), длиннее в pipe2 (child2).

#include "utils.h"

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// CMake передаёт полный путь к child через -DCHILD_PATH=...
#ifndef CHILD_PATH
#define CHILD_PATH "./child"
#endif

// Создаёт дочерний процесс: stdin <- pipe_read_fd, stdout -> file_fd, затем exec(CHILD_PATH).
// Возвращает PID ребёнка.
static pid_t spawn_child(int pipe_read_fd, int file_fd) {
    // TODO: fork()
    //   в ребёнке (pid == 0):
    //     - dup2(pipe_read_fd, STDIN_FILENO)
    //     - dup2(file_fd, STDOUT_FILENO)
    //     - закрыть ВСЕ лишние дескрипторы (иначе кто-то не получит EOF)
    //     - execl/execv(CHILD_PATH, ...); если exec вернулся, значит ошибка
    //   в родителе: вернуть pid
    (void)pipe_read_fd;
    (void)file_fd;
    return -1;
}

int main(void) {
    // TODO 1: вывести свой PID

    // TODO 2: прочитать два имени файлов (getline), отрезать '\n'

    // TODO 3: открыть оба файла: open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644)

    // TODO 4: создать pipe1 и pipe2 через pipe()

    // TODO 5: spawn_child() для child1 и child2, вывести их PID.
    //         Подумайте, какие концы pipe2 должны быть закрыты в child1 и наоборот

    // TODO 6: закрыть в родителе то, что ему больше не нужно:
    //         читающие концы pipe'ов и дескрипторы файлов

    // TODO 7: signal(SIGPIPE, SIG_IGN), чтобы падение ребёнка не убило родителя
    //         и write() вернул ошибку EPIPE

    // TODO 8: цикл getline() до EOF:
    //           длина без '\n' > LENGTH_THRESHOLD -> write_all(pipe2[1], ...)
    //           иначе                              -> write_all(pipe1[1], ...)

    // TODO 9: закрыть пишущие концы pipe1[1] и pipe2[1], чтобы дети получили EOF

    // TODO 10: waitpid() для обоих детей, проверить статус (WIFEXITED/WEXITSTATUS)

    // TODO 11: free() буфера строки

    return EXIT_SUCCESS;
}
