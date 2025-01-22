#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    int iterations = 0;

    if (argc == 2) {
        iterations = atoi(argv[1]);
        if (iterations <= 0) {
            fprintf(stderr, "Ошибка: количество итераций должно быть положительным числом.\n");
            exit(1);
        }
    } else {
        const char* envIter = getenv("ITER_NUM");
        if (envIter != NULL) {
            iterations = atoi(envIter);
        }
    }

    if (iterations <= 0) {
        fprintf(stderr, "Ошибка: необходимо указать количество итераций через аргумент командной строки или переменную окружения ITER_NUM.\n");
        exit(1);
    }

    printf("Количество итераций: %d\n", iterations);

    pid_t processId = getpid();

    for (int i = 1; i <= iterations; i++) {
        printf("Итерация %d, PID: %d\n", i, processId);
        usleep(500000);
    }

    printf("Завершение программы.\n");
    printf("Нажмите любую клавишу для продолжения...\n");
    getchar();
    return 0;
}
