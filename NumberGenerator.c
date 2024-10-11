#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    // Seed the random number generator
    srand((unsigned int)time(NULL));

    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        int random_number = rand() % 1000;
        printf("%d ", random_number);
    }
    printf("\n");
    return 0;
}