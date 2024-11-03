#pragma once
#include "Record.h"
#include "commons.h"
#include <fstream>
#include <iostream>

class OutputBuffer
{
public:
	std::string filename;
	Record* buffer;
	Stats* stats;
	int recordAmount = 0;
	int lastWritten = 0;
	bool printContents = true;

	OutputBuffer(std::string f, Stats* s, bool print = true);
	~OutputBuffer();

	void putRecord(Record* record);
	void writeBuffer();
};

