#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

void PrintInfo(const char *filePath);
void PrintText(const char *filePath);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error. Provide the file name as a parameter.\n");
        return 1;
    }

    const char *filePath = argv[1];
    PrintInfo(filePath);
    PrintText(filePath);

    return 0;
}

void PrintInfo(const char *filePath) {
    struct stat fileStat;

    // Получение информации о файле
    if (stat(filePath, &fileStat) < 0) {
        perror("Failed to get file information");
        return;
    }

    // Имя файла
    const char *fileName = strrchr(filePath, '/');
    fileName = fileName ? fileName + 1 : filePath;
    printf("File name: %s\n", fileName);

    // Размер файла
    printf("File size: %lld B (%.2f K, %.2f MB)\n",
           (long long)fileStat.st_size,
           fileStat.st_size / 1024.0,
           fileStat.st_size / (1024.0 * 1024.0));

    // Тип файла
    printf("File type: ");
    if (S_ISREG(fileStat.st_mode)) {
        printf("Regular file\n");
    } else if (S_ISDIR(fileStat.st_mode)) {
        printf("Directory\n");
    } else if (S_ISCHR(fileStat.st_mode)) {
        printf("Character device\n");
    } else if (S_ISBLK(fileStat.st_mode)) {
        printf("Block device\n");
    } else if (S_ISFIFO(fileStat.st_mode)) {
        printf("FIFO/pipe\n");
    } else if (S_ISLNK(fileStat.st_mode)) {
        printf("Symlink\n");
    } else if (S_ISSOCK(fileStat.st_mode)) {
        printf("Socket\n");
    } else {
        printf("Unknown\n");
    }

    // Время создания, последнего доступа и изменения
    struct tm lt;
    char timebuf[80];

    // Время последнего доступа
    localtime_r(&fileStat.st_atime, &lt);
    strftime(timebuf, sizeof(timebuf), "%d/%m/%Y %H:%M", &lt);
    printf("Last access time: %s\n", timebuf);

    // Время последнего изменения
    localtime_r(&fileStat.st_mtime, &lt);
    strftime(timebuf, sizeof(timebuf), "%d/%m/%Y %H:%M", &lt);
    printf("Last modification time: %s\n", timebuf);

    // Время создания
    localtime_r(&fileStat.st_ctime, &lt);
    strftime(timebuf, sizeof(timebuf), "%d/%m/%Y %H:%M", &lt);
    printf("Creation time: %s\n", timebuf);
}

void PrintText(const char *filePath) {
    // Проверка расширения файла
    const char *dot = strrchr(filePath, '.');
    if (!(dot && strcmp(dot, ".txt") == 0)) {
        fprintf(stderr, "Error: The file must have a '.txt' extension\n");
        return;
    }

    // Открытие файла для чтения
    int fd = open(filePath, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }

    // Чтение и вывод данных из файла
    char buffer;
    ssize_t bytesRead;
    while ((bytesRead = read(fd, &buffer, sizeof(buffer))) > 0) {
        putchar(buffer);
    }
    if (bytesRead == -1) {
        perror("Failed to read file");
    }

    putchar('\n');

    // Закрытие файла
    close(fd);

    printf("The file has been opened and read.\n");
}
