#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler(int sig) {
    char buffer[50];
    int length = snprintf(buffer, sizeof(buffer), "Caught SIGINT (%d)!\n", sig);
    write(1, buffer, length);
    exit(1); // This will terminate the entire program
}

int main() {
    signal(SIGINT, sigint_handler);  // Register the SIGINT handler

    while (1) {
        sleep(1);  // Just sleeping for demonstration purposes
    }

    return 0;
}
