#include <iostream>
#include <ctime>

#include "Record.h"
#include "functions.h"
#include "Polyphase.h"

// Jakub Wojtkowiak, 193546
// 33. Rekordy pliku: 2 zdarzenia - ich prawdopodobieństwa oraz prawdopodobieństwo sumy tych zdarzeń.
// Uporządkowanie wg prawdopodobieństwa iloczynu tych zdarzeń.

int main()
{
	srand(time(NULL));
	std::cout << "Choose an option:\n1. Generate records\n2. Create record with given values\n3. Read directly from file (t3.tap)\n";
	int choice;
	std::cin >> choice;
	if (choice == 1)
	{
		std::cout << "How many records do you want to generate?\n";
		int n;
		std::cin >> n;
		generateRandom(n);
		Polyphase polyphase;
	}
	else if (choice == 2)
	{
		std::cout << "How many records do you want to create?\n";
		int n;
		std::cin >> n;
		Record* records = new Record[n];
		for (int i = 0; i < n; i++)
		{
			while (true)
			{
				// TODO: test every value if it's within limits individually
				std::cout << "Enter probabilities so that their sum is between 0 and 1.\n";
				std::cout << "Enter probability 1: ";
				double p1;
				std::cin >> p1;
				std::cout << "Enter probability 2: ";
				double p2;
				std::cin >> p2;
				std::cout << "Enter probability sum: ";
				double pS;
				std::cin >> pS;
				if (pS <= p1 + p2 && pS >= std::max(p1, p2))
				{
					records[i] = Record(p1, p2, pS);
					break;
				}
				else
					std::cout << "Invalid values. Try again.\n";
			}
		}
	}
	else if (choice == 3)
	{
		Polyphase polyphase;
	}
    return 0;
}