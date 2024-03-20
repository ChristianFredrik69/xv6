# Notes for task 1.2

## Table of Contents 📚

- [Notes for task 1.2](#notes-for-task-12)
  - [Table of Contents 📚](#table-of-contents-)
  - [What are we doing?](#what-are-we-doing)
  - [Considerations](#considerations)
  - [Name explanations](#name-explanations)
  - [PTE flags](#pte-flags)

## What are we doing?

We are going to implement vfork in the xv6 operating system.
Instead of copying the contents of the parent process (the stuff in RAM) to the child process, we will just give the child process a pointer to the parent's memory.
This is especially great if the child doesn't need to modify the memory of the parent.
The child will only make a copy of the parent's memory if it needs to modify it.
All the data which is just read by the child will be shared with the parent.

The strategy we are using is called COW (Copy On Write). It makes sense, we only copy the memory if we need to write to it.

## Considerations

When implementing vfork, we need to have some set of extra variables to keep track of which pages are shared and which are not.

## Name explanations

- `fork`: Make a copy of the current process.
- `vfork`: Virtual fork. This is the system call we are implementing.
- `COW`: Copy On Write. This is the strategy we are using to implement vfork.
- `kalloc`: Kernel allocation. This is the function we use to allocate memory which is used by the kernel.
- `kfree`: Kernel free. This is the function we use to free memory which is used by the kernel.
- `PTE`: Page Table Entry. This is a structure which provides the mapping between virtual and physical memory.

## PTE flags

PTEs are really complicated, they are not just a simple mapping between virtual and physical memory.
It also contains metadata that specifies the access permissions and state of the physical page in memory.
Each PTE typically contains:

- Physical address of the page
- Flags for the page
  - `PTE_V` (valid)
  - `PTE_R` (readable)
  - `PTE_W` (writable)
  - `PTE_X` (executable)
  - `PTE_U` (user accessible)

Vatopa: if 0 argument, then use myproc.
