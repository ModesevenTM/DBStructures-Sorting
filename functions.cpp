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

void createRecords(int n)
{
	Record record;
	std::ofstream file("t3.tap", std::ios::out | std::ios::binary | std::ios::trunc);
	for (int i = 0; i < n; i++)
	{
		std::cout << "Enter probabilities so that their sum is between 0 and 1.\n";
		while (true)
		{
			std::cout << "Enter probability 1: ";
			std::cin >> record.prob1;
			if (record.prob1 < 0 || record.prob1 > 1)
				std::cout << "Invalid value. Try again.\n";
			else break;
		}

		while (true)
		{
			std::cout << "Enter probability 2: ";
			std::cin >> record.prob2;
			if (record.prob2 < 0 || record.prob2 > 1)
				std::cout << "Invalid value. Try again.\n";
			else break;
		}

		while (true)
		{
			std::cout << "Enter probability sum: ";
			std::cin >> record.probSum;
			if (record.probSum <= record.prob1 + record.prob2 && record.probSum >= std::max(record.prob1, record.prob2))
				break;
			else
				std::cout << "Invalid value. Try again.\n";
		}
		file.write((char*)&record.prob1, sizeof(double));
		file.write((char*)&record.prob2, sizeof(double));
		file.write((char*)&record.probSum, sizeof(double));
	}
	file.close();
}