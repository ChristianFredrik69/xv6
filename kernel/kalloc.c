// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

/*
PERSONAL COMMENTS:

PGSIZE is defined in riscv.h as 4096
This means that we have 4096 bytes per page. 4096 = 2**12

XV6 uses 32-bit addressing, so we have 2**32 addresses
2**32 / 2**12 = 2**20, meaning we have 2**20 pages -> The first 20 bits of the address specifies the page number
2**20 = 1048576 different pages at most.
*/

uint64 MAX_PAGES = 0; // Maximum number of pages is dynamically set when booting the kernel. Depends on the amount of physical memory available and the amount of memory reserved for the kernel.
uint64 FREE_PAGES = 0; // We dynamically keep track of the number of free pages.
uint64 NUM_REFS[33000]; // Based on print-statements, MAX_pages is 32731, and I am adding a bit of extra space for safety. We can now have a reference count for each page.
void freerange(void *pa_start, void *pa_end);
extern char end[]; // first address after kernel, defined by kernel.ld

/*
A run is a free block of memory.
The entire structure is a singly linked list. Each run instance points to the next free page.
*/
struct run { // A run is a free block of memory
    struct run *next;
};

/*
A structure which manages the pool of free memory pages.
We only need one instance of this structure, thus the struct is anonymous.
*/
struct {
    struct spinlock lock; // The lock is used to ensure exclusive access to the freelist
    struct run *freelist; // The linked list of free pages
} kmem;

/*
I don't know the specifics, but this is used when booting the kernel.
*/
void kinit() {
    initlock(&kmem.lock, "kmem"); // Set up the lock for the freelist
    char *p = (char *)PGROUNDUP((uint64)end); // Rounds up the start address to the next page boundary
    for (; p + PGSIZE <= (char *)(void *)PHYSTOP; p += PGSIZE) { // Free all pages between pa_start and pa_end
        NUM_REFS[((uint64)p - *end) >> 12] = 1; // Stupid workaround, else the reference counts become -1 after booting the kernel
    }
    freerange(end, (void *)PHYSTOP); // Free all pages between the end of the kernel and the end of physical memory
    
    MAX_PAGES = FREE_PAGES; // The maximum number of pages is set to the number of free pages after booting the kernel
    printf("End: %p, PHYSTOP: %p, MAX_PAGES: %d\n", end, (void *)PHYSTOP, MAX_PAGES);
}

/*
Frees all pages between pa_start and pa_end.
Uses kfree as sub-routine.
*/
void freerange(void *pa_start, void *pa_end) {
    char *p = (char *)PGROUNDUP((uint64)pa_start); // Rounds up the start address to the next page boundary
    for (; p + PGSIZE <= (char *)pa_end; p += PGSIZE) { // Free all pages between pa_start and pa_end
        kfree(p);
    }
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void kfree(void *pa) {
    if (MAX_PAGES != 0) // sanity check, you cannot have more free pages than the maximum number of pages
        assert(FREE_PAGES < MAX_PAGES);

    if (((uint64)pa % PGSIZE) != 0 || (char *)pa < end || (uint64)pa >= PHYSTOP || NUM_REFS[((uint64)pa - *end) >> 12] == 0) // Check if the address is valid
        panic("kfree");
    
    if (--NUM_REFS[(uint64)pa >> 12] > 0) // Decrement the reference count, and if the reference count is greater than 0, the page is still in use.
        return;
    
    // Reference count is 0 -> Free the page.
    memset(pa, 1, PGSIZE); // fill with junk (the value 1 is inserted in each byte of the page)
    struct run *r = (struct run *)pa; // The address of the page is casted to a run pointer, each free page is a run.

    acquire(&kmem.lock); // Ensure exclusive access to the freelist

    r->next = kmem.freelist; // Fix the next pointer of the free page to point to the current run at the head of the freelist
    kmem.freelist = r; // Insert the current run at the head of the freelist
    FREE_PAGES++; // We have successfully freed a page, so we increment the number of free pages
    
    release(&kmem.lock); // Release the lock, making the freelist available to other threads
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void) {
    /*
    I must say that these if(r) checks should not be necessary.
    You are already checking that number of free pages is greater than 0.
    It is beyond me how you could have a free page without a run.
    */
    assert(FREE_PAGES > 0); // sanity check, you cannot allocate a page if there are no free pages
    acquire(&kmem.lock); // Ensure that you are the only one accessing the freelist
    struct run *r = kmem.freelist; // Get the first run in the freelist
    if (r) // If the freelist is not empty
        kmem.freelist = r->next; // Update the head of the freelist to point to the next run
    release(&kmem.lock); // Release the lock, making the freelist available to other threads

    if (r) { // If the freelist is not empty 
        memset((char *)r, 5, PGSIZE); // fill with junk
        NUM_REFS[((uint64)r - *end) >> 12]++; // Increment the reference count of the page. RSHIFT 12 is the same as dividing by 4096 and then taking the floor.
    }
    FREE_PAGES--; // Decrement the number of free pages
    return (void *)r; // Return a pointer to the allocated page
}


/*
On kalloc,
the page index is calculated (as (r - end) / pagesize), and its counter incremented.

On kfree, the page index is calculated as (pa - end) / pagesize), 
and its counter decremented. If it becomes 0, the page is freed.
*/