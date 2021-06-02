#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#include "diff.h"

char *progName = NULL;

struct option st_options[] = {
	{ "help",	no_argument,    	NULL,	'h' },
	{ "log",	no_argument,    	NULL,	'l' },
	{ "exclude",	required_argument,	NULL,	'x' }
};

bool printDiffs = false;

int main(int argc, char **argv) {
	progName = argv[0];

	char *excFileName = NULL;

	char curOpt;
	int optIndex;

	while ((curOpt = getopt_long(argc, argv, "hlx:", st_options, &optIndex)) != -1) {
		switch (curOpt) {
			case 'x':
				excFileName = optarg;
				break;

			case 'l':
				printDiffs = true;
				break;

			case 'h':
			case '?':
			default:
				usage();
				break;
		}
	}

	if (optind > argc - 2) {
		usage_error("%s: not enough input files given\n");
	}

	if (optind < argc - 2) {
		usage_error("%s: too many input files given\n");
	}

	struct ExcludeRange *ranges;

	if (excFileName != NULL) {
		unsigned int buf;
		unsigned int numRanges;

		FILE *excFile = load_file(excFileName, "rb");

		fread(&buf, 1, 4, excFile);

		if (buf != 0xED17BA68) {
			fprintf(stderr, "%s: invalid exclude file", progName);
			return 1;
		}

		fread(&numRanges, 1, 4, excFile);

		ranges = malloc(numRanges * sizeof(struct ExcludeRange));

		for (int i = 0; i < numRanges; i++) {
			fread(&ranges[i], 1, sizeof(struct ExcludeRange), excFile);
		}

		fclose(excFile);
	} else {
		ranges = malloc(sizeof(struct ExcludeRange));

		ranges[0].addr = 0xFFFFFFFF;
		ranges[0].size = 0xFFFFFFFF;
	}
for (int i = 0; ranges[i].addr != 0xFFFFFFFF; i++) printf("%08x %08x\n", ranges[i].addr, ranges[i].size);
	int diffs = 0;

	unsigned long long int buf1;
	unsigned long long int buf2;

	gzFile file1 = gzopen(argv[optind++], "rb");
	gzFile file2 = gzopen(argv[optind++], "rb");

	gzseek(file1, 0x1b0, SEEK_SET);
	gzseek(file2, 0x1b0, SEEK_SET);

	unsigned int addr;
	unsigned int size;

	for (unsigned int i = 0x80000000, curRange = 0; i < 0x80800000; (i += sizeof(buf1)), (i >= addr + size ? curRange++ : 0)) {
		addr = ranges[curRange].addr;
		size = ranges[curRange].size;

		if (addr < i && i < addr + size) {
			continue;
		}

		gzread(file1, &buf1, sizeof(buf1));
		gzread(file2, &buf2, sizeof(buf2));

		if (buf1 != buf2) {
			diffs++;

			if (printDiffs) {
				printf("Difference at 0x%08x: %016llx became %016llx\n", i, buf1, buf2);
			}
		}
	}

	gzclose(file1);
	gzclose(file2);

	free(ranges);

	printf("Differences: %d\n", diffs);
}
