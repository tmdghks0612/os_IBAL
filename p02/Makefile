OBJECTS = main.o bitmap.o list.o hash.o
CC = gcc
CFLAGS = -g 

aa.out : main.o bitmap.o list.o hash.o
	$(CC) $(CFLAGS) -o aa.out main.o bitmap.o list.o hash.o

main.o : main.c a.h
	$(CC) $(CFLAGS) -c main.c

bitmap.o : bitmap.c
	$(CC) $(CFLAGS) -c bitmap.c

list.o : list.c
	$(CC) $(CFLAGS) -c list.c

hash.o : hash.c
	$(CC) $(CFLAGS) -c hash.c

clean :
	rm *.o aa.out
