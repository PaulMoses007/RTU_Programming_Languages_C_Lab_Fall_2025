// week5_task3_student_management_system.c
// Task 3: Mini-project – Student management system with file persistence
// Week 5 – Files & Modular Programming
// TODO: Implement functions to load, save, add, and list students.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50
#define DATA_FILE "students.txt"

typedef struct {
  char name[NAME_LEN];
  int id;
  float gpa;
} Student;

// Function prototypes
int load_students(Student arr[]);
void save_students(Student arr[], int count);
void add_student(Student arr[], int* count);
void list_students(Student arr[], int count);

int main(void) {
  Student students[MAX_STUDENTS];
  int count = 0;
  int choice = 0;

  // TODO: Load existing data from file using load_students()
  count = load_students(students);

  do {
    printf("\n=== Student Management System ===\n");
    printf("1. List students\n");
    printf("2. Add student\n");
    printf("3. Save and Exit\n");

    char buf[32];
    printf("Select an option: ");
    if (!fgets(buf, sizeof(buf), stdin)) {
      save_students(students, count);
      printf("\nInput closed. Data saved. Goodbye!\n");
      break;
    }
    if (sscanf(buf, "%d", &choice) != 1) {
      printf("Invalid input. Please enter 1, 2, or 3.\n");
      continue;
    }

    switch (choice) {
      case 1:
        // TODO: Call list_students()
        list_students(students, count);
        break;
      case 2:
        // TODO: Call add_student()
        add_student(students, &count);
        break;
      case 3:
        // TODO: Call save_students() and exit loop
        save_students(students, count);
        printf("Data saved. Goodbye!\n");
        break;
      default:
        printf("Invalid option. Try again.\n");
    }
  } while (choice != 3);

  return 0;
}

// TODO: Implement load_students()
// Open DATA_FILE, read records until EOF, return number of records loaded
int load_students(Student arr[]) {
  FILE* fp = fopen(DATA_FILE, "r");
  if (fp == NULL) {
    return 0;
  }

  int count = 0;
  while (count < MAX_STUDENTS) {
    Student s;
    int scanned = fscanf(fp, " %49s %d %f", s.name, &s.id, &s.gpa);
    if (scanned == 3) {
      arr[count++] = s;
    } else if (scanned == EOF) {
      break;
    } else {
      int ch;
      while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
      }
    }
  }

  fclose(fp);
  return count;
}

// TODO: Implement save_students()
// Write all students to DATA_FILE
void save_students(Student arr[], int count) {
  FILE* fp = fopen(DATA_FILE, "w");
  if (fp == NULL) {
    perror("Error opening file for writing");
    return;
  }

  for (int i = 0; i < count; i++) {
    if (fprintf(fp, "%s %d %.2f\n", arr[i].name, arr[i].id, arr[i].gpa) < 0) {
      perror("Error writing to file");
    }
  }

  if (fclose(fp) != 0) {
    perror("Error closing file after writing");
  }
}

// TODO: Implement add_student()
// Read input from user and append to array
void add_student(Student arr[], int* count) {
  if (*count >= MAX_STUDENTS) {
    printf("Cannot add more students (max %d).\n", MAX_STUDENTS);
    return;
  }

  Student s;
  char buffer[256];

  printf("Enter name: ");
  while (1) {
    if (!fgets(buffer, sizeof(buffer), stdin)) {
      printf("Input error. Student not added.\n");
      return;
    }
    if (sscanf(buffer, "%49s", s.name) == 1) break;
    printf("Invalid name. Enter name: ");
  }

  printf("Enter ID: ");
  while (1) {
    if (!fgets(buffer, sizeof(buffer), stdin)) {
      printf("Input error. Student not added.\n");
      return;
    }
    if (sscanf(buffer, "%d", &s.id) == 1) break;
    printf("Invalid ID. Enter ID: ");
  }

  printf("Enter GPA: ");
  while (1) {
    if (!fgets(buffer, sizeof(buffer), stdin)) {
      printf("Input error. Student not added.\n");
      return;
    }
    if (sscanf(buffer, "%f", &s.gpa) == 1) break;
    printf("Invalid GPA. Enter GPA: ");
  }

  arr[*count] = s;
  (*count)++;
  printf("Student added successfully!\n");
}

// TODO: Implement list_students()
// Print all students in readable format
void list_students(Student arr[], int count) {
  if (count == 0) {
    printf("No students found.\n");
    return;
  }

  printf("\n-- Students (%d) --\n", count);
  for (int i = 0; i < count; i++) {
    printf("%d. %s (ID: %d) GPA: %.2f\n", i + 1, arr[i].name, arr[i].id,
           arr[i].gpa);
  }
}

