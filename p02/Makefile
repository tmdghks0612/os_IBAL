OBJECTS = main.o bitmap.o list.o hash.o
CC = gcc
CFLAGS = -g 
TARGET = aa.exe

$(TARGET) : main.o bitmap.o list.o hash.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o bitmap.o list.o hash.o

main.o : main.c main.h
	$(CC) $(CFLAGS) -c main.c

bitmap.o : bitmap.c
	$(CC) $(CFLAGS) -c bitmap.c

list.o : list.c
	$(CC) $(CFLAGS) -c list.c

hash.o : hash.c
	$(CC) $(CFLAGS) -c hash.c

clean :
	rm *.o aa.out
