#include <stdlib.h>

int main() {
	int *p = malloc(4 * sizeof(int));
	free(p);
	p[0] = 2;
	return 0;
}
