// Thomas Aaran Paul Moses 231ADB187
// https://github.com/PaulMoses007/RTU_Programming_Languages_C_Lab_Fall_2025.git
// Compile with: gcc -o calc calc.c -lm

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// PART 1: BASIC DEFINITIONS

// Different types of tokens we can find
typedef enum {
  TOK_NUMBER,  // Like 42, 3.14
  TOK_PLUS,    // +
  TOK_MINUS,   // -
  TOK_STAR,    // *
  TOK_SLASH,   // /
  TOK_POW,     // **
  TOK_LPAREN,  // (
  TOK_RPAREN,  // )
  TOK_EOF,     // End of file
  TOK_ERROR    // Something wrong
} TokenType;

// A token is like a word in our language
typedef struct {
  TokenType type;  // What kind of token is this?
  double value;    // The number value (if it's a number)
  int start_pos;   // Where it starts in the file (for errors)
} Token;

// This keeps track of where we are in parsing
typedef struct {
  const char* input;   // The text we're reading
  int position;        // Current position in the text
  int length;          // Total length of text
  int has_error;       // Did we find an error?
  int error_position;  // Where the error is
} ParserState;

// PART 2: FUNCTION DECLARATIONS

// Forward declarations - tell compiler these functions exist
double parse_expression(ParserState* state, Token* token);
double parse_term(ParserState* state, Token* token);
double parse_power(ParserState* state, Token* token);
double parse_primary(ParserState* state, Token* token);

// PART 3: TOKENIZER (READING "WORDS")

// Skip spaces, tabs, newlines
void skip_spaces(ParserState* state) {
  while (state->position < state->length &&
         isspace(state->input[state->position])) {
    state->position++;
  }
}

// Read a number from the input
Token read_number(ParserState* state) {
  Token token;
  token.type = TOK_NUMBER;
  token.start_pos = state->position + 1;  // +1 for 1-based counting

  // Find where the number ends
  int start = state->position;
  while (state->position < state->length &&
         (isdigit(state->input[state->position]) ||
          state->input[state->position] == '.' ||
          state->input[state->position] == 'e' ||
          state->input[state->position] == 'E' ||
          state->input[state->position] == '+' ||
          state->input[state->position] == '-')) {
    state->position++;
  }

  // Extract the number string
  int length = state->position - start;
  char* num_str = malloc(length + 1);
  strncpy(num_str, state->input + start, length);
  num_str[length] = '\0';

  // Convert to number
  token.value = atof(num_str);
  free(num_str);

  return token;
}

// Get the next token from input
Token get_next_token(ParserState* state) {
  skip_spaces(state);

  // Check if we reached end
  if (state->position >= state->length) {
    Token token;
    token.type = TOK_EOF;
    token.start_pos = state->position + 1;
    return token;
  }

  char current_char = state->input[state->position];
  int current_pos = state->position + 1;  // 1-based position

  // Check what type of token this is
  switch (current_char) {
    case '+':
      state->position++;
      return (Token){TOK_PLUS, 0.0, current_pos};

    case '-':
      state->position++;
      return (Token){TOK_MINUS, 0.0, current_pos};

    case '*':
      // Check if it's ** (power operator)
      if (state->position + 1 < state->length &&
          state->input[state->position + 1] == '*') {
        state->position += 2;
        return (Token){TOK_POW, 0.0, current_pos};
      }
      state->position++;
      return (Token){TOK_STAR, 0.0, current_pos};

    case '/':
      state->position++;
      return (Token){TOK_SLASH, 0.0, current_pos};

    case '(':
      state->position++;
      return (Token){TOK_LPAREN, 0.0, current_pos};

    case ')':
      state->position++;
      return (Token){TOK_RPAREN, 0.0, current_pos};

    default:
      if (isdigit(current_char) || current_char == '.') {
        return read_number(state);
      } else {
        // Invalid character
        state->position++;
        Token token;
        token.type = TOK_ERROR;
        token.start_pos = current_pos;
        return token;
      }
  }
}

// PART 4: ERROR HANDLING

void report_error(ParserState* state, int position, const char* message) {
  if (!state->has_error) {
    state->has_error = 1;
    state->error_position = position;
    printf("Error at position %d: %s\n", position, message);
  }
}

// PART 5: PARSER (UNDERSTANDING THE MEANING)

// Handle numbers and things in parentheses
double parse_primary(ParserState* state, Token* token) {
  if (token->type == TOK_NUMBER) {
    // It's a number like 42 or 3.14
    double value = token->value;
    *token = get_next_token(state);  // Get next token after number
    return value;
  } else if (token->type == TOK_LPAREN) {
    // It's something in parentheses like (2+3)
    *token = get_next_token(state);  // Move past the (

    // Parse what's inside the parentheses
    double result = parse_expression(state, token);

    // Check for closing )
    if (token->type != TOK_RPAREN) {
      report_error(state, token->start_pos, "Expected ')'");
      return 0.0;
    }

    *token = get_next_token(state);  // Move past the )
    return result;
  } else {
    report_error(state, token->start_pos, "Expected number or '('");
    return 0.0;
  }
}

// Handle power operations (2**3)
double parse_power(ParserState* state, Token* token) {
  double left = parse_primary(state, token);

  if (state->has_error) return 0.0;

  // Handle multiple ** operators (right-associative: 2**3**2 = 2**(3**2))
  while (token->type == TOK_POW) {
    int op_position = token->start_pos;
    *token = get_next_token(state);

    double right = parse_power(
        state, token);  // Note: recursive call for right-associativity

    if (state->has_error) return 0.0;

    // Calculate power
    left = pow(left, right);
  }

  return left;
}

// Handle multiplication and division
double parse_term(ParserState* state, Token* token) {
  double result = parse_power(state, token);

  if (state->has_error) return 0.0;

  // Handle multiple * and / operators
  while (token->type == TOK_STAR || token->type == TOK_SLASH) {
    TokenType operation = token->type;
    int op_position = token->start_pos;

    *token = get_next_token(state);

    double right_value = parse_power(state, token);

    if (state->has_error) return 0.0;

    // Perform the operation
    if (operation == TOK_STAR) {
      result = result * right_value;
    } else {  // Division
      if (right_value == 0.0) {
        report_error(state, op_position, "Division by zero");
        return 0.0;
      }
      result = result / right_value;
    }
  }

  return result;
}

// Handle addition and subtraction (this is the main entry point)
double parse_expression(ParserState* state, Token* token) {
  double result = parse_term(state, token);

  if (state->has_error) return 0.0;

  // Handle multiple + and - operators
  while (token->type == TOK_PLUS || token->type == TOK_MINUS) {
    TokenType operation = token->type;

    *token = get_next_token(state);

    double right_value = parse_term(state, token);

    if (state->has_error) return 0.0;

    // Perform the operation
    if (operation == TOK_PLUS) {
      result = result + right_value;
    } else {
      result = result - right_value;
    }
  }

  return result;
}

// PART 6: FILE HANDLING

// Read entire file into memory
char* read_whole_file(const char* filename, int* file_length) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    printf("Cannot open file: %s\n", filename);
    return NULL;
  }

  // Find file size
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Allocate memory and read file
  char* content = malloc(size + 1);
  fread(content, 1, size, file);
  content[size] = '\0';  // Null-terminate the string

  fclose(file);

  *file_length = size;
  return content;
}

// Create output filename according to requirements
void make_output_filename(const char* input_file, char* output_path) {
  // Find the base name (after last /)
  const char* base_name = input_file;
  const char* last_slash = strrchr(input_file, '/');
  if (last_slash) {
    base_name = last_slash + 1;
  }

  // Remove .txt extension if present
  char name_without_ext[100];
  strcpy(name_without_ext, base_name);
  char* dot = strstr(name_without_ext, ".txt");
  if (dot) {
    *dot = '\0';  // Cut off at .txt
  }

  // Create output filename: task1_Thomas_Aaran_231ADB187.txt
  sprintf(output_path, "%s_Thomas_Aaran_231ADB187.txt", name_without_ext);
}

// Process one input file
void process_file(const char* input_filename) {
  printf("Processing: %s\n", input_filename);

  // Read the input file
  int file_length;
  char* file_content = read_whole_file(input_filename, &file_length);
  if (!file_content) {
    return;
  }

  // Initialize our parser state
  ParserState state;
  state.input = file_content;
  state.position = 0;
  state.length = file_length;
  state.has_error = 0;
  state.error_position = 0;

  // Get first token and start parsing
  Token first_token = get_next_token(&state);
  double result = parse_expression(&state, &first_token);

  // Check if there are extra tokens at the end
  if (!state.has_error && first_token.type != TOK_EOF) {
    report_error(&state, first_token.start_pos,
                 "Extra characters after expression");
  }

  // Create output file
  char output_filename[200];
  make_output_filename(input_filename, output_filename);

  FILE* output_file = fopen(output_filename, "w");
  if (output_file) {
    if (state.has_error) {
      // Write error position
      fprintf(output_file, "ERROR:%d\n", state.error_position);
      printf("  -> ERROR at position %d\n", state.error_position);
    } else {
      // Write result (as integer if whole number, otherwise as float)
      if (fabs(result - round(result)) < 0.000001) {
        fprintf(output_file, "%.0f\n", result);
        printf("  -> Result: %.0f\n", result);
      } else {
        fprintf(output_file, "%.15g\n", result);
        printf("  -> Result: %.15g\n", result);
      }
    }
    fclose(output_file);
  }

  free(file_content);
}

// PART 7: MAIN PROGRAM

void show_help() {
  printf("Arithmetic Expression Parser\n");
  printf("Usage: calc input.txt\n");
  printf("Supported operations: + - * / ** ( )\n");
  printf("Examples: 2+3, (2+3)*4, 2**3**2\n");
}

int main(int argc, char* argv[]) {
  printf("=== Beginner Arithmetic Parser ===\n");
  printf("By Thomas Aaran Paul Moses (231ADB187)\n\n");

  if (argc != 2) {
    show_help();
    return 1;
  }

  if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
    show_help();
    return 0;
  }

  process_file(argv[1]);

  printf("\nDone! Check the output file.\n");
  return 0;
}
