#include "OutputBuffer.h"

OutputBuffer::OutputBuffer(std::string f, Stats* s, bool print)
{
	buffer = new Record[BLOCK_SIZE];
	filename = f;
	stats = s;
	printContents = print;
	std::ofstream file(filename, std::ios::out | std::ios::trunc);
	file.close();
}

OutputBuffer::~OutputBuffer()
{
	writeBuffer();
	delete[] buffer;
}

void OutputBuffer::putRecord(Record* record)
{
	if (recordAmount == BLOCK_SIZE)
	{
		writeBuffer();
		recordAmount = 0;
	}

	buffer[recordAmount++] = *record;
}

void OutputBuffer::writeBuffer()
{
	if (recordAmount)
	{
		std::ofstream file(filename, std::ios::out | std::ios::binary | std::ios::app);
		if (!file.is_open())
			std::cout << "Error opening file" << std::endl;
		else {
			for (int i = 0; i < recordAmount; i++)
			{
				if(printContents) buffer[i].print();
				file.write((char*)&buffer[i].prob1, sizeof(double));
				file.write((char*)&buffer[i].prob2, sizeof(double));
				file.write((char*)&buffer[i].probSum, sizeof(double));
			}
			file.close();
			stats->writes++;
		}
	}
}
	