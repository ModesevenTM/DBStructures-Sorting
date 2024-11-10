#pragma once

#define RECORD_SIZE 3 * sizeof(double)  // size of a record in bytes
#define BLOCK_SIZE	1024				// size of a block in bytes

struct Stats {
	int reads = 0;
	int writes = 0;
	int sortings = 0;
	int phases = 0;
	int dummyRuns = 0;
	int tapeWithDummies = -1;
};