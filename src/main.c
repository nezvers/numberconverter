#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>

enum TokenType {INTEGER, BINARY, HEX, SHIFT_LEFT, SHIFT_RIGHT, AND, OR, XOR, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO};

typedef struct{
  char* str;
  uint16_t type; // TokenType
  long long value;
}Token;

#define MAX_TOKENS 3
Token tokens[MAX_TOKENS] = {};
uint16_t token_count = 0;

bool get_tokens(const char* input){
  size_t length = strlen(input);
  
  char* str = strtok((char*)input, " \n");
  while (str != NULL){
    tokens[token_count].str = str;
    token_count++;
    str = strtok(NULL, " \n");
  }
  return true;
}


bool is_integer(const char* input){
  int length = strlen(input);
  int i = 0;
  if(input[0] == '-' || input[0] == '+'){
    i++;
  }
  if(input[i] == '0'){
    return false;
  }

  for ( ; i < length; i++){
    if (input[i] < '0' || input[i] > '9'){
      return false;
    }
  }
  return true;
}

bool is_binary(const char* input){
  int length = strlen(input);
  if (length < 3 || length > 66){
    return false;
  }
  if ((strncmp(input, "0b", 2) != 0) && (strncmp(input, "0B", 2) != 0)){
    return false;
  }
  for(int i = 2; i < length; i++){
    if (input[i] != '0' && input[i] != '1'){
      return false;
    }
  }
  return true;
}

bool is_hex(const char* input){
  int length = strlen(input);
  if (length < 3){
    return false;
  }
  if ((strncmp(input, "0x", 2) != 0) && (strncmp(input, "0X", 2) != 0)){
    return false;
  }
  for(int i = 2; i < length; i++){
    char c = input[i];
    if (!isalnum(c)){
      return false;
    }
    if (c >= '0' && c <= '9'){
      continue;
    }
    c = toupper(c);
    if (c >= 'A' && c <= 'F'){
      continue;
    }
    return false;
  }
  return true;
}

long long hex2int(char *input) {
  // https://stackoverflow.com/a/39052987
  // Skip 0x
  input += 2;
  long long value = 0;
  while (*input) {
      // get current character then increment
      char c = *input++; 
      // transform hex character to the 4bit equivalent number, using the ascii table indexes
      if (c >= '0' && c <= '9') c = c - '0';
      else if (c >= 'a' && c <='f') c = c - 'a' + 10;
      else if (c >= 'A' && c <='F') c = c - 'A' + 10;    
      // shift 4 to make space for new digit, and add the 4 bits of the new digit 
      value = (value << 4) | (c & 0xF);
  }
  return value;
}

long long bin2int(char *input){
  // Skip 0b
  input += 2;
  long long value = 0;
  while (*input){
    // get current character then increment
    uint8_t bit = *input++ == '1' ? 1 : 0;
    value = (value << 1) | bit;
  }

  return value;
}

void print_binary(long long value){
  char bin[67] = {0};
  int i = 66;
  while(value != 0){
    i--;
    bin[i] = value & 1 ? '1' : '0';
    value = value >> 1;
  }
  bin[--i] = 'b';
  bin[--i] = '0';
  printf("bin: %s\n", &bin[i]);
}

void print_output(long long value){
  printf("int: %d\n", value);
  printf("hex: 0x%02x\n", value);
  print_binary(value);
}

long long parse_integer_string(const char* input){
  long long value;
  sscanf(input, "%d", &value);
  return value;
}

long long parse_binary_string(const char* input){
  return bin2int((char*)input);
}

long long parse_hex_string(const char* input){
  return hex2int((char*)input);
}

bool process_value_token(Token* token){
  if (is_integer(token->str)){
    token->value = parse_integer_string(token->str);
    token->type = INTEGER;
  }
  else if (is_binary(token->str)){
    token->value = parse_binary_string(token->str);
    token->type = BINARY;
  }
  else if (is_hex(token->str)){
    token->value = parse_hex_string(token->str);
    token->type = HEX;
  }
  else{
    printf("Number not recognized: %s\n", token->str);
    return false;
  }
  return true;
}

bool process_operator_token(Token* token){
  if (strncmp(token->str, "<<", 2) == 0){
    token->type = SHIFT_LEFT;
  }
  else if (strncmp(token->str, ">>", 2) == 0){
    token->type = SHIFT_RIGHT;
  }
  else if (strncmp(token->str, "|", 1) == 0){
    token->type = OR;
  }
  else if (strncmp(token->str, "&", 1) == 0){
    token->type = AND;
  }
  else if (strncmp(token->str, "^", 1) == 0){
    token->type = XOR;
  }
  else if (strncmp(token->str, "%", 1) == 0){
    token->type = MODULO;
  }
  else if (strncmp(token->str, "+", 1) == 0){
    token->type = PLUS;
  }
  else if (strncmp(token->str, "-", 1) == 0){
    token->type = MINUS;
  }
  else if (strncmp(token->str, "*", 1) == 0){
    token->type = MULTIPLY;
  }
  else if (strncmp(token->str, "/", 1) == 0){
    token->type = DIVIDE;
  }
  else{
    printf("Operator not recognized: %s\n", token->str);
    printf("Supported only +, -, *, /, %%, &, |, ^, <<, >>\n");
    return false;
  }
  return true;
}

long long get_result(){
  if (token_count == 1){
    return tokens[0].value;
  }

  switch (tokens[1].type){
    case SHIFT_LEFT:
      return tokens[0].value << tokens[2].value;
    case SHIFT_RIGHT:
      return tokens[0].value >> tokens[2].value;
    case AND:
      return tokens[0].value & tokens[2].value;
    case OR:
      return tokens[0].value | tokens[2].value;
    case XOR:
      return tokens[0].value ^ tokens[2].value;
    case PLUS:
      return tokens[0].value + tokens[2].value;
    case MINUS:
      return tokens[0].value - tokens[2].value;
    case MULTIPLY:
      return tokens[0].value * tokens[2].value;
    case DIVIDE:
      return tokens[0].value / tokens[2].value;
    case MODULO:
      return tokens[0].value % tokens[2].value;
  }
}

int main(int argc, char* argv[]){
  printf("\n");
  if (argc < 2){
    printf("No provided input.");
    return 0;
  }

  if (!get_tokens(argv[1])){
    return 1;
  }
  if (token_count != 1 && token_count != 3){
    printf("Enter only either single number or [number operator number]\n");
    return 1;
  }
  if (!process_value_token(&tokens[0])){
    return 1;
  }
  if (token_count > 2 && !process_operator_token(&tokens[1])){
    return 1;
  }
  if (token_count > 2 && !process_value_token(&tokens[2])){
    return 1;
  }
  
  long long value = get_result();
  print_output(value);

  return 0;
}