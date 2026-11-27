#include "redis.h"
#include <stdio.h>
#include <string.h>

int main() {
	char line[1024];
	Dict *d = dict_create();

	if(d == NULL) {
		perror("dict_create");
		return 1;
	}

	while(fgets(line, sizeof line, stdin) != NULL) {
		size_t pos = strcspn(line, "\n");  //在line中找到换行符的下标
		size_t len = strlen(line);  

		line[pos] = '\0';  //将换行符改为'\0'

		if(pos == len && !feof(stdin) && !ferror(stdin)) {  //没有读到换行符，行超长开始下一次循环
			while(fgets(line, sizeof line, stdin)) {
				pos = strcspn(line, "\n");
				len = strlen(line);
				if(pos == len && !feof(stdin) && !ferror(stdin)) continue;
				else break;
			}

			continue;
		}

		if(line[0] == '\0') continue; //行为空，进入下一循环

		int argc = 0;
		char *argv[16];
		char *p = line;  //p 指向 line 的第一个字符

		while (*p != '\0' && argc < 16) {
			if(*p == ' ' || *p == '\t') {
				p++;
				continue;
			}

			argv[argc++] = p;  //保存当前单词首字符的地址，但改单词会从存的字符地址一直读到结尾的'\0' 

			while (*p !='\0' && *p != ' ' && *p != '\t') {
				p++;
			}

			//因此需要在单词的后面加上一个'\0'来框定一个词
			if(*p != '\0') {
				*p = '\0';
				p++;
			}
		}

		if(argc == 0) continue;

		char out[128];

		if(cmd_exec(d, argc, argv, out, sizeof out) == CMD_ERR_OUTFULL) printf("ERR output too long\n");
		else printf("%s\n", out);
	}

	dict_free(d);

	return 0;
}
