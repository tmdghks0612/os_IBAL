#include "a.h"

int main() {
	char user_command[300]; // user command
	char *comtok[10]; // tockenized user commands
	int list_num = 0, hash_num = 0, bit_num = 0;
	ListList listarr[10];
	HashList hasharr[10];
	BitList bitlist[10];

	while (1) {
		printf("Input : ");
		fgets(user_command, 300, stdin);
		user_command[strlen(user_command)-1] = '\0';
		comtok[0] = strtok(user_command, " ");

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
				int i;
				for (i = 0; i < list_num; ++i)
					if (!strcmp(listarr[i].name, comtok[1]))
						break; 
				if (i != list_num) { // dumpadata list
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
			int i;
				for (i = 0; i < list_num; ++i)
					if (!strcmp(listarr[i].name, comtok[1]))
						break; 
				if (i != list_num) { // delete list
					IntList* temp;
					for (struct list_elem *e = list_begin(&(listarr[i].list)); e != list_end(&(listarr[i].list)); e = list_next(e)) {
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
				int i;
				for (i = 0 ; i < list_num; ++i)
					if (!strcmp(listarr[i].name, comtok[1]))
						break;
				
				if (i == list_num) // 해당 이름의 list가 없을 경우
					puts("There is not list of name you enter!");
				else {
					struct list_elem* e = list_begin(&(listarr[i].list));
					IntList *temp = (IntList*)malloc(sizeof(IntList));
					for (int j = 0; j < atoi(comtok[2]); ++j) {
						if (e != list_end(&(listarr[i].list)))
							list_next(e);
						else
							break;
					}
					
					temp->val = atoi(comtok[3]);
					list_insert(e, &(temp->elem));
				}
			}
		}
	}

	return 0;
}
