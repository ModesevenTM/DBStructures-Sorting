#pragma once
#include "InputBuffer.h"
#include "OutputBuffer.h"
#include "Record.h"

struct RecordInfo
{
public:
	InputBuffer* in = nullptr;
	OutputBuffer* out = nullptr;
	Record prev = Record(-1.0f);
	Record* now = nullptr;

	void reset();
	void fetchRecord();
	void putRecord();
	void putRest();
	bool seriesEnd();
};

