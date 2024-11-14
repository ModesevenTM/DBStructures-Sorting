#include <iostream>
#include <ctime>
#include <string>
#include <filesystem>

#include "Record.h"
#include "functions.h"
#include "Polyphase.h"

// Jakub Wojtkowiak, 193546
// 33. Rekordy pliku: 2 zdarzenia - ich prawdopodobieństwa oraz prawdopodobieństwo sumy tych zdarzeń.
// Uporządkowanie wg prawdopodobieństwa iloczynu tych zdarzeń.

int main()
{
	srand(time(NULL));
	std::cout << "Choose an option:\n1. Generate records\n2. Create record with given values\n3. Read directly from file\n";
	int choice;
	std::cin >> choice;
	if (choice == 1)
	{
		std::cout << "How many records do you want to generate?\n";
		int n;
		std::cin >> n;
		generateRandom(n);
	}
	else if (choice == 2)
	{
		std::cout << "How many records do you want to create?\n";
		int n;
		std::cin >> n;
		createRecords(n);
	}
	else if (choice == 3)
	{
		std::string filename;
		std::cout << "Enter filename: ";
		std::cin.ignore();
		std::getline(std::cin, filename);
		std::filesystem::copy_file(filename, "t3.tap", std::filesystem::copy_options::overwrite_existing);
	}
	if (choice >= 1 && choice <= 3)
		Polyphase polyphase;

    return 0;
}