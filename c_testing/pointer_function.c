#include <stdio.h>

/* 
The key lesson:
A pointer is just a variable which holds a memory address as its value.
We can't just have an addr type, since that would be equivalent to just having void pointers.
In almost all cases, it is useful to know how to interpret the data at the address.

So in contrast to other things, the pointer stores two pieces of information: 
1) the address (the value of the variable)
2) the type of the data at the address.
*/


struct Range {
    short start;
    short end;
};

void pointer_function(short *pointer);
void struct_function(struct Range range);
void struct_function_pointer(struct Range *range);
void null_pointer(struct Range *range);

int main() {

    short bob = 5;
    pointer_function(&bob); // The address of the short

    struct Range myRange = {1, 10};
    struct_function(myRange);
    struct_function_pointer(&myRange);

    struct Range uninitialized;
    null_pointer(&uninitialized);

    return 0;

}

void pointer_function(short *pointer) {

    printf("Argument: %hd\n", *pointer); // The value of the short
    printf("Address: %p\n", pointer); // The address of the short
    printf("Using &: %p\n", &pointer); // The address of the pointer itself
    printf("\n");
}

void struct_function_pointer(struct Range *range) {
    printf("Struct function pointer\n");
    printf("Start: %hd\n", range->start);
    printf("End: %hd\n", range->end);
    printf("Address: %p\n", range);
    printf("Using &: %p\n", &range);
}

void struct_function(struct Range range) {
    printf("Struct function\n");
    printf("Start: %hd\n", range.start);
    printf("End: %hd\n", range.end);
    printf("Address: %p\n", &range);
    printf("\n");
}

void null_pointer(struct Range *range) {

    struct Range bernard = *range;

    // Illegal operation
    // if(*range) {
    //     printf("Not null\n");
    // } else {
    //     printf("Null\n");
    // }

    if (range) {
        printf("Not null\n");
    } else {
        printf("Null\n");
    }

    if (range->end) {
        printf("Not null\n");
    } else {
        printf("Null\n");
    }
}