# Debug the XV6 OS

## Step 1: Compile the code
Start by typing the following command in the terminal:

```bash
make qemu-gdb
```

## Step 2: Start gdb
Open a new terminal, and navigate to the kernel directory.

```bash
cd kernel
```

Start the debugger by typing the following command:

```bash
gdb
```

## Step 3: Set port number
Set the port number to 26000 (the default port number for the QEMU debugger).

```bash
target remote localhost:26000
```

You can now use gdb as usual to debug the xv6 OS.