// Дочерний процесс: читает строки из stdin, переворачивает их и пишет в stdout.
// К моменту запуска родитель уже перенаправил stdin на pipe, а stdout на файл,
// поэтому здесь про pipe и файлы ничего знать не нужно.

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    fprintf(stderr, "child: PID=%d\n", getpid());

    char *line = NULL;
    size_t cap = 0;
    ssize_t len;
    while((len = getline(&line, &cap, stdin)) != -1){
        if (line[len-1] == '\n'){
            reverse_string(&line[0], len - 1);
        }
        else{
            reverse_string(&line[0], len);
        }
        if (write_all(STDOUT_FILENO, line, len) == -1) {
        free(line);
        die("write");
        }
    }
    if (ferror(stdin)){
        free(line);
        die("getline");
    }
    free(line);
    return EXIT_SUCCESS;
}
