#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

int
main (int argc UNUSED, char* argv[])
{
    printf("main here!\n");
    printf("arg0 in sum.c is %p\n",argv[0]);
    printf("%d %d\n",fibonacci(atoi(argv[1])),sum_of_four_int(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4])));

    return EXIT_SUCCESS;
}
