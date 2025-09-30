/*
 * Lab 3, Task 2
 * Paul Moses Thomas Aaran, 231ADB187
 *
 * Practice using pointers as function parameters.
 * Implement:
 *   - swap (exchange values of two ints)
 *   - modify_value (multiply given int by 2)
 *
 * Rules:
 *   - Use pointers to modify variables in the caller.
 *   - Demonstrate changes in main.
 *
 * Example:
 *   int a = 5, b = 10;
 *   swap(&a, &b);   // now a = 10, b = 5
 *
 *   modify_value(&a); // now a = 20
 */

#include <stdio.h>

// Function prototypes
void swap(int *x, int *y);
void modify_value(int *x);

int main(void) {
    int a, b;

    // User input
    printf("Enter two integers (a and b): ");
    scanf("%d %d", &a, &b);

    printf("Before swap: a=%d, b=%d\n", a, b);

    swap(&a, &b);
    printf("After swap: a=%d, b=%d\n", a, b);

    modify_value(&a);
    printf("After modify_value (a * 2): a=%d\n", a);

    return 0;
}

// Swap values of two integers using pointers
void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

// Multiply the value of an integer by 2
void modify_value(int *x) {
    *x = *x * 2;
}

