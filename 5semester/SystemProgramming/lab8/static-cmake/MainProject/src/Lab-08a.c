#include "Mappings.h"
#include <windows.h>

HANDLE hMapping;
LPSTR FilePath;
int lSize;

int main(int argc, char* argv[]) {
    int option;
    int rowPosition;
    struct Student newStudent;

    while (1) {
        printf("choose option: \n1.Open file.\n2.Input row.\n3.Remove row.\n4.Print row.\n5.Print file.\n6.Close file.\n0.Exit.\n");

        if ((scanf("%d", &option) != 1) || option > 6 || option < 0) {
            printf("Wrong input command!\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (option) {
        case 0:
            printf("Exit the program");
            exit(0);
        case 1:
            FilePath = (char*)malloc(100 * sizeof(char));
            printf("Eneter file path\n");
            if (!fgets(FilePath, 100, stdin)) {
                printf("Can't read file path\n");
                break;
            }
            size_t len = strlen(FilePath);
            if (len > 0 && FilePath[len - 1] == '\n') {
                FilePath[len - 1] = '\0';
            }

            printf("input list maxsize in case of the lack of the mapping:\n");
            scanf("%d", &lSize);
            while (getchar() != '\n');
            MyOpenFile(&hMapping, FilePath, &lSize);
            break;
        case 2:
            printf("Enter new student details (Name, Surname, Course, Group, ID):\n");
            scanf("%s %s %hhu %hhu %s", newStudent.Name, newStudent.Surname, &newStudent.Course, &newStudent.Group, newStudent.ID);
            while (getchar() != '\n');
            printf("Enter position to insert (-1 for end): ");
            scanf("%d", &rowPosition);
            while (getchar() != '\n');

            AddRow(hMapping, newStudent, rowPosition);
            break;
        case 3:
            printf("Enter row index to delete\n");
            scanf("%d", &rowPosition);
            while (getchar() != '\n');

            RemRow(hMapping, rowPosition);
            break;
        case 4:
            printf("Enter row index to print\n");
            scanf("%d", &rowPosition);
            while (getchar() != '\n');

            PrintRow(hMapping, rowPosition);
            break;
        case 5:
            PrintRows(hMapping);
            break;
        case 6:
            CloseFile(hMapping);
            if (FilePath != NULL)
                free(FilePath);
            break;
        }
        system("pause & cls");
    }

    return 0;
}
