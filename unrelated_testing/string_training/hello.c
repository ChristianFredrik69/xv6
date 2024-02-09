#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

// Concatenates two strings and returns the result

char *concatenate(char *str1, char *str2) {
    char *result = malloc(strlen(str1) + strlen(str2) + 1); // +1 for the null-terminator
    strcpy(result, str1);
    strcat(result, str2);
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 3) { // The program expects two arguments
        // Instructions on how to use the program correctly
        fprintf(stderr, "usage: cpu <string1> <string2>\n");
        exit(1); // exit code 1 to indicate an error
    }

    char *other = malloc(30);
    strcpy(other, "Original other string content");
    
    char *str1 = argv[1]; // Create a pointer to the first argument
    char *str2 = argv[2]; // Create a pointer to the second argument
    char *result = concatenate(str1, str2); // Point to the resulting string
    
    printf("%s\n", result); // Print the result
    printf("%s\n", other); // Print the result
    free(result); // Free the memory allocated for the result
    free(other); // Free the memory allocated for the result
    return 0; // exit code 0 to indicate success
}


