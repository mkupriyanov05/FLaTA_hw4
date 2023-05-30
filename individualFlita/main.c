#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_NUMB 1000

typedef struct currEdge {
    int firstDot;
    int secondDot;
} CurrEdge_t;

typedef struct dot {
    int bindNumber;
    int *bindTo;
} Dot_t;

typedef struct withMinBinds {
    int Min_BindNumber;
    int *Min_BindTo;
} MinBinds_t;


void findMinBinds(Dot_t *dots, int maxNumber, MinBinds_t *minConnections);

int main(int argc, char **argv) {

    time_t startTime = time(NULL);

    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        exit(1);
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        printf("Could not open file %s\n", argv[1]);
        exit(1);
    }

    CurrEdge_t currentEdge;
    Dot_t *dots = malloc(sizeof(Dot_t) * MAX_NUMB);

    for (int i = 0; i < MAX_NUMB; ++i) {
        dots[i].bindNumber = 0;
        dots[i].bindTo = malloc(sizeof(int));
    }

    int maxDotNumber = -1;
    int numberOfEdges = 0;

    while (!feof(file)) {
        fscanf(file, "%d -- %d", &currentEdge.firstDot, &currentEdge.secondDot);
        numberOfEdges++;
        if (currentEdge.firstDot > maxDotNumber)
            maxDotNumber = currentEdge.firstDot;
        if (currentEdge.secondDot > maxDotNumber)
            maxDotNumber = currentEdge.secondDot;

        dots[currentEdge.firstDot].bindNumber++;
        dots[currentEdge.firstDot].bindTo = realloc(dots[currentEdge.firstDot].bindTo,
                                                           sizeof(int) * dots[currentEdge.firstDot].bindNumber);
        dots[currentEdge.firstDot].bindTo[dots[currentEdge.firstDot].bindNumber] = currentEdge.secondDot;


        dots[currentEdge.secondDot].bindNumber++;
        dots[currentEdge.secondDot].bindTo = realloc(dots[currentEdge.secondDot].bindTo,
                                                           sizeof(int) * dots[currentEdge.secondDot].bindNumber);
        dots[currentEdge.secondDot].bindTo[dots[currentEdge.secondDot].bindNumber] = currentEdge.firstDot;
    }

    MinBinds_t *minBinds = malloc(sizeof(MinBinds_t));
    findMinBinds(dots, maxDotNumber, minBinds);

    for (int i = 0; i <= minBinds->Min_BindNumber; ++i) {
        printf("\nDot %d connected with: ", minBinds->Min_BindTo[i]);
        for (int j = 1; j <= dots[minBinds->Min_BindTo[i]].bindNumber; ++j) {
            printf("%d ", dots[minBinds->Min_BindTo[i]].bindTo[j]);
        }
    }

    for (int i = 0; i < MAX_NUMB; ++i)
        free(dots[i].bindTo);
    free(dots);
    free(minBinds->Min_BindTo);
    free(minBinds);

    fclose(file);

    printf("\nNumber of edges: %d", numberOfEdges);

    time_t endTime = time(NULL);

    printf("\n\nProgram takes: %lld secs", endTime - startTime);

    return 0;
}


void findMinBinds(Dot_t *dots, int maxNumber, MinBinds_t *minConnections) {
    int currMinValue = INT_MAX;
    minConnections->Min_BindNumber = 0;
    minConnections->Min_BindTo = malloc(sizeof(int));

    for (int i = 0; i <= maxNumber; ++i) {
        if (dots[i].bindNumber < currMinValue) {
            currMinValue = dots[i].bindNumber;
            minConnections->Min_BindNumber = 0;
            minConnections->Min_BindTo = realloc(minConnections->Min_BindTo, sizeof(int));
            minConnections->Min_BindTo[minConnections->Min_BindNumber] = i;
        } else if (dots[i].bindNumber == currMinValue) {
            minConnections->Min_BindNumber++;
            minConnections->Min_BindTo = realloc(minConnections->Min_BindTo,
                                                      sizeof(int) * (minConnections->Min_BindNumber + 1));
            minConnections->Min_BindTo[minConnections->Min_BindNumber] = i;
        }
    }
}
