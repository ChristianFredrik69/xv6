#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

int main(int argc, char *argv[]) {
    
    if (argc == 1) {
        printf("Usage: vatopa virtual_address [pid]\n");
        return 0;
    }
    
    uint64 va = (uint64) atoi(argv[1]);

    /*
    Stupid printing procedure. Instead of writing %p in kernel space, we return the value down to user space and then need to perform the other-worldly 
    printf("0x%x\n", va2pa(va, getpid()));
    to get a printed value which matches the expected output in the tests.
    The tests only want 32-bit addresses, presumably because the kernel is 32-bit.
    */
   
    if (argc == 2) {
        printf("0x%x\n", va2pa(va, getpid()));
    }
    
    else {
        int pid = atoi(argv[2]);
        printf("0x%x\n", va2pa(va, pid));
    }

    return 0;
}