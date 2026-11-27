#include "redis.h"
#include <stdio.h>
#include <string.h>

static int cmd_set(Dict *d, int argc, char **argv, char *out, size_t outcap) {
	if(argc != 3) {
		int n = snprintf(out, outcap, "%s", "ERR wrong number of arguments for 'set' command");
		
		if (n < 0 || (size_t)n >= outcap) return CMD_ERR_OUTFULL;
		
		return CMD_ERR_ARGS;
	}

	if(dict_set(d, argv[1], argv[2]) == -1) {
		int n = snprintf(out, outcap, "%s", "ERR out of memory");
		if (n < 0 || (size_t)n >= outcap) return CMD_ERR_OUTFULL;
		return CMD_ERR_NOMEM;
	}

	int n = snprintf(out, outcap, "%s", "OK");
	if (n < 0 || (size_t)n >= outcap) return CMD_ERR_OUTFULL;

	return CMD_OK;
}

static int cmd_get(Dict *d, int argc, char **argv, char *out, size_t outcap) {
	if(argc != 2) {
		int n = snprintf(out, outcap, "%s", "ERR wrong number of arguments for 'get' command");
		if (n < 0 || (size_t)n >= outcap) return CMD_ERR_OUTFULL;
		return  CMD_ERR_ARGS;
	}

	const char *val = dict_get(d, argv[1]);

	if(val == NULL) {
		int n = snprintf(out, outcap, "%s", "(nil)");
		if (n < 0 || (size_t)n >= outcap) return CMD_ERR_OUTFULL;
		return CMD_OK;
	}

	int n = snprintf(out, outcap, "%s", val);
	if (n < 0 || (size_t)n >= outcap) return CMD_ERR_OUTFULL;

	return CMD_OK;
}

int cmd_exec(Dict *d, int argc, char **argv, char* out, size_t outcap) {
	if(strcasecmp(argv[0], "SET") == 0) 
		return cmd_set(d, argc, argv, out, outcap);

	if(strcasecmp(argv[0], "GET") == 0)
		return cmd_get(d, argc, argv, out, outcap);

	int n = snprintf(out, outcap, "%s %s", "ERR unknown command", argv[0]);
	if (n < 0 || (size_t)n >= outcap) return CMD_ERR_OUTFULL;

	return CMD_ERR_UNKNOWN;
}
