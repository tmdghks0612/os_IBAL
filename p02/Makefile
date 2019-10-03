OBJECTS = a.o bitmap.o list.o hash.o
CC = gcc
CFLAGS = -g 

a.exe : a.o bitmap.o list.o hash.o
	$(CC) $(CFLAGS) -o a.exe a.o bitmap.o list.o hash.o

a.o : a.c a.h
	$(CC) $(CFLAGS) -c a.c

bitmap.o : bitmap.c
	$(CC) $(CFLAGS) -c bitmap.c

list.o : list.c
	$(CC) $(CFLAGS) -c list.c

hash.o : hash.c
	$(CC) $(CFLAGS) -c hash.c

clean :
	rm *.o a.exe

