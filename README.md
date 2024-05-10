# XV6

## Table of contents

- [XV6](#xv6)
  - [Table of contents](#table-of-contents)
  - [Running and terminating the operating system](#running-and-terminating-the-operating-system)
  - [Rebuilding XV6](#rebuilding-xv6)
  - [First hand-in](#first-hand-in)
    - [Task1](#task1)
    - [Task2](#task2)

## Running and terminating the operating system

To run the operating system, you can run the following command:

```zsh
make qemu
```

To terminate the operating system, you can press the following key combination:

```zsh
(CTRL + A) + X
```

## Rebuilding XV6

To reset the xv6 operating system, you can run the following command:

```zsh
make clean
```

Then you can start over by running the following command:

```zsh
make qemu
```

We are assuming a lot of things here, among other things that you have linux
set up, and that qemu is installed, etc.

## First hand-in

There are a lot of things which needed to be done to make the first handin work.

### Task1

When defining a user call, I needed to first make a file in the user space, and then
I needed to add that file under the UPROGS in Makefile. I also needed to add the
relevant headers to the c-file in the user space. There is no `stdio.h` in this
operating system, so if you need something like printf, you need to import
the `user.h` file (as an example).

### Task2

When defining a system call, we needed to go through several files.
We started by defining the system call in `syscall.h`.
The exact code was:

```c
#define SYS_christian 22
```

Here we are defining the name of the system call, and its corresponding ID.

Next, we added these two lines in `syscall.c`:

```c
extern uint64 sys_christian(void);
[SYS_christian] sys_christian,
```

I am not sure what these are doing at the moment, but they are necessary, and
I am just following the template from the other system calls at the moment.

I then went to `sysproc.c` and added the following code:

```c
int sys_christian(void)
{
    printf("Christian Fredrik Johnsen\n");
    return 0;
}
```

This is the actual code which is executed when the system call is called.
The next step was to add the system call to the user space. I did this by
adding the following code to the `user.h` file:

```c
int christian(void);
```

We also needed to add this line to the `usys.pl` file:

```perl
entry(christian);
```

Again, I am not sure what this does, but it is necessary, and
the other system calls seem to follow the same pattern.

Finally, I added my own user program, which calls the system call. The user program is called `christian.c`, and it was set up following the template from task 1.

The code is extremely simple, the whole file is:

```c
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

int main() {
    return christian();
}
```

The result is that we are now able to call the system call from the user space, by running the command:

```zsh
christian
```

(3)
