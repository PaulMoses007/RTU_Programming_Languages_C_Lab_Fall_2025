/*
 * week4_3_struct_database.c
 * Author: Thomas Aaran Paul Moses
 * Student ID: 231ADB187
 * Description:
 *   Simple in-memory "database" using an array of structs.
 *   Dynamically allocates memory for multiple Student records,
 *   inputs, displays, and frees the data.
 *   Properly handles full names with spaces and avoids scanf issues.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    char name[50];
    int id;
    float grade;
};

int main(void) {
    int n;
    struct Student *students = NULL;
    char line[100];

    printf("Enter number of students: ");
    if (scanf("%d", &n) != 1 || n <= 0) return 1;
    getchar(); // consume newline

    students = (struct Student *)malloc(n * sizeof(struct Student));
    if (!students) return 1;

    for (int i = 0; i < n; i++) {
        printf("Enter data for student %d (Name ID Grade): ", i + 1);
        if (!fgets(line, sizeof(line), stdin)) {
            printf("Input error.\n");
            free(students);
            return 1;
        }

        // Remove newline at the end
        line[strcspn(line, "\n")] = 0;

        // Find last space (before grade)
        char *lastSpace = strrchr(line, ' ');
        if (!lastSpace) { i--; continue; }
        students[i].grade = atof(lastSpace + 1);
        *lastSpace = 0; // terminate string to isolate rest

        // Find second-to-last space (before ID)
        char *secondLastSpace = strrchr(line, ' ');
        if (!secondLastSpace) { i--; continue; }
        students[i].id = atoi(secondLastSpace + 1);
        *secondLastSpace = 0; // terminate string to isolate name

        // Rest is name
        strncpy(students[i].name, line, sizeof(students[i].name));
        students[i].name[sizeof(students[i].name)-1] = 0;
    }

    printf("\nID\tName\t\tGrade\n");
    printf("----------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%-15s\t%.1f\n", students[i].id, students[i].name, students[i].grade);
    }

    free(students);
    return 0;
}

