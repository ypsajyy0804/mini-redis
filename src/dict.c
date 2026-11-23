#include "redis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dict {
	struct Node {
		char *key;
		char *val;
		struct Node *next;
	} **buckets;
	unsigned bucket_count;
	unsigned size;
} Dict;

Dict *dict_creat(void) {
	Dict *d = malloc(sizeof(Dict));

	if(d == NULL) {
		return NULL;
	}

	d->buckets = calloc(8, sizeof(struct Node*));

	if(d->buckets == NULL) {
		free(d);

		return NULL;
	}

	d->bucket_count = 8;

	d->size = 0;

	return d;
}

void dict_free(Dict *d) {
	if(d == NULL) {
		return;
	}

	for(unsigned i = 0; i < d->bucket_count; i++) {
		struct Node *cur = d->buckets[i];

		while(cur != NULL) {
			free(cur->key);
			free(cur->val);

			struct Node* temp = cur;
			cur = cur->next;
			free(temp);
		}
	}
	free(d->buckets);
	free(d);
}

unsigned hash_val(const Dict *d, const char *key) {
	int sum = 0;
	for(const char *p = key; *p != '\0'; p++) {
		sum += *p;
	}
	unsigned num = sum % d->bucket_count;

	return num;
}

int dict_set(Dict *d, const char *key, const char *val) {
	unsigned num = hash_val(d, key);

	struct Node *cur = d->buckets[num];

	while(cur != NULL) {
		if(strcmp(key, cur->key) == 0) {
			cur->val = strdup(val);
			return 0;
		}

		if(cur->next == NULL) break;

		cur = cur->next;
	}

	struct Node *newnode = malloc(sizeof(struct Node));

	if(newnode == NULL) return -1;

	newnode->key = strdup(key);

	if(newnode->key == NULL) {
		perror("strdup");
		exit(1);
	}

	newnode->val = strdup(val);
	newnode->next = NULL;

	if(cur == NULL) d->buckets[num] = newnode;

	if(cur != NULL) cur->next = newnode;

	d->size++;

	return 1;
}

const char *dict_get(const Dict *d, const char *key) {
	unsigned num = hash_val(d, key);

	struct Node *cur = d->buckets[num];

	while(cur != NULL) {
		if(strcmp(key, cur->key) == 0) {
			return cur->val;
		}

		cur = cur->next;
	}

	return NULL; 
}

int dict_del(Dict *d, const char *key) {
	unsigned num = hash_val(d, key);
	struct Node *head = d->buckets[num];
	if(head == NULL) return 0;

	if(strcmp(key, head->key) == 0) {
		d->buckets[num] = head->next;

		free(head->key);
		free(head->val);
		free(head);

		d->size--;

		return 1;
	}

	struct Node *prev = head;

	struct Node *cur = prev->next;

	while(cur != NULL) {
		if(strcmp(key,cur->key) == 0) {
			prev->next = cur->next;

			free(cur->key);
			free(cur->val);
			free(cur);

			d->size--;

			return 1;
		}
		prev = cur;
		cur = cur->next;
	}
	return 0;
}

//int cmd_exec(Dict *d, int argc, char **argv, char *out, size_t outcap) {}
