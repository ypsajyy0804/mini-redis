#include "redis.h"
#include <stdio.h>

int main() {
	Dict *d = dict_create();

	dict_set(d, "a", "1");

	dict_set(d, "a", "2");

	dict_free(d);

	return 0;
}
