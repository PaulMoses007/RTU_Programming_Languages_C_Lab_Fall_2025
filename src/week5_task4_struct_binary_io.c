// week5_task4_struct_binary_io.c
// Optional Bonus Task 4: Binary File Version (Advanced)
// Save and load a Student struct in binary using fwrite/fread,
// and compare text vs binary file sizes.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define TEXT_FILE "student.txt"
#define BIN_FILE  "student.bin"

typedef struct {
    char name[MAX_NAME_LEN];
    int age;
    float gpa;
} Student;

// Prototypes
void save_student_text(Student s, const char *filename);
void save_student_bin(Student s, const char *filename);
Student load_student_bin(const char *filename);
long file_size_bytes(const char *filename);

int main(void) {
    Student s1;
    strcpy(s1.name, "Paul");
    s1.age = 28;
    s1.gpa = 3.90f;

    // Also produce a text version so we can compare sizes
    save_student_text(s1, TEXT_FILE);

    printf("Saving student in binary format...\n");
    save_student_bin(s1, BIN_FILE);

    printf("Loading student from binary file...\n");
    Student s2 = load_student_bin(BIN_FILE);

    printf("Loaded student: %s, %d, GPA %.2f\n", s2.name, s2.age, s2.gpa);

    long bin_sz = file_size_bytes(BIN_FILE);
    long txt_sz = file_size_bytes(TEXT_FILE);
    if (bin_sz >= 0) {
        printf("Binary file size: %ld bytes\n", bin_sz);
    } else {
        printf("Binary file size: (file not found)\n");
    }
    if (txt_sz >= 0) {
        printf("Text file size: %ld bytes\n", txt_sz);
    } else {
        printf("Text file size: (file not found)\n");
    }

    return EXIT_SUCCESS;
}

void save_student_text(Student s, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Error opening text file for writing");
        exit(EXIT_FAILURE);
    }
    // Simple text format: name age gpa (name as a single token)
    if (fprintf(fp, "%s %d %.2f\n", s.name, s.age, s.gpa) < 0) {
        perror("Error writing text file");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    if (fclose(fp) != 0) {
        perror("Error closing text file");
        exit(EXIT_FAILURE);
    }
}

void save_student_bin(Student s, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Error opening binary file for writing");
        exit(EXIT_FAILURE);
    }
    if (fwrite(&s, sizeof(Student), 1, fp) != 1) {
        perror("Error writing binary file");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    if (fclose(fp) != 0) {
        perror("Error closing binary file after writing");
        exit(EXIT_FAILURE);
    }
}

Student load_student_bin(const char *filename) {
    Student s = (Student){0};
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Error opening binary file for reading");
        exit(EXIT_FAILURE);
    }
    if (fread(&s, sizeof(Student), 1, fp) != 1) {
        perror("Error reading binary file");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    if (fclose(fp) != 0) {
        perror("Error closing binary file after reading");
        exit(EXIT_FAILURE);
    }
    return s;
}

long file_size_bytes(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return -1;
    if (fseek(fp, 0L, SEEK_END) != 0) {
        fclose(fp);
        return -1;
    }
    long size = ftell(fp);
    fclose(fp);
    return size;
}

