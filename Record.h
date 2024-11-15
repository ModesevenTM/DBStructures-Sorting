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

	void randomize();
	void print();
};