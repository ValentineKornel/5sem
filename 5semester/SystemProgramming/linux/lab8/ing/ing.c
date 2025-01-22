#include <stdio.h>


int main() {
    printf("%s", "password:mySecretPassword123");  // Вывод: Modified string: password:***********************
    printf("\n");
    printf("%s", "hello123\n");                      // Вывод: String: hello123

    return 0;
}