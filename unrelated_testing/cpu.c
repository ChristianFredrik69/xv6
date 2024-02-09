#include <stdio.h> // contains printf() and fprintf()
#include <stdlib.h> // contains exit()
#include "common.h" // double quote means look in current directory

/*
This main declaration is similar to the
public static void main(String[] args) in Java
It is just how the main function must be declared in C

argc is the number of arguments passed to the program
It is always at least 1, because the program name counts as an argument

argv is an array of strings containing the arguments
argv[0] is the program name, argv[1] is the first argument, and so on
*/

int main(int argc, char *argv[])
{
    if (argc != 2) { // The program expects one argument

    // fprint allows us to specify a file stream to print to
    // In this case, we are printing to stderr, the standard error stream
    // The error message tells us that the program expects one argument
	fprintf(stderr, "usage: cpu <string>\n");
	exit(1); // exit with status 1, meaning error
    }
    char *str = argv[1];

    while (1) {
	printf("%s\n", str);
	Spin(1);
    }
    return 0;
}