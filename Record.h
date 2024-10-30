#pragma once
#include <iostream>

class Record {
public:
	double prob1;
	double prob2;
	double probSum;
	double probProd;

	Record();
	Record(double p1, double p2, double pS);
	Record(double p1, double p2, double pS, double pP);

	void randomize();
	void print();
};