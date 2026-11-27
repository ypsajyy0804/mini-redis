#include "redis.h"
#include <stdio.h>
#include <string.h>

#define CHECK(cond)\
	checks++;\
	do{\
		if(!(cond)) {\
			printf("%s:%d 失败: %s\n", __FILE__, __LINE__, #cond);\
			failed++;\
		}\
	}while(0)

int main() {
	int failed = 0, checks = 0;
	Dict *d = dict_create();
	CHECK(d != NULL);

	CHECK(dict_set(d, "a", "1") == 1);

	const char *v = dict_get(d, "a");
	//这里让我拆开
	CHECK(v != NULL);
	CHECK(strcmp(v, "1") == 0);

	v = dict_get(d, "b");
	CHECK(v == NULL);

	CHECK(dict_set(d, "a", "2") == 0);

	v = dict_get(d, "a");
	CHECK(v != NULL && strcmp(v, "2") == 0);

	CHECK(dict_del(d, "a") == 1);

	v = dict_get(d, "a");
	CHECK(v == NULL);

	CHECK(dict_del(d, "a") == 0);

	char key[16];
	char val[16];

	int sum1 = 0;
	for(int i = 1; i <= 100; i++) {
		snprintf(key, sizeof(key), "k%d", i);
		snprintf(val, sizeof(val), "v%d", i);
		sum1 += dict_set(d, key, val);
	}
	CHECK(sum1 == 100);

	int sum2 = 0;
	for(int i = 1; i <= 100; i++){
		snprintf(key, sizeof(key), "k%d", i);
		snprintf(val, sizeof(val), "v%d", i);
		const char *v = dict_get(d, key);
		if(v == NULL || strcmp(v, val) != 0) {
			sum2++;
		}
	}
	CHECK(sum2 == 0);

	int sum3 = 0;
	for(int i = 1; i <= 100; i++){
		snprintf(key, sizeof(key), "k%d", i);
		snprintf(val, sizeof(val), "v%d", i);
		sum3 += dict_del(d, key);
	}
	CHECK(sum3 == 100);

	int sum4 = 0;
	for(int i = 1; i <= 100; i++){
		snprintf(key, sizeof(key), "k%d", i);
		snprintf(val, sizeof(val), "v%d", i);
		const char *v = dict_get(d, key);
		if(v != NULL) sum4++;
	}
	CHECK(sum4 == 0);

	dict_free(d);
	printf("%d checks, %d failed\n", checks, failed);

	return failed;
}
