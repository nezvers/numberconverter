#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>

int shift_left = 0;
int shift_right = 0;

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
  if (length < 3){
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
  char bin[65] = {0};
  int i = 64;
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
  if (shift_left != 0){
    value = value << shift_left;
  }
  else if (shift_right != 0){
    value = value >> shift_right;
  }

  printf("int: %d\n", value);
  printf("hex: 0x%02x\n", value);
  print_binary(value);
}

void parse_integer_string(const char* input){
  printf("Input: %s\n", input);
  
  long long value;
  sscanf(input, "%d", &value);
  print_output(value);
}

void parse_binary_string(const char* input){
  printf("Input: %s\n", input);

  long long value = bin2int((char*)input);
  print_output(value);
}

void parse_hex_string(const char* input){
  printf("Input: %s\n", input);

  long long value = hex2int((char*)input);
  print_output(value);
}

int main(int argc, char* argv[]){
// int main(){
//   int argc = 4;
//   char* argv[] = {"", "0b10110", ">>", "1"};

  printf("\n");
  if (argc < 2){
    printf("Expecting a number: decimal (14), hex (0xFF1C), or binary (0b01101001)");
    return 0;
  }

  if (argc > 3){
    if (strncmp(argv[2], "<<", 2) == 0){
      sscanf(argv[3], "%d", &shift_left);
      if(shift_left < 1){
        printf("Incorrect shifting count: %s\n", argv[3]);
        return 0;
      }
    }
    else if(strncmp(argv[2], ">>", 2) == 0){
      sscanf(argv[3], "%d", &shift_right);
      if(shift_right < 1){
        printf("Incorrect shifting count: %s\n", argv[3]);
        return 0;
      }
    }
    else{
      printf("Unsupported opperation: %s\n", argv[2]);
      return 0;
    }
  }

  if (is_integer(argv[1])){
    parse_integer_string(argv[1]);
    return 0;
  }
  else if (is_binary(argv[1])){
    parse_binary_string(argv[1]);
    return 0;
  }
  else if (is_hex(argv[1])){
    parse_hex_string(argv[1]);
    return 0;
  }

  printf("Invalid number! Expecting a number: integer (14), hex (0xFF1C), or binary (0b01101001)");

  return 0;
}