// week5_task2_struct_save_load.c
// Task 2: Save and load structured records from a file
// Week 5 – Files & Modular Programming
// TODO: Complete function implementations and file handling logic.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50

typedef struct {
  char name[MAX_NAME_LEN];
  int age;
  float gpa;
} Student;

// Function prototypes
void save_student(Student s, const char* filename);
Student load_student(const char* filename);

int main(void) {
  Student s1;
  strcpy(s1.name, "Paul");
  s1.age = 28;
  s1.gpa = 3.90f;

  const char* filename = "student.txt";

  // TODO: Call save_student() to save student data to file
  printf("Saving student to file...\n");
  save_student(s1, filename);

  // TODO: Call load_student() to read data back into a new struct
  printf("Loading student from file...\n");
  Student s2 = load_student(filename);

  // TODO: Print loaded data to confirm correctness
  printf("Loaded student: %s, %d, GPA %.2f\n", s2.name, s2.age, s2.gpa);

  return 0;
}

// TODO: Implement save_student()
// Open file for writing, check errors, write fields, then close file
void save_student(Student s, const char* filename) {
  FILE* fp = fopen(filename, "w");
  if (fp == NULL) {
    perror("Error opening file for writing");
    exit(EXIT_FAILURE);
  }

  // Simple text format: name age gpa (name cannot contain spaces)
  if (fprintf(fp, "%s %d %.2f\n", s.name, s.age, s.gpa) < 0) {
    perror("Error writing to file");
    fclose(fp);
    exit(EXIT_FAILURE);
  }

  if (fclose(fp) != 0) {
    perror("Error closing file after writing");
    exit(EXIT_FAILURE);
  }
}

// TODO: Implement load_student()
// Open file for reading, check errors, read fields, then close file
Student load_student(const char* filename) {
  Student s = (Student){0};

  FILE* fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Error opening file for reading");
    exit(EXIT_FAILURE);
  }

  if (fscanf(fp, "%49s %d %f", s.name, &s.age, &s.gpa) != 3) {
    fprintf(stderr, "Error: invalid file format while reading '%s'\n",
            filename);
    fclose(fp);
    exit(EXIT_FAILURE);
  }

  if (fclose(fp) != 0) {
    perror("Error closing file after reading");
    exit(EXIT_FAILURE);
  }

  return s;
}
