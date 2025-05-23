#include "Polyphase.h"

Polyphase::Polyphase()
{
	std::cout << "Do you want to print the content of the output tape during merging? (y/n)\n";
	char c;
	std::cin >> c;
	if (c == 'y' || c == 'Y')
		printContent = true;
	distribute();
	std::cout << "Dummy runs: " << stats.dummyRuns << "\n";
	merge();
}

void Polyphase::distribute()
{
	std::string tapes[] = { "t1.tap", "t2.tap", "t3.tap" };
	InputBuffer* in = new InputBuffer(tapes[outTape], &stats);
	OutputBuffer* out1 = new OutputBuffer(tapes[(outTape + 1) % 3], &stats, false);
	OutputBuffer* out2 = new OutputBuffer(tapes[(outTape + 2) % 3], &stats, false);
	OutputBuffer* outTapes[] = {out1, out2};
	Record prevRecord[] = { Record(-1.0f), Record(2.0f) };
	int tapeIdx = 0;
	int seriesBefore = 1, seriesAfter = 1;

	std::cout << "Before distribution:\n";

	// Fibonacci distribution
	int i = 1, fib = 1, prev = 1;
	bool isEnd = false, isNewSeries = false;
	bool isCoalescence = false, lastSeriesCoalescence = false;
	while (!isEnd)
	{
		if (i > 2)
		{
			int newfib = fib + prev;
			prev = fib;
			fib = newfib;
		}
		for (int j = 1 - isCoalescence; j <= prev; j++)
		{
			if (lastSeriesCoalescence) break;
			if (isCoalescence) isCoalescence = false;
			if (isEnd)
				stats.dummyRuns++;
			else while (!isEnd)
			{
				isNewSeries = false;
				Record* record = in->fetchRecord();
				if (record == nullptr)
				{
					isEnd = true;
					if (!j)
					{
						tapeIdx = (tapeIdx + 1) % 2; // since we ended up on a "shorter" tape
						lastSeriesCoalescence = true;
					}
					break;
				}
				if (prevRecord[tapeIdx] > *record)
				{
					isNewSeries = true;
					seriesBefore++;
					seriesAfter++;
					if (j == prev)
						tapeIdx = (tapeIdx + 1) % 2;
				}
				std::cout << "Tape " << tapeIdx + 1 << " -> ";
				record->print();
				outTapes[tapeIdx]->putRecord(record);
				if (isNewSeries && prevRecord[tapeIdx] <= *record)
				{
					isCoalescence = true;
					seriesAfter--;
				}
				prevRecord[tapeIdx] = *record;
				if (isNewSeries) break;
			}
		}
		i++;
	}

	std::cout << "\nSeries before distribution: " << seriesBefore << "\n";
	std::cout << "Series after distribution: " << seriesAfter << "\n";

	delete in;
	delete out1;
	delete out2;

	longerTape = tapeIdx;
}

void Polyphase::merge()
{
	std::string tapes[] = { "t1.tap", "t2.tap", "t3.tap" };
	OutputBuffer* out = new OutputBuffer(tapes[outTape], &stats, printContent);
	InputBuffer* in[2] = { new InputBuffer(tapes[(outTape + 1) % 3], &stats), new InputBuffer(tapes[(outTape + 2) % 3], &stats) };
	RecordInfo record[2] = { {in[0], out, Record(-1.0f), in[0]->fetchRecord()}, {in[1], out, Record(-1.0f), in[1]->fetchRecord()}};

	bool isEnd = false, isPhaseEnd = false;
	int phase = 1;
	if (printContent)
	{
		std::cout << "\nAfter phase " << phase << ":\n";
		std::cout << "Longer tape: " << record[longerTape].in->filename << "\n";
		std::cout << "Shorter tape: " << record[(longerTape + 1) % 2].in->filename << "\n";
		std::cout << "Output tape: " << out->filename << "\n";
	}

	while (!isEnd)
	{
		while (stats.dummyRuns)
		{
			record[(longerTape + 1) % 2].putRest();
			record[0].reset();
			record[1].reset();
			stats.dummyRuns--;
		}
		if (record[0].now == nullptr && record[1].now == nullptr)
		{
			isEnd = true;
			delete record[0].in;
			delete record[1].in;
			delete out;
		}
		else {
			if (record[(longerTape + 1) % 2].now == nullptr)
			{
				if (!isPhaseEnd)
					record[longerTape].putRest();
				
				delete record[(longerTape + 1) % 2].in;
				delete record[(longerTape + 1) % 2].out;
				record[(longerTape + 1) % 2].in = record[longerTape].in;
				record[(longerTape + 1) % 2].now = record[longerTape].now;

				if (record[(longerTape + 1) % 2].now == nullptr)
				{
					isEnd = true;
					delete record[(longerTape + 1) % 2].in;
				}
				else
				{
					isPhaseEnd = false;
					record[longerTape].in = new InputBuffer(tapes[outTape], &stats);
					record[longerTape].fetchRecord();
					outTape = (outTape + 2 - longerTape) % 3;
					out = new OutputBuffer(tapes[outTape], &stats, printContent);
					record[longerTape].out = out;
					record[(longerTape + 1) % 2].out = out;
					phase++;
					if (printContent)
					{
						std::cout << "\nAfter phase " << phase << ":\n";
						std::cout << "Longer tape: " << record[longerTape].in->filename << "\n";
						std::cout << "Shorter tape: " << record[(longerTape + 1) % 2].in->filename << "\n";
						std::cout << "Output tape: " << out->filename << "\n";
					}
					record[0].reset();
					record[1].reset();
				}
			}
			else
			{
				if (record[0].seriesEnd())
				{
					record[1].putRest();
					record[0].reset();
					record[1].reset();
					if (record[1].now == nullptr && (longerTape + 1) % 2 == 1)
						isPhaseEnd = true;
				}
				else if (record[1].seriesEnd())
				{
					record[0].putRest();
					record[0].reset();
					record[1].reset();
					if (record[0].now == nullptr && (longerTape + 1) % 2 == 0)
						isPhaseEnd = true;
				}
				else record[*record[1].now < *record[0].now].putRecord();
			}
		}
	}

	InputBuffer finalIn(tapes[outTape], nullptr);
	std::cout << "\nAfter merging:\n";
	Record* finalRecord = finalIn.fetchRecord();
	while (finalRecord != nullptr)
	{
		finalRecord->print();
		finalRecord = finalIn.fetchRecord();
	}

	std::cout << "\nStatistics:\n";
	std::cout << "Number of reads: " << stats.reads << "\n";
	std::cout << "Number of writes: " << stats.writes << "\n";
	std::cout << "Number of phases: " << phase << "\n";

}