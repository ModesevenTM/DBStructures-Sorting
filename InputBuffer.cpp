#include "InputBuffer.h"

InputBuffer::InputBuffer(std::string f, Stats* s)
{
	buffer = new Record[BLOCK_SIZE / RECORD_SIZE];
	filename = f;
	stats = s;
}

InputBuffer::~InputBuffer()
{
	delete[] buffer;
}

Record* InputBuffer::fetchRecord()
{
	if (currentIdx == recordAmount)
	{
		currentIdx = 0;
		recordAmount = 0;
		std::ifstream file(filename, std::ios::in | std::ios::binary);
		if (!file.is_open())
		{
			std::cout << "Error opening file" << std::endl;
			return nullptr;
		}

		file.seekg(0, std::ios::end);
		if (file.tellg() == lastRead * 3 * sizeof(double))
		{
			file.close();
			return nullptr;
		}
		file.seekg(lastRead * 3 * sizeof(double), std::ios::beg);

		stats->reads++;

		while (file.peek() != EOF && recordAmount < BLOCK_SIZE / RECORD_SIZE)
		{
			file.read((char*)&buffer[recordAmount].prob1, sizeof(double));
			file.read((char*)&buffer[recordAmount].prob2, sizeof(double));
			file.read((char*)&buffer[recordAmount].probSum, sizeof(double));
			buffer[recordAmount].probProd = buffer[recordAmount].prob1 + buffer[recordAmount].prob2 - buffer[recordAmount].probSum;
			recordAmount++;
			lastRead++;
		}

		file.close();
	}
	return &buffer[currentIdx++];
}