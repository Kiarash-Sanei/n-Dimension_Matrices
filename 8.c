#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to create an n-dimensional matrix
void *createNDMatrix(int *dimensions, int dimensionCount, int currentDimension)
{
    if (currentDimension == dimensionCount - 1)
    {
        // Base case: allocate memory for the innermost dimension
        return malloc(*(dimensions + currentDimension) * sizeof(int));
    }

    // Allocate memory for pointers to sub-matrices
    void **matrix = malloc(*(dimensions + currentDimension) * sizeof(void *));
    for (int i = 0; i < *(dimensions + currentDimension); i++)
    {
        // Recursively create sub-matrices
        *(matrix + i) = createNDMatrix(dimensions, dimensionCount, currentDimension + 1);
    }
    return matrix;
}

// Function to free an n-dimensional matrix
void freeNDMatrix(void *matrix, int *dimensions, int dimensionCount, int currentDimension)
{
    if (currentDimension == dimensionCount - 1)
    {
        // Base case: free the innermost dimension
        free(matrix);
        return;
    }

    // Recursively free sub-matrices
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
        // Base case: fill the innermost dimension
        for (int i = 0; i < *(dimensions + currentDimension); i++)
        {
            *((int *)matrix + i) = *(values + i);
        }
        return;
    }

    // Calculate the size of sub-matrices
    int subSize = 1;
    for (int i = currentDimension + 1; i < dimensionCount; i++)
    {
        subSize *= *(dimensions + i);
    }

    // Recursively fill sub-matrices
    for (int i = 0; i < *(dimensions + currentDimension); i++)
    {
        fillNDMatrix(*(((void **)matrix) + i), dimensions, dimensionCount, currentDimension + 1, values + i * subSize);
    }
}

// Function to print an n-dimensional matrix in one line
void printNDMatrix(void *matrix, int *dimensions, int dimensionCount, int currentDimension, int *coordinates)
{
    if (currentDimension == dimensionCount - 1)
    {
        // Base case: print the elements of the innermost dimension with coordinates
        for (int i = 0; i < *(dimensions + currentDimension); i++)
        {
            coordinates[currentDimension] = i;
            printf("(");
            for (int j = 0; j < dimensionCount; j++)
            {
                printf("%d", coordinates[j] + 1);
                if (j < dimensionCount - 1)
                    printf(",");
            }
            printf("): %d\n", *((int *)matrix + i));
        }
        return;
    }

    // Recursively print sub-matrices
    for (int i = 0; i < *(dimensions + currentDimension); i++)
    {
        coordinates[currentDimension] = i;
        void *subMatrix = *(((void **)matrix) + i);
        printNDMatrix(subMatrix, dimensions, dimensionCount, currentDimension + 1, coordinates);
    }
}

// Function to perform linear operation C = αA + βB
void linearOperation(void *A, void *B, void *C, int *dimensions, int dimensionCount, int currentDimension, double alpha, double beta)
{
    if (currentDimension == dimensionCount - 1)
    {
        // Base case: perform the operation on the innermost dimension
        for (int i = 0; i < *(dimensions + currentDimension); i++)
        {
            *((int *)C + i) = alpha * (*((int *)A + i)) + beta * (*((int *)B + i));
        }
        return;
    }

    // Recursively perform the operation on sub-matrices
    for (int i = 0; i < *(dimensions + currentDimension); i++)
    {
        linearOperation(*(((void **)A) + i), *(((void **)B) + i), *(((void **)C) + i), dimensions, dimensionCount, currentDimension + 1, alpha, beta);
    }
}

// Helper function to flatten the old matrix
void flattenMatrix(void *m, int *dims, int count, int current, int *tempArr, int *idx)
{
    if (current == count - 1)
    {
        // Base case: copy elements to tempArray
        for (int i = 0; i < *(dims + current); i++)
        {
            *(tempArr + *idx) = *((int *)m + i);
            (*idx)++;
        }
        return;
    }

    // Recursively flatten sub-matrices
    for (int i = 0; i < *(dims + current); i++)
    {
        flattenMatrix(*(((void **)m) + i), dims, count, current + 1, tempArr, idx);
    }
}

// Helper function to get element from n-dimensional matrix
void *getElement(void *matrix, int *coords, int *dimensions, int dimensionCount)
{
    for (int i = 0; i < dimensionCount - 1; i++)
    {
        matrix = ((void **)matrix)[coords[i]];
    }
    return (int *)matrix + coords[dimensionCount - 1];
}

// Function to transpose the first two dimensions of a matrix
void transpose(void **matrix1, void **matrix2, int *dimensions, int dimensionCount)
{
    if (dimensionCount < 2)
    {
        return;
    }

    // Calculate total number of elements
    int totalElements = 1;
    for (int i = 0; i < dimensionCount; i++)
    {
        totalElements *= dimensions[i];
    }

    // Create new matrices with swapped dimensions
    int newDimensions[dimensionCount];
    newDimensions[0] = dimensions[1];
    newDimensions[1] = dimensions[0];
    for (int i = 2; i < dimensionCount; i++)
    {
        newDimensions[i] = dimensions[i];
    }

    void *newMatrix1 = createNDMatrix(newDimensions, dimensionCount, 0);
    void *newMatrix2 = createNDMatrix(newDimensions, dimensionCount, 0);

    // Perform transposition
    int *oldCoords = (int *)calloc(dimensionCount, sizeof(int));
    int *newCoords = (int *)calloc(dimensionCount, sizeof(int));

    for (int i = 0; i < totalElements; i++)
    {
        // Calculate old coordinates
        int temp = i;
        for (int j = dimensionCount - 1; j >= 0; j--)
        {
            oldCoords[j] = temp % dimensions[j];
            temp /= dimensions[j];
        }

        // Calculate new coordinates 
        temp = i;
        for (int j = dimensionCount - 1; j >= 0; j--)
        {
            newCoords[j] = temp % newDimensions[j];
            temp /= newDimensions[j];
        }

        // Copy values to new matrices
        int oldValue1 = *((int *)getElement(*matrix1, oldCoords, dimensions, dimensionCount));
        int oldValue2 = *((int *)getElement(*matrix2, oldCoords, dimensions, dimensionCount));

        *((int *)getElement(newMatrix1, newCoords, newDimensions, dimensionCount)) = oldValue1;
        *((int *)getElement(newMatrix2, newCoords, newDimensions, dimensionCount)) = oldValue2;
    }

    // Free old matrices and update pointers
    freeNDMatrix(*matrix1, dimensions, dimensionCount, 0);
    freeNDMatrix(*matrix2, dimensions, dimensionCount, 0);
    *matrix1 = newMatrix1;
    *matrix2 = newMatrix2;

    // Update dimensions
    dimensions[0] = newDimensions[0];
    dimensions[1] = newDimensions[1];

    // Free temporary arrays
    free(oldCoords);
    free(newCoords);
}

// Function to reshape the matrix
void reshape(void *matrix, int *oldDimensions, int *newDimensions, int oldDimensionCount, int newDimensionCount)
{
    // Calculate total number of elements
    int totalElements = 1;
    for (int i = 0; i < oldDimensionCount; i++)
    {
        totalElements *= *(oldDimensions + i);
    }

    // Create a temporary array to store flattened matrix
    int *tempArray = (int *)malloc(totalElements * sizeof(int));
    int index = 0;

    flattenMatrix(matrix, oldDimensions, oldDimensionCount, 0, tempArray, &index);

    // Create new matrix with new dimensions
    void *newMatrix = createNDMatrix(newDimensions, newDimensionCount, 0);

    // Fill the new matrix
    fillNDMatrix(newMatrix, newDimensions, newDimensionCount, 0, tempArray);

    // Copy the new matrix back to the original pointer
    void **matrixPtr = (void **)matrix;
    void **newMatrixPtr = (void **)newMatrix;
    for (int i = 0; i < *(newDimensions + 0); i++)
    {
        *(matrixPtr + i) = *(newMatrixPtr + i);
    }

    // Clean up
    free(tempArray);
    free(newMatrix);
}

int main()
{
    // Read dimension count
    int dimensionCount;
    scanf("%d", &dimensionCount);

    // Read dimensions
    int *dimensions = (int *)malloc(dimensionCount * sizeof(int));
    int totalElements = 1;
    for (int i = 0; i < dimensionCount; i++)
    {
        scanf("%d", dimensions + i);
        totalElements *= *(dimensions + i);
    }

    // Create matrices
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

    free(input);

    // Read and perform operations
    int operationCount;
    scanf("%d", &operationCount);

    for (int i = 0; i < operationCount; i++)
    {
        char operation;
        scanf("\n%c", &operation);
        if (operation == 'L')
        {
            // Linear operation
            int targetMatrix;
            double alpha, beta;
            scanf("%d %lf %lf", &targetMatrix, &alpha, &beta);
            void *result = createNDMatrix(dimensions, dimensionCount, 0);
            linearOperation(matrix1, matrix2, result, dimensions, dimensionCount, 0, alpha, beta);
            if (targetMatrix == 1)
            {
                freeNDMatrix(matrix1, dimensions, dimensionCount, 0);
                matrix1 = result;
            }
            else
            {
                freeNDMatrix(matrix2, dimensions, dimensionCount, 0);
                matrix2 = result;
            }
        }
        else if (operation == 'T')
        {
            // Transpose operation for both matrices
            transpose(&matrix1, &matrix2, dimensions, dimensionCount);
        }
        else if (operation == 'R')
        {
            // Reshape operation
            int newDimensionCount;
            scanf("%d", &newDimensionCount);
            int *newDimensions = (int *)malloc(newDimensionCount * sizeof(int));
            for (int j = 0; j < newDimensionCount; j++)
            {
                scanf("%d", newDimensions + j);
            }
            reshape(matrix1, dimensions, newDimensions, dimensionCount, newDimensionCount);
            reshape(matrix2, dimensions, newDimensions, dimensionCount, newDimensionCount);
            free(dimensions);
            dimensions = newDimensions;
            dimensionCount = newDimensionCount;
        }
    }

    // Allocate and initialize coordinates array
    int *coordinates = (int *)calloc(dimensionCount, sizeof(int));

    // Print results with coordinates
    printf("Matrix 1:\n");
    printNDMatrix(matrix1, dimensions, dimensionCount, 0, coordinates);
    printf("Matrix 2:\n");
    printNDMatrix(matrix2, dimensions, dimensionCount, 0, coordinates);

    // Free coordinates array
    free(coordinates);

    // Clean up
    freeNDMatrix(matrix1, dimensions, dimensionCount, 0);
    freeNDMatrix(matrix2, dimensions, dimensionCount, 0);
    free(dimensions);
    return 0;
}
