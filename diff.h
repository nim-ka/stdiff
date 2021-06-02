#pragma once

#include <stdbool.h>
#include <stdio.h>

struct ExcludeRange {
	unsigned int addr;
	unsigned int size;
};

extern char *progName;

extern void usage_error(char *);
extern void usage(void);

extern FILE *load_file(char *, const char *);
