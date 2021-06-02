#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "diff.h"

void usage_error(char *msg) {
	fprintf(stderr, msg, progName);
	usage();
	exit(1);
}

void usage(void) {
	fprintf(stderr,
		"Usage:\n"
		"\t%s [--help/-h]\n"
		"\t%s [--exclude/-x excludefile] [--log/-l] file1 file2\n",
	progName, progName);
	exit(1);
}

FILE *load_file(char *filename, const char *mode) {
	FILE *ptr = fopen(filename, mode);

	if (ptr == NULL) {
		fprintf(stderr, "%s: file %s could not be opened\n", progName, filename);
		exit(1);
	}

	return ptr;
}
