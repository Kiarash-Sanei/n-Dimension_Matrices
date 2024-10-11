#include <stdio.h>
#include <stdlib.h>

// Function to create an n-dimensional matrix
void *createNDMatrix(int *dimensions, int dimensionCount, int currentDimension)
{
    if (currentDimension == dimensionCount - 1)
    {
        return malloc(*(dimensions + currentDimension) * sizeof(int));
    }

    void **matrix = malloc(*(dimensions + currentDimension) * sizeof(void *));
    for (int i = 0; i < *(dimensions + currentDimension); i++)
    {
        *(((void **)matrix) + i) = createNDMatrix(dimensions, dimensionCount, currentDimension + 1);
    }
    return matrix;
}

// Function to free an n-dimensional matrix
void freeNDMatrix(void *matrix, int *dimensions, int dimensionCount, int currentDimension)
{
    if (currentDimension == dimensionCount - 1)
    {
        free(matrix);
        return;
    }

    for (int i = 0; i < *(dimensions + currentDimension); i++)
    {
        freeNDMatrix(*(((void **)matrix) + i), dimensions, dimensionCount, currentDimension + 1);
    }
    free(matrix);
}

// Function to fill the matrix with input values
void fillNDMatrix(void *matrix, int *dimensions, int dimensionCount, int currentDimension, int *values)
{
    if (currentDimension == dimensionCount - 1)
    {
        for (int i = 0; i < dimensions[currentDimension]; i++)
        {
            ((int *)matrix)[i] = values[i];
        }
        return;
    }

    int subSize = 1;
    for (int i = currentDimension + 1; i < dimensionCount; i++)
    {
        subSize *= dimensions[i];
    }

    for (int i = 0; i < dimensions[currentDimension]; i++)
    {
        fillNDMatrix(((void **)matrix)[i], dimensions, dimensionCount, currentDimension + 1, values + i * subSize);
    }
}

// Function to print an n-dimensional matrix in one line
void printNDMatrix(void *matrix, int *dimensions, int dimensionCount, int currentDimension) {
    if (currentDimension == dimensionCount - 1) {
        // Base case: print the elements of the last dimension
        for (int i = 0; i < dimensions[currentDimension]; i++) {
            printf("%d ", ((int *)matrix)[i]);
        }
        return;
    }

    // Recursive case: print each sub-matrix
    for (int i = 0; i < dimensions[currentDimension]; i++) {
        printNDMatrix(((void **)matrix)[i], dimensions, dimensionCount, currentDimension + 1);
    }
}

int main()
{
    // Read dimension count
    int dimensionCount;
    scanf("%d", &dimensionCount);

    // Allocate memory for dimensions
    int *dimensions = (int *)malloc(dimensionCount * sizeof(int));

    // Read dimensions and calculate total number of elements
    int totalElements = 1;
    for (int i = 0; i < dimensionCount; i++)
    {
        scanf("%d", dimensions + i);
        totalElements *= *(dimensions + i);
    }

    // Create the n-dimensional matrices
    void *matrix1 = createNDMatrix(dimensions, dimensionCount, 0);
    void *matrix2 = createNDMatrix(dimensions, dimensionCount, 0);

    // Read input for matrix1
    int *input = (int *)malloc(totalElements * sizeof(int));
    for (int i = 0; i < totalElements; i++)
    {
        scanf("%d", input + i);
    }
    fillNDMatrix(matrix1, dimensions, dimensionCount, 0, input);

    // Read input for matrix2
    for (int i = 0; i < totalElements; i++)
    {
        scanf("%d", input + i);
    }
    fillNDMatrix(matrix2, dimensions, dimensionCount, 0, input);

    // Free temporary array
    free(input);

    // Print dimensions
    // printf("%d\n", dimensionCount);
    // for (int i = 0; i < dimensionCount; i++)
    // {
    //     printf("%d ", *(dimensions + i));
    // }
    // printf("\n");

    // Print matrix1
    printf("Matrix 1: ");
    printNDMatrix(matrix1, dimensions, dimensionCount, 0);
    printf("\n");

    // Print matrix2
    printf("Matrix 2: ");
    printNDMatrix(matrix2, dimensions, dimensionCount, 0);
    printf("\n");

    // Free the n-dimensional matrices and dimensions
    freeNDMatrix(matrix1, dimensions, dimensionCount, 0);
    freeNDMatrix(matrix2, dimensions, dimensionCount, 0);
    free(dimensions);
    return 0;
}