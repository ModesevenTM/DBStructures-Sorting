#include "functions.h"

void generateRandom(int n)
{
	Record record;
	std::ofstream file("t3.tap", std::ios::out | std::ios::binary | std::ios::trunc);
	for (int i = 0; i < n; i++)
	{
		record.randomize();
		file.write((char*)&record.prob1, sizeof(double));
		file.write((char*)&record.prob2, sizeof(double));
		file.write((char*)&record.probSum, sizeof(double));
	}
	file.close();
}

void fileReadTest(Record* records)
{
	std::ifstream file("t3.tap", std::ios::in | std::ios::binary);
	double p1, p2, pS;
	int i = 0;
	while (file.read((char*)&p1, sizeof(double)))
	{
		file.read((char*)&p2, sizeof(double));
		file.read((char*)&pS, sizeof(double));
		records[i++] = Record(p1, p2, pS);
	}
	file.close();
}

void printFile(std::string f)
{
	std::ifstream file("t3.tap", std::ios::in | std::ios::binary);
	double p1, p2, pS;
	int i = 0;
	while (file.read((char*)&p1, sizeof(double)))
	{
		file.read((char*)&p2, sizeof(double));
		file.read((char*)&pS, sizeof(double));
		Record r = Record(p1, p2, pS);
		r.print();
	}
	file.close();
}