#include <stddef.h>

#ifndef REDIS_H
#define REDIS_H

#define CMD_OK 0
#define CMD_ERR_ARGS 1
#define CMD_ERR_UNKNOWN 2
#define CMD_ERR_NOMEM 3
#define CMD_ERR_OUTFULL 4

typedef struct Dict Dict;

//创建键值对字典，返回指向结构体字典的指针
Dict *dict_create(void);

//释放内存
void dict_free(Dict *d);

//哈希函数计算哈希值
unsigned hash_val(const Dict *d, const char *key);

//执行插入，返回状态码，成功为 1，失败为 0；
int dict_set(Dict *d, const char *key, const char *val);

//返回的 const char * 为借出的值，不可修改
const char *dict_get(const Dict *d, const char *key);

//执行删除，返回状态码，成功为 1，失败为 0；
int dict_del(Dict *d, const char *key);

//
int cmd_exec(Dict *d, int argc, char **argv, char *out, size_t outcap);


#endif
