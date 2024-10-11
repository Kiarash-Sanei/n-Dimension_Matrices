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
void printNDMatrix(void *matrix, int *dimensions, int dimensionCount, int currentDimension)
{
    if (currentDimension == dimensionCount - 1)
    {
        // Base case: print the elements of the innermost dimension
        for (int i = 0; i < *(dimensions + currentDimension); i++)
        {
            printf("%d ", *((int *)matrix + i));
        }
        return;
    }

    // Recursively print sub-matrices
    for (int i = 0; i < *(dimensions + currentDimension); i++)
    {
        printNDMatrix(*(((void **)matrix) + i), dimensions, dimensionCount, currentDimension + 1);
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

// Function to transpose two dimensions of a matrix
void transpose(void *matrix, int *dimensions, int dimensionCount, int dim1, int dim2)
{
    // Swap the dimensions using XOR swap
    *(dimensions + dim1) ^= *(dimensions + dim2);
    *(dimensions + dim2) ^= *(dimensions + dim1);
    *(dimensions + dim1) ^= *(dimensions + dim2);

    // Create a new matrix with swapped dimensions
    void *newMatrix = createNDMatrix(dimensions, dimensionCount, 0);

    // Helper function to copy elements with transposed dimensions
    void copyTransposed(void *src, void *dest, int *dims, int count, int current, int d1, int d2, int *indices)
    {
        if (current == count)
        {
            // Base case: copy the element
            int *srcElem = (int *)src;
            int *destElem = (int *)dest;
            for (int i = 0; i < count; i++)
            {
                if (i == d1)
                    srcElem = *(((void **)srcElem) + *(indices + d2));
                else if (i == d2)
                    srcElem = *(((void **)srcElem) + *(indices + d1));
                else
                    srcElem = *(((void **)srcElem) + *(indices + i));

                if (i == d1)
                    destElem = *(((void **)destElem) + *(indices + d2));
                else if (i == d2)
                    destElem = *(((void **)destElem) + *(indices + d1));
                else
                    destElem = *(((void **)destElem) + *(indices + i));
            }
            *destElem = *srcElem;
            return;
        }

        // Recursively copy elements
        for (*(indices + current) = 0; *(indices + current) < *(dims + current); (*(indices + current))++)
        {
            copyTransposed(src, dest, dims, count, current + 1, d1, d2, indices);
        }
    }

    int *indices = (int *)calloc(dimensionCount, sizeof(int));
    copyTransposed(matrix, newMatrix, dimensions, dimensionCount, 0, dim1, dim2, indices);
    free(indices);

    // Copy the transposed matrix back to the original
    memcpy(matrix, newMatrix, sizeof(void *) * *(dimensions + 0));
    freeNDMatrix(newMatrix, dimensions, dimensionCount, 0);
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

    // Helper function to flatten the old matrix
    void flattenMatrix(void *m, int *dims, int count, int current)
    {
        if (current == count - 1)
        {
            // Base case: copy elements to tempArray
            for (int i = 0; i < *(dims + current); i++)
            {
                *(tempArray + index) = *((int *)m + i);
                index++;
            }
            return;
        }

        // Recursively flatten sub-matrices
        for (int i = 0; i < *(dims + current); i++)
        {
            flattenMatrix(*(((void **)m) + i), dims, count, current + 1);
        }
    }

    flattenMatrix(matrix, oldDimensions, oldDimensionCount, 0);

    // Create new matrix with new dimensions
    void *newMatrix = createNDMatrix(newDimensions, newDimensionCount, 0);

    // Fill the new matrix
    fillNDMatrix(newMatrix, newDimensions, newDimensionCount, 0, tempArray);

    // Copy the new matrix back to the original pointer
    memcpy(matrix, newMatrix, sizeof(void *) * *(newDimensions + 0));

    // Clean up
    free(tempArray);
    freeNDMatrix(newMatrix, newDimensions, newDimensionCount, 0);
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
        scanf("%c ", &operation);

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
            // Transpose operation
            int targetMatrix, dim1, dim2;
            scanf("%d %d %d", &targetMatrix, &dim1, &dim2);
            dim1--;
            dim2--; // Adjust for 0-based indexing
            if (targetMatrix == 1)
            {
                transpose(matrix1, dimensions, dimensionCount, dim1, dim2);
            }
            else
            {
                transpose(matrix2, dimensions, dimensionCount, dim1, dim2);
            }
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
        printNDMatrix(matrix1, dimensions, dimensionCount, 0);
        printf("\n");
        printNDMatrix(matrix2, dimensions, dimensionCount, 0);
        printf("\n");
    }

    // Print results
    printNDMatrix(matrix1, dimensions, dimensionCount, 0);
    printf("\n");
    printNDMatrix(matrix2, dimensions, dimensionCount, 0);
    printf("\n");

    // Clean up
    freeNDMatrix(matrix1, dimensions, dimensionCount, 0);
    freeNDMatrix(matrix2, dimensions, dimensionCount, 0);
    free(dimensions);
    return 0;
}