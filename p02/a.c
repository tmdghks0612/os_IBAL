#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"
#include "debug.h"
#include "list.h"
#include "round.h"
#include "limits.h"

int main() {
	char command[300];

	while (1) {
		scanf("%s", command);
		
		if (!strcmp(command, "quit"))
			break;
	}

	return 0;
}
