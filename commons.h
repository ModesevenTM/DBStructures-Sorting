#pragma once

#define BLOCK_SIZE 10 // should be amount of bytes in a block, not the amount of records

struct Stats {
	int reads = 0;
	int writes = 0;
	int sortings = 0;
	int phases = 0;
	int dummyRuns = 0;
	int tapeWithDummies = -1;
};