#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

struct spinlock tickslock;
uint ticks;

extern char trampoline[], uservec[], userret[];

// in kernelvec.S, calls kerneltrap().
void kernelvec();

extern int devintr();

void
trapinit(void)
{
  initlock(&tickslock, "time");
}

// set up to take exceptions and traps while in the kernel.
void
trapinithart(void)
{
  w_stvec((uint64)kernelvec);
}

//
// handle an interrupt, exception, or system call from user space.
// called from trampoline.S
//
void
usertrap(void)
{
  int which_dev = 0;

  if((r_sstatus() & SSTATUS_SPP) != 0)
    panic("usertrap: not from user mode");

  w_stvec((uint64)kernelvec); // send interrupts and exceptions to kerneltrap(), since we're now in the kernel.

  struct proc *p = myproc();
  p->trapframe->epc = r_sepc(); // save user program counter.
  
  // if(r_scause() == 15) { // Store/AMO page fault
  //   printf("Inside scause 15\n");
  //   uint64 fault_addr = PGROUNDDOWN(r_stval());
  //   pte_t *pte = walk(p->pagetable, fault_addr, 0); // Walk the page table to find the PTE

  //   if (pte && (*pte & PTE_V) && !(*pte & PTE_W)) {
  //     char *new_pa = kalloc(); // Allocate a new page
  //     if (new_pa == 0) {
  //       panic("usertrap: out of memory\n");
  //     }

  //     printf("Allocated new page at %p\n", new_pa);

  //     uint64 old_pa = PTE2PA(*pte);
  //     memmove(new_pa, (char*)old_pa, PGSIZE);
  //     uvmunmap(p->pagetable, fault_addr, 1, 0); // Unmap the old page, calls kfree at some point, so reference_count is decremented
  //     *pte = PA2PTE((uint64)new_pa) | PTE_W | PTE_R; // Add read/write permissions to the new page
  //     if (mappages(p->pagetable, fault_addr, PGSIZE, (uint64)new_pa, PTE_FLAGS(*pte)) != 0) { // Map the new page to the same location as the old page
  //       panic("usertrap 15: mappages\n");
  //     }
  //     printf("NUM_REFS[PA_INDEX(old_pa)] = %d\n", NUM_REFS[PA_INDEX(old_pa)]);
  //   }
  // }


  if(r_scause() == 8){
    // system call

    if(killed(p))
      exit(-1);

    // sepc points to the ecall instruction,
    // but we want to return to the next instruction.
    p->trapframe->epc += 4;

    // an interrupt will change sepc, scause, and sstatus,
    // so enable only now that we're done with those registers.
    intr_on();

    syscall();
  } else if((which_dev = devintr()) != 0){
    // ok
  } else if (r_scause() == 15) {
  // Modify usertrap() to recognize page faults.
  // When a page-fault occurs on a COW page, allocate a new page with kalloc(),
  // copy the old page to the new page,
  // and install the new page in the PTE with PTE_W set.

    // Get physial page address and correct flags.
    uint64 start_va = PGROUNDDOWN(r_stval());
    pte_t *pte;
    pte = walk(p->pagetable, start_va, 0);
    if (pte == 0) {
      printf("page not found\n");
      p->killed = 1;
      panic("page not found\n");
    }
    printf("usertrap(): page fault at address 0x%p\n", start_va);
    printf("Page number = %d\n", PA_INDEX(PTE2PA(*pte)));
    if (pte && (*pte & PTE_V) && !(*pte & PTE_W) && (*pte & PTE_COW)) {
      uint flags = PTE_FLAGS(*pte);
      flags |= PTE_W;
      flags &= ~PTE_COW;

      char *new_pa = kalloc();
      char *old_pa = (char *)PTE2PA(*pte);
      memmove(new_pa, old_pa, PGSIZE);
      uvmunmap(p->pagetable, start_va, 1, 0);

      if (mappages(p->pagetable, start_va, PGSIZE, (uint64)new_pa, flags) != 0) {
        panic("mappages");
      }
      printf("\n--------------------\nInside usertrap\n--------------------\n");
      printf("Number of references, new page: %d\n", NUM_REFS[PA_INDEX((uint64)new_pa)]);
      printf("Number of references, old page: %d\n", NUM_REFS[PA_INDEX((uint64)old_pa)]);
    }
  }

  
  else {
    printf("usertrap(): unexpected scause %p pid=%d\n", r_scause(), p->pid);
    printf("            sepc=%p stval=%p\n", r_sepc(), r_stval());
    setkilled(p);
  }

  if(killed(p))
    exit(-1);

  // give up the CPU if this is a timer interrupt.
  if(which_dev == 2)
    yield();

  usertrapret();
}

//
// return to user space
//
void
usertrapret(void)
{
  struct proc *p = myproc();

  // we're about to switch the destination of traps from
  // kerneltrap() to usertrap(), so turn off interrupts until
  // we're back in user space, where usertrap() is correct.
  intr_off();

  // send syscalls, interrupts, and exceptions to uservec in trampoline.S
  uint64 trampoline_uservec = TRAMPOLINE + (uservec - trampoline);
  w_stvec(trampoline_uservec);

  // set up trapframe values that uservec will need when
  // the process next traps into the kernel.
  p->trapframe->kernel_satp = r_satp();         // kernel page table
  p->trapframe->kernel_sp = p->kstack + PGSIZE; // process's kernel stack
  p->trapframe->kernel_trap = (uint64)usertrap;
  p->trapframe->kernel_hartid = r_tp();         // hartid for cpuid()

  // set up the registers that trampoline.S's sret will use
  // to get to user space.
  
  // set S Previous Privilege mode to User.
  unsigned long x = r_sstatus();
  x &= ~SSTATUS_SPP; // clear SPP to 0 for user mode
  x |= SSTATUS_SPIE; // enable interrupts in user mode
  w_sstatus(x);

  // set S Exception Program Counter to the saved user pc.
  w_sepc(p->trapframe->epc);

  // tell trampoline.S the user page table to switch to.
  uint64 satp = MAKE_SATP(p->pagetable);

  // jump to userret in trampoline.S at the top of memory, which 
  // switches to the user page table, restores user registers,
  // and switches to user mode with sret.
  uint64 trampoline_userret = TRAMPOLINE + (userret - trampoline);
  ((void (*)(uint64))trampoline_userret)(satp);
}

// interrupts and exceptions from kernel code go here via kernelvec,
// on whatever the current kernel stack is.
void 
kerneltrap()
{
  int which_dev = 0;
  uint64 sepc = r_sepc();
  uint64 sstatus = r_sstatus();
  uint64 scause = r_scause();
  
  if((sstatus & SSTATUS_SPP) == 0)
    panic("kerneltrap: not from supervisor mode");
  if(intr_get() != 0)
    panic("kerneltrap: interrupts enabled");

  if((which_dev = devintr()) == 0){
    printf("scause %p\n", scause);
    printf("sepc=%p stval=%p\n", r_sepc(), r_stval());
    panic("kerneltrap");
  }

  // give up the CPU if this is a timer interrupt.
  if(which_dev == 2 && myproc() != 0 && myproc()->state == RUNNING)
    yield();

  // the yield() may have caused some traps to occur,
  // so restore trap registers for use by kernelvec.S's sepc instruction.
  w_sepc(sepc);
  w_sstatus(sstatus);
}

void
clockintr()
{
  acquire(&tickslock);
  ticks++;
  wakeup(&ticks);
  release(&tickslock);
}

// check if it's an external interrupt or software interrupt,
// and handle it.
// returns 2 if timer interrupt,
// 1 if other device,
// 0 if not recognized.
int
devintr()
{
  uint64 scause = r_scause();

  if((scause & 0x8000000000000000L) &&
     (scause & 0xff) == 9){
    // this is a supervisor external interrupt, via PLIC.

    // irq indicates which device interrupted.
    int irq = plic_claim();

    if(irq == UART0_IRQ){
      uartintr();
    } else if(irq == VIRTIO0_IRQ){
      virtio_disk_intr();
    } else if(irq){
      printf("unexpected interrupt irq=%d\n", irq);
    }

    // the PLIC allows each device to raise at most one
    // interrupt at a time; tell the PLIC the device is
    // now allowed to interrupt again.
    if(irq)
      plic_complete(irq);

    return 1;
  } else if(scause == 0x8000000000000001L){
    // software interrupt from a machine-mode timer interrupt,
    // forwarded by timervec in kernelvec.S.

    if(cpuid() == 0){
      clockintr();
    }
    
    // acknowledge the software interrupt by clearing
    // the SSIP bit in sip.
    w_sip(r_sip() & ~2);

    return 2;
  } else {
    return 0;
  }
}

