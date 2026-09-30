#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <sys/types.h>

// Строки длиннее этого порога идут в pipe2, остальные в pipe1 (вариант 20)
#define LENGTH_THRESHOLD 10

// Печатает сообщение об ошибке через perror() и завершает процесс с кодом ошибки
void die(const char *msg);

// Пишет в fd ровно len байт (write() может записать меньше, чем просили).
// Возвращает 0 при успехе, -1 при ошибке (errno выставлен).
int write_all(int fd, const void *buf, size_t len);

// Переворачивает строку str длины len на месте
void reverse_string(char *str, size_t len);

#endif
