#pragma once
#include <string>
#include <iostream>
#include "commons.h"
#include "functions.h"
#include "InputBuffer.h"
#include "OutputBuffer.h"

class Polyphase
{
public:
	Polyphase();
	~Polyphase();

	int outTape = 2;
	int longerTape = 0;
	bool printContent = false;

	Stats stats;

	void distribute();
	void merge();
};

