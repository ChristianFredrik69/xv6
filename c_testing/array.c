#include <stdio.h>

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
    
    return 0;
}
