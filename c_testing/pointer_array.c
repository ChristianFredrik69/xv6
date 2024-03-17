#include <stdio.h>


// Checking if you can treat a pointer as an array, even in situations where it doesn't make sense
// Indeed, you can. The pointer is treated as an array of the type of the pointer.

int main() {

    char bob_favorite_word[] = "blueue";
    short *pointer = (short *) bob_favorite_word; 

    printf("Address: %p\n", pointer);
    printf("Integer value: %hd\n", *pointer);
    printf("The same value: %hd\n", pointer[0]); // bl
    printf("Some unreasonable value: %hd\n", pointer[1]); // ue
    printf("Another unreasonable value: %hd\n", pointer[2]); // Still interpreting 'ue' as a short, [1] and [2] will be equivalent.

    return 0;

}