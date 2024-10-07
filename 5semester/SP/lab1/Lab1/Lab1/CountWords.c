#include <stdio.h>
#include <ctype.h>

int CountWords(const char* s) {
    if (s == NULL) {
        return -1;
    }

    int word_count = 0;
    int in_word = 0;

    for (int i = 0; s[i] != '\0'; i++) {
        if (isalnum(s[i])) {  // Если символ является буквой или цифрой
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        }
        else {
            in_word = 0;
        }
    }

    return word_count;
}

//int CountWords(const char* s) {
//    if (s == NULL) {
//        return -1;
//    }
//
//    int word_count = 0;
//    int in_word = 0;
//
//    while (*s != '\0') {
//        if (isalnum(*s)) {
//            if (!in_word) {
//                word_count++;
//                in_word = 1;
//            }
//        }
//        else {
//            in_word = 0;
//        }
//        s++;  // Переход к следующему символу
//    }
//
//    return word_count;
//}

int main() {
    const char* str = "Hello, world! This is a test";
    printf(str);
    printf(" -  %d\n", CountWords(str));

    str = "Wow... Amazing!";
    printf(str);
    printf(" -  %d\n", CountWords("Wow... Amazing!"));

    str = "It's a well-known fact.";
    printf(str);
    printf(" -  %d\n", CountWords("It's a well-known fact."));

    str = "";
    printf(str);
    printf(" -  %d\n", CountWords(str));

    str = ",.!;:";
    printf(str);
    printf(" -  %d\n", CountWords(",.!;:"));

    str = "123 456 789";
    printf(str);
    printf(" -  %d\n", CountWords("123 456 789"));

    str = "Hello\tworld\nGoodbye";
    printf(str);
    printf(" -  %d\n", CountWords("Hello\tworld\nGoodbye"));

    str = "Hello,world";
    printf(str);
    printf(" -  %d\n", CountWords("Hello,world"));

    str = ",,,Hello world,,,,";
    printf(str);
    printf(" -  %d\n", CountWords(",,,Hello world,,,,"));

    str = "a b c";
    printf(str);
    printf(" -  %d\n", CountWords("a b c"));

    str = "(((Hello))) (((world)))";
    printf(str);
    printf(" -  %d\n", CountWords("(((Hello))) (((world)))"));

    str = "Hello 😃 world 🌍";
    printf(str);
    printf(" -  %d\n", CountWords("Hello 😃 world 🌍"));

    return 0;
}
