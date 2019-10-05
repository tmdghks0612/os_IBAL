#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitmap.h"
#include "hash.h"
#include "debug.h"
#include "list.h"
#include "round.h"
#include "limits.h"

#define MAX_ARRAY_NUM 10

enum DataType {
    LIST, BITMAP, HASH
};

typedef struct tagListList {
    char name[20];
    struct list list;
    
} ListList;
typedef struct tagBitList {
    char name[20];
	struct bitmap* bitmap;
} BitList;

typedef struct tagIntList {
    struct list_elem elem;
    int val;
} IntList;
typedef struct tagIntHash {
	struct hash_elem elem;
	int val;
} IntHash;

int FindArray(void* arr, char *name, int data); // Data의 이름을 받아 array의 index를 반환
bool ListLessFunction(const struct list_elem *a, const struct list_elem*b, void* aux);
void list_swap(struct list_elem *a, struct list_elem *b);
void list_shuffle(struct list *list);
int InputCheck(char** p, int num);
unsigned hash_int_2(int i);
bool HashLessFunction(const struct hash_elem *a, const struct hash_elem *b, void *aux);
unsigned HashFunction (const struct hash_elem *e, void *aux);
void HashDestructor (struct hash_elem *e, void *aux);
void HashSquare (struct hash_elem *e, void *aux);
void HashTriple (struct hash_elem *e, void *aux);
struct bitmap *bitmap_expand(struct bitmap *bitmap, int size);
