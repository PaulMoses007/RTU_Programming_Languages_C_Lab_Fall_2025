/*
 * Lab 3, Task 3
 * Thomas Aaran Paul Moses, 231ADB187
 *
 * Implement basic string handling functions.
 * Write your own versions of:
 *   - my_strlen (finds string length)
 *   - my_strcpy (copies string from src to dest)
 *
 * Rules:
 *   - Do not use <string.h> functions for strlen/strcpy.
 *   - Use loops and manual pointer/array access.
 *
 * Example:
 *   char s[] = "hello";
 *   int len = my_strlen(s);   // should return 5
 *
 *   char buffer[100];
 *   my_strcpy(buffer, s);     // buffer now contains "hello"
 */

#include <stdio.h>

// Function prototypes
int my_strlen(const char *str);
void my_strcpy(char *dest, const char *src);

int main(void) {
    char input[100];
    char copy[100];

    printf("Enter a string: ");
    fgets(input, sizeof(input), stdin);

    // Remove newline if present (since fgets keeps '\n')
    int len = my_strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;  // adjust length
    }

    printf("Length: %d\n", len);

    my_strcpy(copy, input);
    printf("Copy: %s\n", copy);

    return 0;
}

// Count characters until '\0'
int my_strlen(const char *str) {
    int count = 0;
    while (str[count] != '\0') {
        count++;
    }
    return count;
}

// Copy characters until '\0'
void my_strcpy(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0'; // add null terminator
}