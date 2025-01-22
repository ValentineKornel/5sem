#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int hFile = -1;
char* fileBuffer = NULL;  // Buffer for storing file content
char* newRow = NULL;

//const char* filePath = "/home/valentine/labs/sp/lab6/build/file.txt"; 
char* filePath = NULL;
size_t len = 0;

void MyOpenFile(const char* filePath);
void AddRow(const char* row, int pos);
void RemRow(int pos);
void PrintRow(int pos);
void PrintRows();
void CloseFile();

int main() {
    int option;
    int rowPosition;

    while (1) {
        printf("choose option: 1.Open file. 2.Input row. 3.Remove row. 4.Print row. 5.Print file. 6.Close file. 0.Exit.\n");
        //printf("Choose option: \n1.Open file.\n2.Input row.\n3.Remove row.\n4.Print row.\n5.Print file.\n6.Close file.\n0.Exit.\n");

        if ((scanf("%d", &option) != 1) || option > 6 || option < 0) {
            printf("Invalid command!\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n'); // Clear the input buffer

        switch (option) {
            case 0:
                printf("Exiting program\n");
                exit(0);
            case 1:
                printf("Eneter filepath:\n");
                ssize_t read = getline(&filePath, &len, stdin);
                if (read == -1) {
                    perror("Error reading input");
                    return 1;
                }
                if (filePath[read - 1] == '\n') {
                    filePath[read - 1] = '\0';
                }
                MyOpenFile(filePath);
                break;
            case 2:
                if (newRow == NULL) {
                    fprintf(stderr, "Error: Memory for the string wasn't allocated.\n");
                    break;
                }
                printf("Enter new row:\n");
                if (!fgets(newRow, 100, stdin)) {
                    printf("Can't read new row");
                    break;
                }
                printf("Enter new row position:");
                if (!(scanf("%d", &rowPosition))) {
                    printf("Can't read new row position");
                    break;
                }
                AddRow(newRow, rowPosition);
                break;
            case 3:
                printf("Enter row index to delete\n");
                if ((scanf("%d", &rowPosition) < 1) || rowPosition < -1) {
                    printf("Invalid command!\n");
                    while (getchar() != '\n');
                    continue;
                }
                RemRow(rowPosition);
                break;
            case 4:
                printf("Enter row index to print\n");
                if ((scanf("%d", &rowPosition) < 1) || rowPosition < -1) {
                    printf("Invalid command!\n");
                    while (getchar() != '\n');
                    continue;
                }
                PrintRow(rowPosition);
                break;
            case 5:
                PrintRows();
                break;
            case 6:
                CloseFile();
                break;
        }

    }

    return 0;
}

void MyOpenFile(const char* filePath) {
    hFile = open(filePath, O_RDWR);
    if (hFile == -1) {
        fprintf(stderr, "Failed to open file. Error: %s\n", strerror(errno));
        return;
    }

    newRow = (char*)malloc(100 * sizeof(char));
    if (newRow == NULL) {
        fprintf(stderr, "Failed to allocate memory for new row\n");
        return;
    }

    printf("File successfully opened.\n");
}

void AddRow(const char* row, int pos) {
    if (hFile == -1 || row == NULL) {
        fprintf(stderr, "Error: file not opened or row is NULL\n");
        return;
    }

    off_t fileSize = lseek(hFile, 0, SEEK_END);
    if (fileSize == -1) {
        fprintf(stderr, "Failed to get file size. Error: %s\n", strerror(errno));
        return;
    }

    char* buffer = (char*)malloc(fileSize);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return;
    }

    if (pos == -1) { // Append at the end
        if (fileSize > 0) {
        char lastChar;
        // Читаем последний символ файла
        if (lseek(hFile, fileSize - 1, SEEK_SET) == -1) {
            perror("Ошибка при переходе к последнему символу");
            return;
        }

        if (read(hFile, &lastChar, 1) != 1) {
            perror("Ошибка при чтении последнего символа");
            return;
        }

        // Если последний символ не новая строка, добавляем её
        if (lastChar != '\n') {
            if (write(hFile, "\n", 1) != 1) {
                perror("Ошибка при записи символа новой строки");
                return;
            }
        }
        }
        if (lseek(hFile, 0, SEEK_END) == -1 || write(hFile, row, strlen(row)-1) == -1) {
            fprintf(stderr, "Error writing to the end of the file. Error: %s\n", strerror(errno));
            free(buffer);
            return;
        }
    } else if (pos == 0) { // Add at the beginning
        lseek(hFile, 0, SEEK_SET);
        read(hFile, buffer, fileSize);
        lseek(hFile, 0, SEEK_SET);
        write(hFile, row, strlen(row));
        write(hFile, buffer, fileSize);
    } else {
        // Add in the middle
        lseek(hFile, 0, SEEK_SET);
        read(hFile, buffer, fileSize);
        
        int line = 1, i = 0;
        while (line < pos && i < fileSize) {
            if (buffer[i] == '\n') line++;
            i++;
        }
        if (line != pos) {
            fprintf(stderr, "Invalid index\n");
            free(buffer);
            return;
        }

        lseek(hFile, i, SEEK_SET);
        write(hFile, row, strlen(row));
        write(hFile, buffer + i, fileSize - i);
    }

    free(buffer);
    printf("Row added successfully.\n");
}

void RemRow(int pos) {
    if (hFile == -1 || pos < -1) {
        fprintf(stderr, "Error: invalid file descriptor or index\n");
        return;
    }

    off_t fileSize = lseek(hFile, 0, SEEK_END);
    char* buffer = (char*)malloc(fileSize);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return;
    }

    lseek(hFile, 0, SEEK_SET);
    read(hFile, buffer, fileSize);


    int currentLine = 1, startPos = 0, endPos = 0, found = 0;

    if (pos == 0) {
    // Удаляем первую строку
    endPos = 0;
    while (endPos < fileSize && buffer[endPos] != '\n') {
        endPos++;
    }
    endPos++; // Включаем символ новой строки
    found = 1;
    }
    else if (pos == -1) {
        // Удаляем последнюю строку
        int i = fileSize - 1;
        while (i >= 0 && buffer[i] != '\n' && i > 0) {
            i--;
        }
        startPos = i; //!!!!!!!!!!!!!!!!!
        endPos = fileSize;
        found = 1;
    }
    else {
        // Удаляем строку по индексу pos
        for (int i = 0; i < fileSize; i++) {
            if (buffer[i] == '\n') {
                if (currentLine == pos) {
                    endPos = i + 1;  // Конец строки для удаления
                    found = 1;
                    break;
                }
                currentLine++;
                startPos = i + 1;  // Начало следующей строки
            }
        }
    }

    if (!found) {
        fprintf(stderr, "Error. Index is out of bounds\n");
        free(buffer);
        return;
    }

// Сдвигаем данные после строки вверх на место удаляемой строки
    memmove(buffer + startPos, buffer + endPos, fileSize - endPos);

    lseek(hFile, 0, SEEK_SET);
    write(hFile, buffer, fileSize - (endPos - startPos));
    ftruncate(hFile, fileSize - (endPos - startPos));

    free(buffer);
    printf("Row deleted successfully.\n");
}

void PrintRow(int pos) {
    if (hFile == -1 || pos < -1) {
        fprintf(stderr, "Error: invalid file descriptor or index\n");
        return;
    }

    // Получаем размер файла
    off_t fileSize = lseek(hFile, 0, SEEK_END);
    if (fileSize == -1) {
        perror("Error getting file size");
        return;
    }

    // Выделяем буфер для чтения файла
    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return;
    }

    // Читаем файл в буфер
    lseek(hFile, 0, SEEK_SET);
    if (read(hFile, buffer, fileSize) != fileSize) {
        perror("Error reading file");
        free(buffer);
        return;
    }
    buffer[fileSize] = '\0';

    // Логика поиска строки
    int line = 1;
    char* lineStart = buffer;
    char* lastLineStart = NULL;

    // Определяем начало строк
    for (char* ptr = buffer; *ptr; ptr++) {
        if (*ptr == '\n') {
            line++;
            lastLineStart = ptr + 1; // Указатель на начало последней строки
        }
    }

    if (pos == -1) {
        // Если задана позиция -1, выводим последнюю строку
        if (lastLineStart) {
            lineStart = lastLineStart;
        } else {
            lineStart = buffer; // Если файл содержит только одну строку
        }
        pos = line - 1; // Устанавливаем номер последней строки
    } else {
        // Ищем строку с указанным индексом
        line = 1;
        lineStart = buffer;
        while (*lineStart && line < pos) {
            if (*lineStart == '\n') line++;
            lineStart++;
        }

        if (line != pos || *lineStart == '\0') {
            fprintf(stderr, "Row with index %d not found.\n", pos);
            free(buffer);
            return;
        }
    }

    // Вывод строки
    printf("Row %d: ", pos);
    while (*lineStart && *lineStart != '\n') {
        putchar(*lineStart++);
    }
    putchar('\n');

    free(buffer);
}


void PrintRows() {
    if (hFile == -1) {
        fprintf(stderr, "Error: file not opened\n");
        return;
    }

    off_t fileSize = lseek(hFile, 0, SEEK_END);
    char* buffer = (char*)malloc(fileSize);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return;
    }

    lseek(hFile, 0, SEEK_SET);
    read(hFile, buffer, fileSize);

    write(STDOUT_FILENO, buffer, fileSize);
    free(buffer);
    putchar('\n');
}

void CloseFile() {
    if (hFile == -1) {
        fprintf(stderr, "Error: file not opened\n");
        return;
    }

    close(hFile);
    hFile = -1;
    printf("File closed successfully.\n");
}
