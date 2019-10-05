#include "main.h"

int main() {
	char user_command[300]; // user command
	char *comtok[10]; // tockenized user commands
	int list_num = 0, hash_num = 0, bit_num = 0;
	ListList listarr[MAX_ARRAY_NUM];
	struct hash hasharr[MAX_ARRAY_NUM];
	BitList bitarr[MAX_ARRAY_NUM];

	for (int i = 0; i < MAX_ARRAY_NUM; ++i) {
		listarr[i].name[0] = '\0';
		hasharr[i].aux = NULL;
		bitarr[i].name[0] = '\0';
	}

	while (1) {
		fgets(user_command, 300, stdin);
		user_command[strlen(user_command)-1] = '\0';
		comtok[0] = strtok(user_command, " ");

		if (!comtok[0]) // 아무것도 입력하지 않았을 시
			continue;

		if (!strcmp(comtok[0], "quit"))
			break;
		else if (!strcmp(comtok[0], "create")) {
			comtok[1] = strtok(NULL, " ");
			comtok[2] = strtok(NULL, " ");
			comtok[3] = strtok(NULL, " ");
			comtok[4] = strtok(NULL, " ");
			
			if (comtok[4]) {
				puts("There are too many inputs");
				continue;
			}

			if (!(comtok[1] && comtok[2])) {
				puts("There are insufficient inputs!");
				continue;
			}

			else if (!strcmp(comtok[1], "list")) { // list case
				if (comtok[3]) {
					puts("There are too many inputs");
					continue;
				}
				int i;
				for (i = 0 ; i < MAX_ARRAY_NUM; ++i)
					if (!listarr[i].name[0])
						break;
				if (i == MAX_ARRAY_NUM)
					puts("list array is full.");
				else {
					list_init(&(listarr[i].list));
					strncpy(listarr[i].name, comtok[2], 20);
				}
			}
			else if (!strcmp(comtok[1], "hashtable")) { // hashtable case
				if (comtok[3]) {
					puts("There are too many inputs");
					continue;
				}
				int i;
				for (i = 0; i < MAX_ARRAY_NUM; ++i)
					if (!hasharr[i].aux)
						break;
				if (i == MAX_ARRAY_NUM) {
					puts("hash array is full!");
					continue;
				}
				char *name = (char*)malloc(sizeof(char)*(strlen(comtok[2])+1));
				strncpy(name, comtok[2], 20);
				if (!hash_init(hasharr+i, HashFunction, HashLessFunction, (void*)name))
					puts("hash_init fail!");
			}
			else if (!strcmp(comtok[1], "bitmap")) {
				if (!comtok[3]) {
					puts("There are insufficient inputs!");
					continue;
				}
					
				int i;
				for (i = 0; i < MAX_ARRAY_NUM; ++i)
					if (!bitarr[i].name[0])
						break;

				if (i == MAX_ARRAY_NUM) {
					puts("bitmap array is full.");
					continue;
				}

				strncpy(bitarr[i].name, comtok[2], 20);
				bitarr[i].bitmap = bitmap_create((size_t)atoi(comtok[3]));
			}
			else
				printf("We can't make this type : %s\n", comtok[1]);
		}
		else if (!strcmp(comtok[0], "dumpdata")) {
			if (InputCheck(comtok, 2))
				continue;
			else {
				int i = FindArray((void*)listarr, comtok[1], LIST);

				if (i != MAX_ARRAY_NUM) { // dumpadata list
					IntList *temp = NULL;
					for (struct list_elem* e = list_begin (&(listarr[i].list)); e != list_end (&(listarr[i].list)); e = list_next (e)) {
						temp = list_entry (e, IntList, elem);
						printf("%d ", temp->val);
					}
					if (temp)
						puts("");
				}
				else {
					int i = FindArray((void*)hasharr, comtok[1], HASH);

					if (i != MAX_ARRAY_NUM) { // dumpdata hash
						struct hash_iterator iter;
						struct hash_elem *e;
						hash_first(&iter, &hasharr[i]);
						int check = 0;
						while (e = hash_next(&iter)) {
							check = 1;
							printf("%d ", hash_entry(e, IntHash, elem)->val);
						}
						if (check)
							puts("");
					}
					else {
						int i = FindArray((void*)bitarr, comtok[1], BITMAP);
						if (i == MAX_ARRAY_NUM) {
							puts("There isn't bitmap of name you enter");
							continue;
						}
						size_t j; // dunpdata bitmap
						for (j = 0; j < bitmap_size(bitarr[i].bitmap); ++j) {
							if (bitmap_test(bitarr[i].bitmap, j))
								fputs("1", stdout);
							else
								fputs("0", stdout);
						}
						if (j)
							puts("");
					}
				}
			}
		}
		else if (!strcmp(comtok[0], "delete")) {
			if (InputCheck(comtok, 2))
				continue;

			int i = FindArray((void*)listarr, comtok[1], LIST);
			if (i != MAX_ARRAY_NUM) { // delete list
				IntList* temp;
				struct list_elem *e;
				while (!list_empty(&listarr[i].list)) {
					e = list_pop_front(&listarr[i].list);
					temp = list_entry(e, IntList, elem);
					free(temp);
				}
				listarr[i].name[0] = '\0';
			}
			else {
				int i = FindArray((void*)hasharr, comtok[1], HASH);
				if (i != MAX_ARRAY_NUM) { // delete hash
					hash_destroy(&hasharr[i], HashDestructor);
					free(hasharr[i].aux);
					hasharr[i].aux = NULL;
				}
				else {
					int i = FindArray((void*)bitarr, comtok[1], BITMAP);
					if (i == MAX_ARRAY_NUM)
						continue;

					bitmap_destroy(bitarr[i].bitmap);
					bitarr[i].name[0] = '\0';
				}
			}
		}
		else if (!strcmp(comtok[0], "list_insert")) {
			if (InputCheck(comtok, 4))
				continue;
			else {
				int i = FindArray((void*)listarr, comtok[1], LIST);
				
				if (i == MAX_ARRAY_NUM) // 해당 이름의 list가 없을 경우
					puts("There is not list of name you enter!");
				else {
					struct list_elem* e = list_begin(&(listarr[i].list));
					IntList *temp = (IntList*)malloc(sizeof(IntList));
					for (int j = 0; j < atoi(comtok[2]); ++j) {
						if (e != list_end(&(listarr[i].list)))
							e = list_next(e);
						else
							break;
					}
					
					temp->val = atoi(comtok[3]);
					list_insert(e, &(temp->elem));
				}
			}
		}
		else if (!strcmp(comtok[0], "list_push_front")) {
			if (InputCheck(comtok, 3))
				continue;
			else {
				int i = FindArray((void*)listarr, comtok[1], LIST);
				if (i == MAX_ARRAY_NUM) // 해당 이름의 list가 없을 경우
					puts("There is not list of name you enter!");
				else {
					IntList *e = (IntList*)malloc(sizeof(IntList));
					e->val = atoi(comtok[2]);

					list_push_front(&(listarr[i].list), &(e->elem));
					}
			}
		}
		else if (!strcmp(comtok[0], "list_push_back")) {
			if (InputCheck(comtok, 3))
				continue;
			else {
				int i = FindArray((void*)listarr, comtok[1], LIST);
				if (i == MAX_ARRAY_NUM) // 해당 이름의 list가 없을 경우
					puts("There is not list of name you enter!");
				else {
					IntList *e = (IntList*)malloc(sizeof(IntList));
					e->val = atoi(comtok[2]);

					list_push_back(&(listarr[i].list), &(e->elem));
				}
			}
		}
		else if (!strcmp(comtok[0], "list_front")) {
			if (InputCheck(comtok, 2))
				continue;
			else {
				int i = FindArray((void*)listarr, comtok[1], LIST);
				if (i == MAX_ARRAY_NUM) // 해당 이름의 list가 없을 경우
					puts("There is not list of name you enter!");
				else {
					struct list_elem *e = list_front(&(listarr[i].list));
					printf("%d\n", list_entry(e, IntList, elem)->val);
				}
			}
		}
		else if (!strcmp(comtok[0], "list_back")) {
			if (InputCheck(comtok, 2))
				continue;
			else {
				int i = FindArray((void*)listarr, comtok[1], LIST);
				if (i == MAX_ARRAY_NUM) // 해당 이름의 list가 없을 경우
					puts("There is not list of name you enter!");
				else {
					struct list_elem *e = list_back(&(listarr[i].list));
					printf("%d\n", list_entry(e, IntList, elem)->val);
				}
			}
		}
		else if (!strcmp(comtok[0], "list_pop_front")) {
			if (InputCheck(comtok, 2))
				continue;
			else {
				int i = FindArray((void*)listarr, comtok[1], LIST);
				if (i == MAX_ARRAY_NUM) // 해당 이름의 list가 없을 경우
					puts("There is not list of name you enter!");
				else {
					struct list_elem *e = list_pop_front(&(listarr[i].list));
					free(list_entry(e, IntList, elem));
				}
			}
		}
		else if (!strcmp(comtok[0], "list_pop_back")) {
			if (InputCheck(comtok, 2))
				continue;
			else {
				int i = FindArray((void*)listarr, comtok[1], LIST);
				if (i == MAX_ARRAY_NUM) // 해당 이름의 list가 없을 경우
					puts("There is not list of name you enter!");
				else {
					struct list_elem *e = list_pop_back(&(listarr[i].list));
					free(list_entry(e, IntList, elem));
				}
			}
		}
		else if (!strcmp(comtok[0], "list_insert_ordered")) {
			if (InputCheck(comtok, 3))
				continue;
			else {
				int i = FindArray(listarr, comtok[1], LIST);
				if (i == MAX_ARRAY_NUM)
					puts("There isn't list of name you enter.");
				else {
					IntList *new = (IntList*)malloc(sizeof(IntList));
					new->val = atoi(comtok[2]);
					list_insert_ordered(&listarr[i].list, &new->elem, ListLessFunction, NULL);
				}
			}
		}
		else if (!strcmp(comtok[0], "list_empty")) {
			if (InputCheck(comtok, 2))
				continue;
			else {
				int i = FindArray(listarr, comtok[1], LIST);
				if (i == MAX_ARRAY_NUM)
					puts("There isn't list of name you enter.");
				else {
					if (list_empty(&listarr[i].list))
						puts("true");
					else
						puts("false");
				}
			}
		}
		else if (!strcmp(comtok[0], "list_size")) {
			if (InputCheck(comtok, 2))
				continue;
			else {
				int i = FindArray(listarr, comtok[1], LIST);
				if (i == MAX_ARRAY_NUM)
					puts("There isn't list of name you enter.");
				else 
					printf("%zd\n", list_size(&listarr[i].list));
			}
		}
		else if (!strcmp(comtok[0], "list_max")) {
			if (InputCheck(comtok, 2))
				continue;
			else {
				int i = FindArray(listarr, comtok[1], LIST);
				if (i == MAX_ARRAY_NUM)
					puts("There isn't list of name you enter.");
				else {
					struct list_elem *e = list_max(&listarr[i].list, ListLessFunction, NULL);
					printf("%d\n", list_entry(e, IntList, elem)->val);
				}
			}
		}
		else if (!strcmp(comtok[0], "list_min")) {
			if (InputCheck(comtok, 2))
				continue;
			else {
				int i = FindArray(listarr, comtok[1], LIST);
				if (i == MAX_ARRAY_NUM)
					puts("There isn't list of name you enter.");
				else {
					struct list_elem *e = list_min(&listarr[i].list, ListLessFunction, NULL);
					printf("%d\n", list_entry(e, IntList, elem)->val);
				}
			}
		}
		else if (!strcmp(comtok[0], "list_remove")) {
			if (InputCheck(comtok, 3))
				continue;
			else {
				int i = FindArray(listarr, comtok[1], LIST);

				if (i == MAX_ARRAY_NUM)
					puts("There isn't list of name you enter.");
				else {
					struct list_elem *e = list_begin(&listarr[i].list);
					for (int j = 0; j < atoi(comtok[2]) && e != list_tail(&listarr[i].list); ++j)
						e = list_next(e);
					if (e != list_tail(&listarr[i].list)) {
						list_remove(e);
						free(list_entry(e, IntList, elem));
					}
				}
			}
		}
		else if (!strcmp(comtok[0], "list_reverse")) {
			if (InputCheck(comtok, 2))
				continue;
			else {
				int i = FindArray(listarr, comtok[1], LIST);
				if (i == MAX_ARRAY_NUM)
					puts("There isn't list of name you enter.");
				else
					list_reverse(&listarr[i].list);
			}
		}
		else if (!strcmp(comtok[0], "list_sort")) {
			if (InputCheck(comtok, 2))
				continue;
			else {
				int i = FindArray(listarr, comtok[1], LIST);
				if (i == MAX_ARRAY_NUM)
					puts("There isn't list of name you enter.");
				else
					list_sort(&listarr[i].list, ListLessFunction, NULL);
			}
		}
		else if (!strcmp(comtok[0], "list_splice")) {
			if (InputCheck(comtok, 6))
				continue;
			
			int num1, num2;

			num1 = FindArray(listarr, comtok[1], LIST);
			num2 = FindArray(listarr, comtok[3], LIST);

			if (num1 == MAX_ARRAY_NUM || num2 == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}

			if (atoi(comtok[4]) >= atoi(comtok[5])) {
				puts("first value should be less than last value");
				continue;
			}

			struct list_elem *before = list_begin(&listarr[num1].list);
			struct list_elem *first, *last; 
			first = last = list_begin(&listarr[num2].list);
			for (int i = 0; i < atoi(comtok[2]) && list_tail(&listarr[num1].list) != before; ++i)
				before = list_next(before);
			for (int i = 0; i < atoi(comtok[4]) && list_tail(&listarr[num2].list) != first; ++i)
				first = list_next(first);
			for (int i = 0; i < atoi(comtok[5]) && list_tail(&listarr[num2].list) != last; ++i)
				last = list_next(last);
			
			list_splice(before, first, last);
		}
		else if (!strcmp(comtok[0], "list_swap")) {
			if (InputCheck(comtok, 4))
				continue;

			int i =FindArray(listarr, comtok[1], LIST);
			struct list_elem *e1, *e2;
			e1 = e2 = list_begin(&listarr[i].list);
			
			for (int j = 0; j < atoi(comtok[2]) && list_tail(&listarr[i].list) != e1; ++j)
				e1 = list_next(e1);
			for (int j = 0; j < atoi(comtok[3]) && list_tail(&listarr[i].list) != e2; ++j)
				e2 = list_next(e2);

			list_swap(e1, e2);
		}
		else if (!strcmp(comtok[0], "list_unique")) {
			comtok[1] = strtok(NULL, " ");
			comtok[2] = strtok(NULL, " ");
			comtok[3] = strtok(NULL, " ");

			if (comtok[3]) {
				puts("There are too many input!");
				continue;
			}

			int num1 = -1, num2 = -1;

			num1 = FindArray(listarr, comtok[1], LIST);
			if (comtok[2])
				num2 = FindArray(listarr, comtok[2], LIST);

			if (num1 == MAX_ARRAY_NUM || num2 == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter!");
				continue;
			}

			if (num2 == -1)
				list_unique(&listarr[num1].list, NULL, ListLessFunction, NULL);
			else
				list_unique(&listarr[num1].list, &listarr[num2].list, ListLessFunction, NULL);
		}
		else if (!strcmp(comtok[0], "list_shuffle")) {
			if (InputCheck(comtok, 2))
				continue;

			int i = FindArray(listarr, comtok[1], LIST);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter!");
				continue;
			}

			list_shuffle(&listarr[i].list);
		}
		else if (!strcmp(comtok[0], "hash_insert")) {
			if (InputCheck(comtok, 3))
				continue;

			int i = FindArray(hasharr, comtok[1], HASH);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter!");
				continue;
			}
			IntHash *new = (IntHash*)malloc(sizeof(IntHash));
			new->val = atoi(comtok[2]);
			hash_insert(&hasharr[i], &new->elem);
		}
		else if (!strcmp(comtok[0], "hash_apply")) {
			if (InputCheck(comtok, 3))
				continue;

			int i = FindArray(hasharr, comtok[1], HASH);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}

			if (!strcmp(comtok[2], "square"))
				hash_apply(&hasharr[i], HashSquare);
			else if (!strcmp(comtok[2], "triple"))
				hash_apply(&hasharr[i], HashTriple);
			else
				puts("hash_apply parameter only can be square, triple.");
		}
		else if (!strcmp(comtok[0], "hash_delete")) {
			if (InputCheck(comtok, 3))
				continue;

			int i = FindArray(hasharr, comtok[1], HASH);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}
			
			IntHash find, *target;
			find.val = atoi(comtok[2]);
			target = hash_entry(hash_delete(&hasharr[i], &find.elem), IntHash, elem);
			if (target)
				free(target);
		}
		else if (!strcmp(comtok[0], "hash_empty")) {
			if (InputCheck(comtok, 2))
				continue;
			int i = FindArray(hasharr, comtok[1], HASH);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}

			if (hash_empty(&hasharr[i]))
				puts("true");
			else
				puts("false");
		}
		else if (!strcmp(comtok[0], "hash_size")) {
			if (InputCheck(comtok, 2))
				continue;
			int i = FindArray(hasharr, comtok[1], HASH);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}

			printf("%zd\n", hash_size(&hasharr[i]));
		}
		else if (!strcmp(comtok[0], "hash_clear")) {
			if (InputCheck(comtok, 2))
				continue;
			int i = FindArray(hasharr, comtok[1], HASH);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}

			hash_clear(&hasharr[i], HashDestructor);
		}
		else if (!strcmp(comtok[0], "hash_find")) {
			if (InputCheck(comtok, 3))
				continue;
			int i = FindArray(hasharr, comtok[1], HASH);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}

			IntHash find;
			find.val = atoi(comtok[2]);

			if (hash_find(&hasharr[i], &find.elem))
				printf("%d\n", find.val);
		}
		else if (!strcmp(comtok[0], "hash_replace")) {
			if (InputCheck(comtok, 3))
				continue;
			int i = FindArray(hasharr, comtok[1], HASH);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}
			struct hash_elem *old;
			IntHash *new = (IntHash*)malloc(sizeof(IntHash));
			new->val = atoi(comtok[2]);
			
			old = hash_replace(&hasharr[i], &new->elem);
			if (old)
				free(hash_entry(old, IntHash, elem));
		}
		else if (!strcmp(comtok[0], "bitmap_set")) {
			if (InputCheck(comtok, 4))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}

			if (bitmap_size(bitarr[i].bitmap) <= (size_t)atoi(comtok[2])) {
				puts("You can't set a bit over the size of bitmap.");
				continue;
			}

			if (!strcmp(comtok[3], "true"))
				bitmap_set(bitarr[i].bitmap, atoi(comtok[2]), true);
			else if (!strcmp(comtok[3], "false"))
				bitmap_set(bitarr[i].bitmap, atoi(comtok[2]), false);
			else
				puts("You can set only true or false value on bitmap.");
		}
		else if (!strcmp(comtok[0], "bitmap_mark")) {
			if (InputCheck(comtok, 3))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}

			if (bitmap_size(bitarr[i].bitmap) <= (size_t)atoi(comtok[2])) {
				puts("You can't set a bit over the size of bitmap.");
				continue;
			}
			
			bitmap_mark(bitarr[i].bitmap, atoi(comtok[2]));
		}
		else if (!strcmp(comtok[0], "bitmap_reset")) {
			if (InputCheck(comtok, 3))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}

			if (bitmap_size(bitarr[i].bitmap) <= (size_t)atoi(comtok[2])) {
				puts("You can't set a bit over the size of bitmap.");
				continue;
			}
			
			bitmap_reset(bitarr[i].bitmap, atoi(comtok[2]));
		}
		else if (!strcmp(comtok[0], "bitmap_all")) {
			if (InputCheck(comtok, 4))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}
			if (bitmap_size(bitarr[i].bitmap) < (size_t)(atoi(comtok[2])+atoi(comtok[3]))) {
				puts("You can't set a bit over the size of bitmap.");
				continue;
			}

			if (bitmap_all(bitarr[i].bitmap, atoi(comtok[2]), atoi(comtok[3])))
				puts("true");
			else
				puts("false");
		}
		else if (!strcmp(comtok[0], "bitmap_none")) {
			if (InputCheck(comtok, 4))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}
			if (bitmap_size(bitarr[i].bitmap) < (size_t)(atoi(comtok[2])+atoi(comtok[3]))) {
				puts("You can't set a bit over the size of bitmap.");
				continue;
			}

			if (bitmap_none(bitarr[i].bitmap, atoi(comtok[2]), atoi(comtok[3])))
				puts("true");
			else
				puts("false");
		}
		else if (!strcmp(comtok[0], "bitmap_any")) {
			if (InputCheck(comtok, 4))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}
			if (bitmap_size(bitarr[i].bitmap) < (size_t)(atoi(comtok[2])+atoi(comtok[3]))) {
				puts("You can't set a bit over the size of bitmap.");
				continue;
			}

			if (bitmap_any(bitarr[i].bitmap, atoi(comtok[2]), atoi(comtok[3])))
				puts("true");
			else
				puts("false");
		}
		else if (!strcmp(comtok[0], "bitmap_contains")) {
			if (InputCheck(comtok, 5))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}
			if (bitmap_size(bitarr[i].bitmap) < (size_t)(atoi(comtok[2])+atoi(comtok[3]))) {
				puts("You can't set a bit over the size of bitmap.");
				continue;
			}
			
			if (!strcmp(comtok[4], "true")) {
				if (bitmap_contains(bitarr[i].bitmap, atoi(comtok[2]), atoi(comtok[3]), true))
					puts("true");
				else
					puts("false");
			}
			else if (!strcmp(comtok[4], "false")) {
				if (bitmap_contains(bitarr[i].bitmap, atoi(comtok[2]), atoi(comtok[3]), false))
					puts("true");
				else
					puts("false");
			}
			else
				puts("You can set only true or false value on bitmap.");
		}
		else if (!strcmp(comtok[0], "bitmap_count")) {
			if (InputCheck(comtok, 5))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}
			if (bitmap_size(bitarr[i].bitmap) < (size_t)(atoi(comtok[2])+atoi(comtok[3]))) {
				puts("You can't set a bit over the size of bitmap.");
				continue;
			}

			if (!strcmp(comtok[4], "true")) 
				printf("%zd\n", bitmap_count(bitarr[i].bitmap, atoi(comtok[2]), atoi(comtok[3]), true));
			else if (!strcmp(comtok[4], "false")) 
				printf("%zd\n", bitmap_count(bitarr[i].bitmap, atoi(comtok[2]), atoi(comtok[3]), false));
		}
		else if (!strcmp(comtok[0], "bitmap_expand")) {
			if (InputCheck(comtok, 3))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}

			bitarr[i].bitmap = bitmap_expand(bitarr[i].bitmap, atoi(comtok[2]));
		}
		else if (!strcmp(comtok[0], "bitmap_set_all")) {
			if (InputCheck(comtok, 3))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}

			if (!strcmp(comtok[2], "true"))
				bitmap_set_all(bitarr[i].bitmap, true);
			else if (!strcmp(comtok[2], "false"))
				bitmap_set_all(bitarr[i].bitmap, false);
			else
				puts("You can set only true or false value on bitmap.");
		}
		else if (!strcmp(comtok[0], "bitmap_flip")) {
			if (InputCheck(comtok, 3))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}
			if (bitmap_size(bitarr[i].bitmap) <= (size_t)atoi(comtok[2])) {
				puts("You can't flip a bit over the size of bitmap.");
				continue;
			}

			bitmap_flip(bitarr[i].bitmap, atoi(comtok[2]));
		}
		else if (!strcmp(comtok[0], "bitmap_scan")) {
			if (InputCheck(comtok, 5))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}
			if (bitmap_size(bitarr[i].bitmap) <= (size_t)atoi(comtok[2])) {
				puts("You can't flip a bit over the size of bitmap.");
				continue;
			}

			if (!strcmp(comtok[4], "true"))
				printf("%u\n", (unsigned int)bitmap_scan(bitarr[i].bitmap, atoi(comtok[2]), atoi(comtok[3]), true));
			else if (!strcmp(comtok[4], "false"))
				printf("%u\n", (unsigned int)bitmap_scan(bitarr[i].bitmap, atoi(comtok[2]), atoi(comtok[3]), false));
			else
				puts("You can set only true or false value on bitmap.");
		}
		else if (!strcmp(comtok[0], "bitmap_scan_and_flip")) {
			if (InputCheck(comtok, 5))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}
			if (bitmap_size(bitarr[i].bitmap) <= (size_t)atoi(comtok[2])) {
				puts("You can't flip a bit over the size of bitmap.");
				continue;
			}

			if (!strcmp(comtok[4], "true"))
				printf("%u\n", (unsigned int)bitmap_scan_and_flip(bitarr[i].bitmap, atoi(comtok[2]), atoi(comtok[3]), true));
			else if (!strcmp(comtok[4], "false"))
				printf("%u\n", (unsigned int)bitmap_scan_and_flip(bitarr[i].bitmap, atoi(comtok[2]), atoi(comtok[3]), false));
			else
				puts("You can set only true or false value on bitmap.");
		}
		else if (!strcmp(comtok[0], "bitmap_set_multiple")) {
			if (InputCheck(comtok, 5))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}
			if (bitmap_size(bitarr[i].bitmap) <= (size_t)atoi(comtok[2])) {
				puts("You can't flip a bit over the size of bitmap.");
				continue;
			}

			if (!strcmp(comtok[4], "true"))
				bitmap_set_multiple(bitarr[i].bitmap, atoi(comtok[2]), atoi(comtok[3]), true);
			else if (!strcmp(comtok[4], "false"))
				bitmap_set_multiple(bitarr[i].bitmap, atoi(comtok[2]), atoi(comtok[3]), false);
			else
				puts("You can set only true or false value on bitmap.");
		}
		else if (!strcmp(comtok[0], "bitmap_size")) {
			if (InputCheck(comtok, 2))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}

			printf("%zd\n", bitmap_size(bitarr[i].bitmap));
		}
		else if (!strcmp(comtok[0], "bitmap_test")) {
			if (InputCheck(comtok, 3))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}
			if (bitmap_size(bitarr[i].bitmap) <= (size_t)atoi(comtok[2])) {
				puts("You can't flip a bit over the size of bitmap.");
				continue;
			}

			if (bitmap_test(bitarr[i].bitmap, atoi(comtok[2])))
				puts("true");
			else
				puts("false");
		}
		else if (!strcmp(comtok[0], "bitmap_dump")) {
			if (InputCheck(comtok, 2))
				continue;
			int i = FindArray(bitarr, comtok[1], BITMAP);
			if (i == MAX_ARRAY_NUM) {
				puts("There isn't list of name you enter.");
				continue;
			}

			bitmap_dump(bitarr[i].bitmap);
		}
	}

	return 0;
}

int FindArray(void* arr, char *name, int data) {
	int i;
	if (data == LIST) {
		for (i = 0; i < MAX_ARRAY_NUM; ++i)
			if (!strcmp(((ListList*)arr)[i].name, name))
				break; 
	}
	else if (data == BITMAP) {
		for (i = 0; i < MAX_ARRAY_NUM; ++i)
			if (!strcmp(((BitList*)arr)[i].name, name))
				break; 
	}
	else if (data == HASH) {
		for (i = 0; i < MAX_ARRAY_NUM; ++i)
			if (((struct hash*)arr)[i].aux)
				if (!strcmp((char*)((struct hash*)arr)[i].aux, name))
					break;
	}
	else {
		puts("Data Parameter Value error!");
		return -1;
	}

	return i;
}
bool ListLessFunction(const struct list_elem *a, const struct list_elem *b, void* aux) {
	IntList *tempa = list_entry(a, IntList, elem);
	IntList *tempb = list_entry(b, IntList, elem);

	if (tempa->val < tempb->val)
		return true;
	else
		return false;
}
void list_swap(struct list_elem *a, struct list_elem *b) {
	if (a->prev == NULL || a->next == NULL || b->prev == NULL || b->next == NULL) {
		puts("head or tail can't be swapted!");
		return;
	}

	if (a == b)
		return;

	IntList *e1 = list_entry(a, IntList, elem);
	IntList *e2 = list_entry(b, IntList, elem);

	int temp = e1->val;
	e1->val = e2->val;
	e2->val = temp;
}
void list_shuffle(struct list *list) {
	int *arr;
	int list_length = 0;
	struct list_elem *e = list_begin(list);
	int i = 0;
	int first, last, temp;

	while (e != list_tail(list)) {
		e = list_next(e);
		++list_length;
	}

	arr = (int*)malloc(sizeof(int) * list_length);

	e = list_begin(list);
	while (e != list_tail(list)) {
		arr[i++] = list_entry(e, IntList, elem)->val;
		e = list_next(e);
	}

	srand(time(NULL));

	for (i = 0 ; i < 100; ++i) {
		first = rand() % list_length; last = rand() % list_length;
		temp = arr[first];
		arr[first] = arr[last];
		arr[last] = temp;
	}
	
	i = 0;
	e = list_begin(list);
	while (e != list_tail(list)) {
		list_entry(e, IntList, elem)->val = arr[i++];
		e = list_next(e);
	}

	free(arr);
}
int InputCheck(char** p, int num) {
	for (int i = 1; i <= num; ++i)
		p[i] = strtok(NULL, " ");

	if (p[num]) {
		puts("There are too many inputs!");
		return 1;
	}

	for (int i = 1; i < num; ++i) {
		if (!p[i]) {
			puts("There are insufficient inputs!");
			return 1;
		}
	}

	return 0;
}
unsigned hash_int_2(int i) {
	return (unsigned)(i % 4);
}
bool HashLessFunction(const struct hash_elem *a, const struct hash_elem *b, void *aux) {
	if (hash_entry(a, IntHash, elem)->val < hash_entry(b, IntHash, elem)->val) 
		return true;
	else
		return false;
}
unsigned HashFunction (const struct hash_elem *e, void *aux) {
	return hash_int(hash_entry(e, IntHash, elem)->val);
}
void HashDestructor (struct hash_elem *e, void *aux) {
	free(hash_entry(e, IntHash, elem));
}
void HashSquare (struct hash_elem *e, void *aux) {
	IntHash *temp = hash_entry(e, IntHash, elem);
	temp->val *= temp->val;
}
void HashTriple (struct hash_elem *e, void *aux){
	IntHash *temp = hash_entry(e, IntHash, elem);
	temp->val *= temp->val * temp->val;
}
struct bitmap *bitmap_expand(struct bitmap *bitmap, int size) {
	struct bitmap *new;

	new = bitmap_create(size + bitmap_size(bitmap));

	if (!new)
		return NULL;

	for (int i = 0; i < (int)bitmap_size(bitmap); ++i) {
		if (bitmap_test(bitmap, i))
			bitmap_mark(new, i);
	}

	bitmap_destroy(bitmap);

	return new;
}
