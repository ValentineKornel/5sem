#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096
#define MAX_NUMBERS 1024

pthread_mutex_t mutex;

char globalPrimes[BUFFER_SIZE];
int globalPrimesSize = 0;

__thread char buffer[1024];

typedef struct {
    int start;
    int end;
} ThreadParams;

pthread_once_t once_control = PTHREAD_ONCE_INIT;

void init() {
    pthread_mutex_init(&mutex, NULL);
}

int is_prime(int num) {
    if (num < 2) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

void* threadFunc(void* lpParam) {
    ThreadParams* param = (ThreadParams*)lpParam;

    int current_buffer_size = 0;
    int buffer_capacity = sizeof(buffer);

    
    memset(buffer, 0, buffer_capacity);

    for (int i = param->start; i <= param->end; i++) {
        if (is_prime(i)) {
            current_buffer_size += snprintf(buffer + current_buffer_size, buffer_capacity - current_buffer_size, "%d ", i);
        }
    }

    pthread_mutex_lock(&mutex);
    printf("Thread %ld in mutex...   param: %d - %d\n", pthread_self(), param->start, param->end);

    printf("Thread %ld found primes: %s\n", pthread_self(), buffer);

    if (globalPrimesSize + current_buffer_size < sizeof(globalPrimes)) {
        strcat(globalPrimes, buffer);
        globalPrimesSize += current_buffer_size;
    } else {
        printf("Global buffer overflow, can't add primes\n");
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

// Функция для сортировки чисел
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// Функция для сортировки массива простых чисел
void sortGlobalPrimes() {
    int numbers[MAX_NUMBERS];
    int numCount = 0;

    // Разбираем строку на числа
    char* token = strtok(globalPrimes, " ");
    while (token != NULL && numCount < MAX_NUMBERS) {
        numbers[numCount++] = atoi(token);
        token = strtok(NULL, " ");
    }

    // Сортируем массив чисел
    qsort(numbers, numCount, sizeof(int), compare);

    // Записываем отсортированные числа обратно в строку
    globalPrimes[0] = '\0'; // Очищаем строку
    for (int i = 0; i < numCount; i++) {
        char temp[16];
        snprintf(temp, sizeof(temp), "%d ", numbers[i]);
        strcat(globalPrimes, temp);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <number of threads> <start> <end>\n", argv[0]);
        return 1;
    }

    int numberOfThreads = atoi(argv[1]);
    int start = atoi(argv[2]);
    int end = atoi(argv[3]);
    pthread_t threads[numberOfThreads];
    int range = end - start + 1;
    int step = range / numberOfThreads;

    pthread_once(&once_control, init);

    for (int i = 0; i < numberOfThreads; i++) {
        int local_start = start + i * step;
        int local_end = local_start + step - 1;
        if (i == numberOfThreads - 1) {
            local_end = end;
        }

        ThreadParams* params = (ThreadParams*)malloc(sizeof(ThreadParams));
        params->start = local_start;
        params->end = local_end;

        pthread_create(&threads[i], NULL, threadFunc, params);
    }

    for (int i = 0; i < numberOfThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    sortGlobalPrimes();
    printf("\nGlobal primes: %s\n", globalPrimes);

    pthread_mutex_destroy(&mutex); 

    return 0;
}
