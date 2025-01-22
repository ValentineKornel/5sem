#include <stdio.h>
#include <time.h>

int main() {
    // Переменные для подсчета времени
    clock_t start_cpu_time, current_cpu_time;
    time_t start_real_time, current_real_time;

    // Инициализация времени
    start_cpu_time = clock();
    start_real_time = time(NULL);

    long iterations = 0;  // Счетчик итераций

    while (1) {
        iterations++;  // Увеличиваем счетчик итераций

        // Получаем текущее процессорное время
        current_cpu_time = clock();
        double cpu_time_used = ((double)(current_cpu_time - start_cpu_time)) / CLOCKS_PER_SEC;

        // Получаем текущее реальное время
        current_real_time = time(NULL);
        double real_time_used = difftime(current_real_time, start_real_time);

        // Проверяем, прошло ли 2 секунды реального времени
        if (real_time_used >= 2) {
            printf("Iterations: %ld\n", iterations);
            printf("Real time elapsed: %.2f seconds\n", real_time_used);
            printf("CPU time used: %.2f seconds\n", cpu_time_used);

            // Завершаем цикл по прошествии 2 секунд реального времени
            break;
        }
    }

    return 0;
}

