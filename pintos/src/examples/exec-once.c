/* Executes and waits for a single child process. */

#include <syscall.h>

int
main (int argc, char* argv[]) 
{
  wait (exec ("child-simple"));

  return 0;
}
