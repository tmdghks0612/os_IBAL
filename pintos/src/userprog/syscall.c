#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "process.h"
#include "devices/shutdown.h"
#include "pagedir.h"
#include "threads/vaddr.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "threads/synch.h"
#define READ_MAX_LENGTH 128

static struct lock file_lock;

static void syscall_handler (struct intr_frame *);
static int checkValidAddress(void *addr);
static int syscall_fibonacci(int a);
void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  lock_init(&file_lock);
}

static void
syscall_handler (struct intr_frame *f) 
{
  if(!checkValidAddress(f->esp)) {
    printf("%s: exit(-1)\n", thread_name());
    thread_exit();
  }
  unsigned int separate_systemcall = *(unsigned int*)(f->esp);
  void *arg0 = f->esp + 4;
  void *arg1 = arg0 + 4;
  void *arg2 = arg0 + 8;
  void *arg3 = arg0 + 12;
  int i = 0, fd;
  char *str;
  unsigned int len;
  tid_t tid;
  struct file* tempfile;

  switch (separate_systemcall) {
  case SYS_HALT:
    shutdown_power_off();
    break;
  case SYS_EXIT:
    if (!checkValidAddress(arg0)) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    printf("%s: exit(%d)\n", thread_name(), *(int*)arg0);
    tid = thread_tid();
    familyChildToDie(tid, *(int*)arg0);
    thread_exit();
    break;
  case SYS_EXEC:
    if (!checkValidAddress(arg0)) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    if (!checkValidAddress((void*)(*(char**)arg0))) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    tid = process_execute(*(char**)arg0);
    f->eax = (uint32_t)tid;
    break;
  case SYS_WAIT:
    if (!checkValidAddress(arg0)) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    i = process_wait(*(tid_t*)arg0);
    f->eax = (uint32_t)i;
    break;
  case SYS_READ:
    fd = *(int*)arg0;
    str = *(char**)arg1;
    len = *(unsigned int*)arg2;
    if (!checkValidAddress(arg2)) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    if (!checkValidAddress((void*)str) && !checkValidAddress((void*)(str+len)))
        thread_exit();

    if (fd == 0) {
        for (i = 0 ; i < (int)len; ++i) {
            str[i] = input_getc();
            putbuf(str+i, 1);
            if (str[i] == 0x0d || str[i] == 0)
                break;
        }
    }
    else {
        tempfile = getFilepointerFromFd(fd);
        if (!tempfile) {
          printf("%s: exit(-1)\n", thread_name());
          thread_exit();
        }
        lock_acquire(&file_lock);
        len = file_read(tempfile, (void*)str, len);
        lock_release(&file_lock);
    }
    f->eax = (uint32_t)len;
    break;
  case SYS_WRITE:
    fd = *(int*)arg0;
    str = *(char**)arg1;
    len = *(unsigned int*)arg2;
    if (!checkValidAddress(arg2)) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    if (!checkValidAddress((void*)str) && !checkValidAddress((void*)(str+len)))
        thread_exit();

    if (fd == 1)
        putbuf(str, len);
    else {
        tempfile = getFilepointerFromFd(fd);
        if (!tempfile) {
          printf("%s: exit(-1)\n", thread_name());
          thread_exit();
        }
        lock_acquire(&file_lock);
        len = file_write(tempfile, (void*)str, len);
        lock_release(&file_lock);
    }
    f->eax = len;
    break;
  case SYS_FIBONACCI:
    if (!checkValidAddress(arg0)) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    f->eax = syscall_fibonacci(*(int*)arg0);
    break;
  case SYS_SUM_OF_FOUR_INT:
    if (!checkValidAddress(arg3)) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    f->eax = *(int*)arg0 + *(int*)arg1 + *(int*)arg2 + *(int*)arg3;
    break;
    // Proj2 Start
  case SYS_CREATE:
    if (!checkValidAddress(arg1)) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    if (!checkValidAddress((void*)(*(char**)arg0))){
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    f->eax = filesys_create(*(char**)arg0, *(int*)arg1);
    break;
  case SYS_REMOVE:
    if (!checkValidAddress(arg0)) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    if (!checkValidAddress((void*)(*(char**)arg0))) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    f->eax = filesys_remove(*(char**)arg0);
    break;
  case SYS_OPEN:
    if (!checkValidAddress(arg0)) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    if (!checkValidAddress((void*)(*(char**)arg0))) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    
    break;
  case SYS_CLOSE:
    if (!checkValidAddress(arg0)) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    
    break;
  case SYS_FILESIZE:
    if (!checkValidAddress(arg0)) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    tempfile = getFilepointerFromFd(*(int*)arg0);
    if (!tempfile) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    
    f->eax = file_length(tempfile);
    break;
  case SYS_SEEK:
    if (!checkValidAddress(arg1)) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    tempfile = getFilepointerFromFd(*(int*)arg0);
    if (!tempfile) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    if ((fd = *(int*)arg1) < 0) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    file_seek(tempfile, fd); 
    break;
  case SYS_TELL:
    if (!checkValidAddress(arg0)) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    tempfile = getFilepointerFromFd(*(int*)arg0);
    if (!tempfile) {
        printf("%s: exit(-1)\n", thread_name());
        thread_exit();
    }
    f->eax = file_tell(tempfile);
    break;
  default:
    printf("Not implemented System call.\n");
  }
}
// if addr is valid user page, return 1 . Else return 0.
static int
checkValidAddress(void *addr) {
    if (!is_user_vaddr(addr))
        return 0;
    if (pagedir_get_page(thread_current()->pagedir, addr) == NULL)
        return 0;
    return 1;
}

static int
syscall_fibonacci(int a) {
    int old = 0, new = 1, temp, i;

    for (i = 2; i <= a; ++i) {
        temp = new;
        new += old;
        old = temp;
    }
    if (a == 0)
        return 0;
    else
        return new;
}
