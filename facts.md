# Facts

Facts about programming in xv6.

## Add programs

We distinguish between user programs and system programs.

### User programs

If we need to add a user program, we first add the name of the user program under the UPROGS section in the makefile.
Then, we add a programname.c file in the user directory.

### System programs

If want to add a system program, we need to do a lot of stuff. We need to add the name of the system program as an entry in the usys.pl file.

We add a line like this: `extern uint sys_programname(void);` in the syscall.c file

In syscall.h we add a line like this: `#define SYS_programname 22`

Finally, in sysproc.c we add a line like this:

```c
def sys_programname(void) {
  some code...
  return useful_value;
}
```
