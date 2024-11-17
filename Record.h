#pragma once
#include <iostream>

class Record {
public:
	double prob1;
	double prob2;
	double probSum;
	double probProd;

	Record();
	Record(double pP);
	Record(double p1, double p2, double pS);
	Record(const Record& r);
	Record& operator=(const Record& r);

	void randomize();
	void print();
	void reset();

	bool operator<(const Record& r) const;
	bool operator>(const Record& r) const;
	bool operator<=(const Record& r) const;
	bool operator>=(const Record& r) const;
	bool operator==(const Record& r) const;
	bool operator!=(const Record& r) const;
};