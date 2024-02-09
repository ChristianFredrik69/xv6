#include <stdio.h>
#include <stdlib.h>
#include "common.h"

int b() {
    // Simulating an error condition
    printf("Error occurred in b()\n");
    // exit(1);  // This will terminate the entire program

    // Alternatively, if you comment out the above line and uncomment the next line,
    // it will return control to a() instead of terminating the program
    return 1;  
}

int a() {
    printf("In a(), calling b()\n");
    int result = b();
    printf("b() returned %d\n", result);  // This line will not be executed if b() calls exit(1)
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: cpu <string>\n");
        exit(1);
    }
    char *str = argv[1];

    int aResult = a();
    printf("a() returned %d\n", aResult);  // This line will not be executed if b() calls exit(1)

    while (1) {
        printf("%s\n", str);
        Spin(1);
    }
    return 0;
}
