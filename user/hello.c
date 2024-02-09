#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

/*
Make clean to remove all of the compiled stuff.
*/

int main(int argc, char *argv[]) { // The standard main declaration

    if (argc > 1) { // argc is always at least 1, since the program name counts as an argument
        printf("Hello %s, nice to meet you!\n", argv[1]);
    } else {
        printf("Hello World\n");
    }
    return 0; // 0 usually means success
}