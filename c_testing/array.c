#include <stdio.h>


void bit_shifting(int number);
void array_decrement();

int main() {
    int array[] = {1, 2, 3, 4, 5};
    int length = sizeof(array) / sizeof(array[0]);

    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    int *point;

    for (int *p = array; p < &array[length]; p++) {
        printf("Digit: %d ", *p);
        if (*p == 5) {
            printf("Last digit\n");
            point = p;
        }
        else {
            printf("Not last digit\n");
        }
        printf("Address: %p\n", p);
    }

    printf("Array address: %p\n", array);
    printf("Integer value: %d\n", *point);
    
    int myNumber = 4;
    bit_shifting(myNumber);
    array_decrement();

    return 0;
}

void bit_shifting(int number) {

    printf("Number: %d\n", number);
    printf("Number << 1: %d\n", number << 1);
    printf("Number >> 1: %d\n", number >> 1);

    printf("LSHIFT: 1 << 0: %ld\n", 1L << 0);
    printf("LSHIFT: 1 << 1: %ld\n", 1L << 1);
    printf("LSHIFT: 1 << 2: %ld\n", 1L << 2);
    printf("LSHIFT: 1 << 3: %ld\n", 1L << 3);
    printf("LSHIFT: 1 << 4: %ld\n", 1L << 4);

    printf("2 + 3 << 1: %d\n", 2 + 3 << 1);

}

void array_decrement() {

    int array[] = {1, 2, 3, 4, 5};
    --array[4];
    for (int i = 0; i < 5; i++) {
        printf("%d ", array[i]);
    }

    if (--array[4] == 3) {
        printf("Decrement successful\n");
    }
    else {
        printf("Decrement unsuccessful\n");
    }

    for (int i = 0; i < 5; i++) {
        printf("%d ", array[i]);
    }

}

