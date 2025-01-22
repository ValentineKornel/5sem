#include <iostream>
#include <unistd.h>

int main() {
    std::cout << "Приложение OS08_03 запущено. Нажмите Ctrl+C для выхода." << std::endl;

    volatile int counter = 0;
    while (true) {
        ++counter;  // Бесконечный цикл увеличения счетчика
        if (counter % 1000000000 == 0) {
            std::cout << "Текущий счетчик: " << counter << std::endl;
        }
        usleep(1000);  // Небольшая задержка
    }

    return 0;
}

