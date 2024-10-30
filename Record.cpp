#include "Record.h"
#include <cstdlib>

Record::Record()
{
	prob1 = 0;
	prob2 = 0;
	probSum = 0;
	probProd = 0;
}

Record::Record(double p1, double p2, double pS)
{
	prob1 = p1;
	prob2 = p2;
	probSum = pS;
	probProd = p1 + p2 - pS;
}

Record::Record(double p1, double p2, double pS, double pP)
{
	prob1 = p1;
	prob2 = p2;
	probSum = pS;
	probProd = pP;
}

void Record::randomize()
{
	prob1 = (double)rand() / RAND_MAX;
	prob2 = (double)rand() / RAND_MAX;
	while (true)
	{
		probSum = (double)rand() / RAND_MAX;
		if (probSum <= prob1 + prob2 && probSum >= std::max(prob1, prob2))
		{
			probProd = prob1 + prob2 - probSum;
			break;
		}
	}
}

void Record::print()
{
	std::cout << "P(A): " << prob1 << "|P(B): " << prob2 << "|P(AuB): " << probSum << "|P(AnB): " << probProd << std::endl;
}