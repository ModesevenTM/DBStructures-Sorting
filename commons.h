#pragma once

const int RECORD_SIZE = 3 * sizeof(double);	// size of a record in bytes
const int BLOCK_SIZE = 960;					// size of a block in bytes

struct Stats {
	int reads = 0;
	int writes = 0;
	int dummyRuns = 0;
};