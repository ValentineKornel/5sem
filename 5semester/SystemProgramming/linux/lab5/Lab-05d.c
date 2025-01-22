#include <stdio.h>
#include <stdlib.h>

int globalVariableInitialized = 2;
int globalVariableNotInitialized;

static int globalStaticVariableInitialized = 3;
static int globalStaticVariableNotInitialized;

void myFunction() { }

int main(int argc, char* argv[])
{
    int localVariableInitialized = 4;
    int localVariableNotInitialized;

    static int localStaticVariableInitialized = 5;
    static int localStaticVariableNotInitialized;

    printf("Address of globalVariableInitialized: %p\n", &globalVariableInitialized);
    printf("Address of globalVariableNotInitialized: %p\n", &globalVariableNotInitialized);

    printf("Address of globalStaticVariableInitialized: %p\n", &globalStaticVariableInitialized);
    printf("Address of globalStaticVariableNotInitialized: %p\n", &globalStaticVariableNotInitialized);

    printf("Address of localVariableInitialized: %p\n", &localVariableInitialized);
    printf("Address of localVariableNotInitialized: %p\n", &localVariableNotInitialized);

    printf("Address of localStaticVariableInitialized: %p\n", &localStaticVariableInitialized);
    printf("Address of localStaticVariableNotInitialized: %p\n", &localStaticVariableNotInitialized);

    printf("Address of argc: %p\n", &argc);
    printf("Address of argv: %p\n", &argv);

    printf("Address of myFunction: %p\n", &myFunction);


    printf("Press any key to exit...\n");
    getchar();

    return 0;
}


