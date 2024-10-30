#pragma once
#include "Record.h"
#include "commons.h"
#include <fstream>
#include <iostream>

class InputBuffer
{
public:
	std::string filename;
	Record* buffer;
	int currentIdx = 0;
	int recordAmount = 0;
	int lastRead = 0;

	InputBuffer(std::string f);
	~InputBuffer();

	Record* fetchRecord();
};

