#include <stdio.h>
#include <string.h>
#include <stdarg.h>

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    const char *first_arg = va_arg(args, const char*);

    if (strncmp(first_arg, "password:", 9) == 0) {
        char modified_str[100];
        strncpy(modified_str, first_arg, 9);
        modified_str[9] = '\0';

        for (int i = 9; i < strlen(first_arg); i++) {
            modified_str[i] = '*';
        }

        va_end(args);
        return fwrite(modified_str, sizeof(char), strlen(modified_str), stdout);
    } else {
        return vfprintf(stdout, format, args);
    }
    return 0;
}
