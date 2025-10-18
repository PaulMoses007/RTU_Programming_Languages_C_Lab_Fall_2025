// week5_task1_file_io.c
// Task 1: Read and write data from text files
// Week 5 – Files & Modular Programming
// Filled according to the outline; includes BONUS features.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  FILE* fp;
  char filename[100] = "data.txt";
  char line[256];
  size_t lineCount = 0;

  printf("Enter filename [default: %s]: ", filename);
  if (fgets(filename, sizeof(filename), stdin) != NULL) {
    filename[strcspn(filename, "\n")] = '\0';  // strip newline
    if (filename[0] == '\0') {
      strcpy(filename, "data.txt");
    }
  }

  // TODO: 1. Open file for writing (mode = "w")
  fp = fopen(filename, "w");
  // TODO: 2. Check if file opened successfully
  if (fp == NULL) {
    perror("Error opening file for writing");
    return EXIT_FAILURE;
  }

  printf("Writing lines to %s...\n", filename);

  // TODO: 3. Write 2–3 lines of text to the file using fprintf()
  if (fprintf(fp, "Hello, file I/O in C!\n") < 0 ||
      fprintf(fp, "This is another line.\n") < 0 ||
      fprintf(fp, "One more line for testing.\n") < 0) {
    perror("Error writing to file");
    fclose(fp);
    return EXIT_FAILURE;
  }

  // TODO: 4. Close the file
  if (fclose(fp) != 0) {
    perror("Error closing file after writing");
    return EXIT_FAILURE;
  }

  // TODO: 5. Open file again for reading (mode = "r")
  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Error opening file for reading");
    return EXIT_FAILURE;
  }

  printf("Reading contents:\n");

  // TODO: 6. Use fgets() in a loop to read and print each line to the console
  while (fgets(line, sizeof(line), fp) != NULL) {
    fputs(line, stdout);
    lineCount++;  // BONUS: count number of lines read
  }

  if (ferror(fp)) {
    perror("Error while reading the file");
    fclose(fp);
    return EXIT_FAILURE;
  }

  // TODO: 7. Close the file
  if (fclose(fp) != 0) {
    perror("Error closing file after reading");
    return EXIT_FAILURE;
  }

  // BONUS: report number of lines read
  printf("Total lines read: %zu\n", lineCount);

  return EXIT_SUCCESS;
}

