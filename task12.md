# Notes for task 1.2

## Table of Contents 📚

- [Notes for task 1.2](#notes-for-task-12)
  - [Table of Contents 📚](#table-of-contents-)
  - [What are we doing?](#what-are-we-doing)
  - [Considerations](#considerations)
  - [Name explanations](#name-explanations)

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

- `vfork`: Virtual fork. This is the system call we are implementing.
- `COW`: Copy On Write. This is the strategy we are using to implement vfork.
- `kalloc`: Kernel allocation. This is the function we use to allocate memory which is used by the kernel.
- `kfree`: Kernel free. This is the function we use to free memory which is used by the kernel.
