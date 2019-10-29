#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{
  printf ("system call!\n");
  unsigned int separate_systemcall = *(unsigned int*)(f->esp);
  void *arg0 = f->esp + 4;
  void *arg1 = arg0 + 4;
  void *arg2 = arg0 + 8;

  switch (separate_systemcall) {
  case SYS_HALT:
    shutdown_power_off();
    break;
  case SYS_EXIT:
    thread_exit();
    break;
  case SYS_EXEC:
    break;
  case SYS_WAIT:

    break;
  case SYS_READ:
    break;
  case SYS_WRITE:
    putbuf(*(char**)arg1, *(unsigned int*)arg2);
    break;
  default:
    printf("Not implemented System call.\n");
  }
  thread_exit ();
}
