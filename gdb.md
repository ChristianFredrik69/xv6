# Notes on the GDB debugger


## Setting up GDB

To use GDB, you have to compile your program with the `-g` flag. This flag tells the compiler to include debugging information in the executable file. This information is necessary for GDB to work properly. Example:

```bash
gcc -g -o program.c program
```

After compiling, you fire up GDB with the following command:
    
```bash
gdb program
```

## Before running the program: Breakpoints and watchpoints

You set a breakpoint with the `break` (`b`) command. Example: `break 10` sets a breakpoint at line 10.

You can also specify a function: `break main`

You can also set a conditional breakpoint. Example: `break 10 if var == 10`. In practice, this means that you will break at line 10 only if the variable `var` has the value 10. This can be useful if you have a loop and you want to break at a specific iteration.

We can use the `watch` command to set a watchpoint on a variable. This will cause the program to break when the variable is modified.
So you would write `watch var` to set a watchpoint on the variable `var`.

## Running the program

If you want to provide a set of arguments you want to use when running the program, you can do that with the `set args` command. Example: `set args arg1 arg2 .. argn`

Then you run the compiled program with the `run` command:

If you want to override the arguments you set with `set args`, you can do that by providing the new arguments as arguments to the `run` command. Example:
`run arg1 arg2 .. argn`

## Inspecting state

There are several useful commands for inspecting the state of the program. Here are some of them:

### Printing information
- `info args` prints the arguments of the current function
- `info locals` prints the local variables
- `info break` prints the current breakpoints
- `info watch` prints the current watchpoints

### Printing values
- `print` (`p`) prints the value of a specific expression
- `print var` prints the value of a specific variable
- `print *ptr` prints the value of the variable pointed to by `ptr`
- `print *array@n` prints the first `n` elements of the array `array`
- `print *array@elems` prints all the elements of the array `array`

### Printing the call stack and source code
- `backtrace` prints the call stack
- `list` (`l`) prints the source code around the current line

## Stepping through the program
There are two primary commands for stepping through the program:
- `step` (`s`) steps into the next function call
- `next` (`n`) goes to the next line in the current function (does not step into function calls)

You can also use the `continue` (`c`) command to continue running the program until the next breakpoint is reached.

We can use the `jump` (`j`) command to jump to a specific line in the program. Example: `jump 10` jumps to line 10.	You can also jump to a specific function by writing `jump function_name`.

## Modify program state

You can modify the value of a variable with the `set` command. Example: `set var = 10` sets the value of `var` to 10.

## Exiting GDB
You exit GDB with the `quit` (`q`) command.
