#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "process.h"
#include "devices/shutdown.h"
#define READ_MAX_LENGTH 128
static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{
  unsigned int separate_systemcall = *(unsigned int*)(f->esp);
  void *arg0 = f->esp + 4;
  void *arg1 = arg0 + 4;
  void *arg2 = arg0 + 8;
  int i = 0;
  int len = *(unsigned int*)arg2;
  char str[READ_MAX_LENGTH] = "";
  unsigned int fd;
  tid_t tid;

  switch (separate_systemcall) {
  case SYS_HALT:
    shutdown_power_off();
    break;
  case SYS_EXIT:
    printf("exit name : %s\n", thread_name());
    tid = thread_tid();
    thread_exit();
    familyChildToDie(tid);
    break;
  case SYS_EXEC:
    process_execute(*(char**)arg0);
    f->eax = (uint32_t)tid;
    break;
  case SYS_WAIT:
    putbuf("syscall wait!", 14);
    i = process_wait(*(tid_t*)arg0);
    if (i == CHILD_KILL || i == -1)
        f->eax = (uint32_t)-1;
    else
        f->eax = (uint32_t)0;
    break;
  case SYS_READ:
    fd = (int)*(uint32_t*)arg0;

    if (fd == 0) {
        for (i = 0 ; i < len; ++i) {
            str[i] = input_getc();
            putbuf(str+i, 1);
            if (str[i] == 0x0d || str[i] == 0)
                break;
        }
    }
    f->eax = (uint32_t)len;
    break;
  case SYS_WRITE:
    putbuf(*(char**)arg1, *(unsigned int*)arg2);
    f->eax = *(uint32_t*)arg2;
    break;
  default:
    printf("Not implemented System call.\n");
  }
}
