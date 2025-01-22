#include "Mappings.h"

int fd;
char filePath[100];
int lSize;

int main() {
    int option;
    int rowPosition;
    struct Student newStudent;

    while (1) {
        printf("Choose option: \n1.Open file.\n2.Input row.\n3.Remove row.\n4.Print row.\n5.Print file.\n6.Close file.\n0.Exit.\n");

        if (scanf("%d", &option) != 1 || option < 0 || option > 6) {
            printf("Wrong input command!\n");
            while (getchar() != '\n' );
            printf("here");
            continue;
        }
        while (getchar() != '\n');

        switch (option) {
        case 0:
            printf("Exit the program");
            exit(0);
        case 1:
            printf("Eneter file path\n");
            scanf("%s", filePath);
            while (getchar() != '\n');
            printf("Input list max size:\n");
            scanf("%d", &lSize);
            while (getchar() != '\n');

            MyOpenFile(fd, filePath, &lSize);
            break;
        case 2:
            printf("Enter new student details (Name, Surname, Course, Group, ID):\n");
            scanf("%s %s %hhu %hhu %s", newStudent.Name, newStudent.Surname, &newStudent.Course, &newStudent.Group, newStudent.ID);
            while (getchar() != '\n');
            printf("Enter position to insert (-1 for end): ");
            scanf("%d", &rowPosition);
            while (getchar() != '\n');

            AddRow(fd, newStudent, rowPosition);
            break;
        case 3:
            printf("Enter row index to delete\n");
            scanf("%d", &rowPosition);
            while (getchar() != '\n');

            RemRow(fd, rowPosition);
            break;
        case 4:
            printf("Enter row index to print\n");
            scanf("%d", &rowPosition);
            while (getchar() != '\n');

            PrintRow(fd, rowPosition);
            break;
        case 5:
            PrintRows(fd);
            break;
        case 6:
            CloseFile(fd);
            break;
        }
    }

    return 0;
}