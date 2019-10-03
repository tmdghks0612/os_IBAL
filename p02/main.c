#include "main.h"

int main() {
	char user_command[300]; // user command
	char *comtok[10]; // tockenized user commands
	int list_num = 0, hash_num = 0, bit_num = 0;
	ListList listarr[MAX_ARRAY_NUM];
	HashList hasharr[MAX_ARRAY_NUM];
	BitList bitlist[MAX_ARRAY_NUM];

	while (1) {
		printf("Input : ");
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

			if (comtok[3]) // 입력 값이 많은 경우
				puts("Input is wrong! Too many input value");
			else if (!comtok[2])
				printf("Enter Name of %s\n", comtok[1]);
			else if (!strcmp(comtok[1], "list")) {
				list_init(&(listarr[list_num].list));
				strncpy(listarr[list_num].name, comtok[2], 20);
				++list_num;
			}
			else if (!strcmp(comtok[1], "hashtable")) {

			}
			else if (!strcmp(comtok[1], "bitmap")) {

			}
			else
				printf("We can't make this type : %s\n", comtok[1]);
		}
		else if (!strcmp(comtok[0], "dumpdata")) {
			comtok[1] = strtok(NULL, " ");
			comtok[2] = strtok(NULL, " ");

			if (comtok[2])
				puts("Input is wrong! Too many input value");
			else {
				int i = FindArray((void*)listarr, comtok[1], LIST);

				if (i != MAX_ARRAY_NUM) { // dumpadata list
					IntList *temp;
					for (struct list_elem* e = list_begin (&(listarr[i].list)); e != list_end (&(listarr[i].list)); e = list_next (e)) {
						temp = list_entry (e, IntList, elem);
						printf("%d ", temp->val);
					}
					puts("");
				}
				else {

				}
			}
		}
		else if (!strcmp(comtok[0], "delete")) {
			comtok[1] = strtok(NULL, " ");
			comtok[2] = strtok(NULL, " ");

			if (comtok[2])
				puts("Input is wrong! Too many input value");

			int i = FindArray((void*)listarr, comtok[1], LIST);
			if (i != MAX_ARRAY_NUM) { // delete list
				IntList* temp;
				struct list_elem *e;
				while (!list_empty(&(listarr[i].list))) {
					e = list_begin(&(listarr[i].list));
					list_remove(e);
					temp = list_entry(e, IntList, elem);
					free(temp);
				}
				for (int j = i; j < list_num-1; ++j)
					listarr[j] = listarr[j+1];
				list_num--;
			}
			else {

			}
		}
		else if (!strcmp(comtok[0], "list_insert")) {
			comtok[1] = strtok(NULL, " ");
			comtok[2] = strtok(NULL, " ");
			comtok[3] = strtok(NULL, " ");
			comtok[4] = strtok(NULL, " ");

			if (comtok[4])
				puts("Input is wrong! Too many input value");
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
			comtok[1] = strtok(NULL, " ");
			comtok[2] = strtok(NULL, " ");
			comtok[3] = strtok(NULL, " ");

			if (comtok[3])
				puts("There are too many parameter!");
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
			comtok[1] = strtok(NULL, " ");
			comtok[2] = strtok(NULL, " ");
			comtok[3] = strtok(NULL, " ");

			if (comtok[3])
				puts("There are too many parameter!");
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
			comtok[1] = strtok(NULL, " ");
			comtok[2] = strtok(NULL, " ");

			if (comtok[2])
				puts("There are too many parameter!");
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
			comtok[1] = strtok(NULL, " ");
			comtok[2] = strtok(NULL, " ");

			if (comtok[2])
				puts("There are too many parameter!");
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
			comtok[1] = strtok(NULL, " ");
			comtok[2] = strtok(NULL, " ");

			if (comtok[2])
				puts("There are too many parameter!");
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
			comtok[1] = strtok(NULL, " ");
			comtok[2] = strtok(NULL, " ");

			if (comtok[2])
				puts("There are too many parameter!");
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
	}

	return 0;
}

int FindArray(void* arr, char *name, int data) {
	int i;
	if (data == LIST) {
		for (i = 0; i < 10; ++i)
			if (!strcmp(((ListList*)arr)[i].name, name))
				break; 
	}
	else if (data == BITMAP) {
		for (i = 0; i < 10; ++i)
			if (!strcmp(((BitList*)arr)[i].name, name))
				break; 
	}
	else if (data == HASH) {
		for (i = 0; i < 10; ++i)
			if (!strcmp(((HashList*)arr)[i].name, name))
				break; 
	}
	else {
		puts("Data Parameter Value error!");
		return -1;
	}

	return i;
}