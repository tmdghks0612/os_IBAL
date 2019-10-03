#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"
#include "debug.h"
#include "list.h"
#include "round.h"
#include "limits.h"

typedef struct tagListList {
    char name[20];
    struct list list;
    
} ListList;
typedef struct tagHashList {
    char name[20];
} HashList;
typedef struct tagBitList {
    char name[20];
} BitList;

typedef struct tagIntList {
    struct list_elem elem;
    int val;
} IntList;