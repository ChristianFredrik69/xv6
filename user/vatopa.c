#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

int main(int argc, char **argv) {
    
    if (argc == 1) {
        fprintf(2, "Usage: vatopa <virtual address> [PID]\n");
        exit(1);
    }

    
    uint64 va = (uint64) atoi(argv[1]);

    if (argc == 2) {
        return va2pa(va, getpid());
    }
    
    else {
        int pid = atoi(argv[2]);
        return va2pa(va, pid);
    }

    return 0;
}