// main.c
// Verwendet die externe Funktion `add` aus add.c

#include <stdio.h>

// extern informiert den Compiler, dass die Definition von `add` woanders existiert

extern int add(int, int);

int main(void) {
	int sum = add(5, 7);
	printf("5 + 7 = %d\n", sum);
	return 0;
}
