#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if (argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if (argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;

  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (myproc()->killed) {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // get the parameters 
  uint64 page, addr;
  int num;
  if (argaddr(0, &page) < 0 ||
    argint(1, &num) < 0 ||
    argaddr(2, &addr) < 0)
    return -1;


  // check at most 512 * 8 pages
  if (num > 512 * 8)
    return num = 512 * 8;
  char buffer[512];
  memset(buffer, 0, sizeof(char) * 512);

  // printf("initial buffer: %x\n", *(int*)buffer);

  // vmprint(myproc()->pagetable);

  for (int i = 0; i < num; ++i) {
    pte_t* pte = walk(myproc()->pagetable, page + i * PGSIZE, 0);
    if (pte == 0)
      panic("sys_pgaccess: walk");
    // printf("%x\n", *pte);
    if (*pte & PTE_A) {
      buffer[i / 8] |= (1L << (i % 8));
      *pte &= (~PTE_A);
    }
  }
  // printf("end buffer: %x\n", *(int*)buffer);

  return copyout(myproc()->pagetable, addr, buffer, num);
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
