/*
 * Lab 3, Task 1
 * Paul Moses Thomas Aaran, 231ADB187
 *
 * Implement array algorithms:
 *   - find minimum value
 *   - find maximum value
 *   - calculate sum
 *   - calculate average
 *
 * Rules:
 *   - Write separate functions for each operation.
 *   - Work with int arrays.
 *   - Average should return a float.
 *
 * Example:
 *   int arr[] = {1, 2, 3, 4, 5};
 *   min = array_min(arr, 5); // 1
 *   max = array_max(arr, 5); // 5
 *   sum = array_sum(arr, 5); // 15
 *   avg = array_avg(arr, 5); // 3.0
 */

#include <stdio.h>
#include <limits.h>

// Function prototypes
int array_min(int arr[], int size);
int array_max(int arr[], int size);
int array_sum(int arr[], int size);
float array_avg(int arr[], int size);

int main(void) {
    int size;

    printf("Enter number of elements: ");
    scanf("%d", &size);

    int arr[size];  // variable length array (C99 feature)
    printf("Enter %d integers:\n", size);
    for (int i = 0; i < size; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Min: %d\n", array_min(arr, size));
    printf("Max: %d\n", array_max(arr, size));
    printf("Sum: %d\n", array_sum(arr, size));
    printf("Avg: %.2f\n", array_avg(arr, size));

    return 0;
}

// Returns the smallest element
int array_min(int arr[], int size) {
    int min = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

// Returns the largest element
int array_max(int arr[], int size) {
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// Returns the sum of elements
int array_sum(int arr[], int size) {
    int total = 0;
    for (int i = 0; i < size; i++) {
        total += arr[i];
    }
    return total;
}

// Returns the average as a float
float array_avg(int arr[], int size) {
    if (size == 0) return 0.0f;  // avoid division by zero
    return (float)array_sum(arr, size) / size;
}
