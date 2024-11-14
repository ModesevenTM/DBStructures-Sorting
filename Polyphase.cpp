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

Polyphase::~Polyphase()
{
}


// TODO: Some thoughts on the distribution and merging:
// - We distribute series on alternating tapes, according to the Fibonacci sequence.
// - If the first fetched number of the series is higher than the last number present on the tape,
//   we append this series to the last one on the tape. That way we need to fetch one additional series
//   since, well, Fibonacci sequence.
// - Alternately, the tape must include the amount of series corresponding to subsequent Fibonacci numbers,
//   e.g 1 on the first tape, 1 on the second, 1+1=2 on first, 1+2=3 on second, 2+3=5 on first, etc.
// - If, in the end, the amount of series on the tape is lower than the Fibonacci number, we need to prepend
//	 dummy series to the tape, stored in stats.dummyRuns global variable. There's no need to write these
//   phisically to the tape after distibution, but we need to keep track of them.
// - After the distribution, we need to merge the tapes. We start by fetching the first records from each tape.
// - We compare the values and write the one with the lower probability product to the output tape, then fetch
//	 another record from the same tape. We're doing this until we reach the end of the series, which happens
//	 when the fetched value is lower than the previous one from the same tape. Then we need to put the rest of
//	 the values from the other tape to the output tape, so the same thing happens with that series too.
//	 We repeat this process until we reach the end of any tape. Then the ended tape is open for writing and
//	 the one previously open for writing is now open for reading. We repeat the process until we empty both the
//   input tapes - the output tape should be sorted.

void Polyphase::distribute()
{
	std::string tapes[] = { "t1.tap", "t2.tap", "t3.tap" };
	InputBuffer* in = new InputBuffer(tapes[outTape], &stats);
	OutputBuffer* out1 = new OutputBuffer(tapes[(outTape + 1) % 3], &stats, false);
	OutputBuffer* out2 = new OutputBuffer(tapes[(outTape + 2) % 3], &stats, false);
	OutputBuffer* outTapes[] = {out1, out2};
	double prevOnTape[] = { -1.0f, 2.0f }; // TODO: ugly workaround
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
				if (prevOnTape[tapeIdx] > record->probProd)
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
				if (isNewSeries && prevOnTape[tapeIdx] <= record->probProd)
				{
					isCoalescence = true;
					seriesAfter--;
				}
				prevOnTape[tapeIdx] = record->probProd;
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
	InputBuffer* in1 = new InputBuffer(tapes[(outTape + 1) % 3], &stats);
	InputBuffer* in2 = new InputBuffer(tapes[(outTape + 2) % 3], &stats);
	InputBuffer* in[2] = { in1, in2 };

	double prevProb[] = { -1.0f, -1.0f };
	Record* record[] = { in1->fetchRecord(), in2->fetchRecord() };
	bool isEnd = false, isPhaseEnd = false;
	int phase = 1;
	if (printContent)
	{
		std::cout << "\nAfter phase " << phase << ":\n";
		std::cout << "Longer tape: " << in[longerTape]->filename << "\n";
		std::cout << "Shorter tape: " << in[(longerTape + 1) % 2]->filename << "\n";
		std::cout << "Output tape: " << out->filename << "\n";
	}

	while (!isEnd)
	{
		while (stats.dummyRuns)
		{
			while (record[(longerTape + 1) % 2] != nullptr && prevProb[(longerTape + 1) % 2] <= record[(longerTape + 1) % 2]->probProd)
			{
				out->putRecord(record[(longerTape + 1) % 2]);
				prevProb[(longerTape + 1) % 2] = record[(longerTape + 1) % 2]->probProd;
				record[(longerTape + 1) % 2] = in[(longerTape + 1) % 2]->fetchRecord();
			}
			prevProb[0] = -1.0f;
			prevProb[1] = -1.0f;
			stats.dummyRuns--;
		}
		if (record[0] == nullptr && record[1] == nullptr)
		{
			isEnd = true;
			delete in[0];
			delete in[1];
			delete out;
		}
		else {
			if (record[(longerTape + 1) % 2] == nullptr)
			{
				if (!isPhaseEnd)
				{
					while (record[longerTape] != nullptr && prevProb[longerTape] <= record[longerTape]->probProd)
					{
						out->putRecord(record[longerTape]);
						prevProb[longerTape] = record[longerTape]->probProd;
						record[longerTape] = in[longerTape]->fetchRecord();
					}
				}
				
				delete in[(longerTape + 1) % 2];
				delete out;
				in[(longerTape + 1) % 2] = in[longerTape];
				record[(longerTape + 1) % 2] = record[longerTape];
				in[longerTape] = new InputBuffer(tapes[outTape], &stats);
				record[longerTape] = in[longerTape]->fetchRecord();
				if (record[longerTape] != nullptr && record[(longerTape + 1) % 2] == nullptr)
				{
					isEnd = true;
					delete in[longerTape];
					delete in[(longerTape + 1) % 2];
				}
				else
				{
					isPhaseEnd = false;
					outTape = (outTape + 2 - longerTape) % 3;
					out = new OutputBuffer(tapes[outTape], &stats, printContent);
					phase++;
					if (printContent)
					{
						std::cout << "\nAfter phase " << phase << ":\n";
						std::cout << "Longer tape: " << in[longerTape]->filename << "\n";
						std::cout << "Shorter tape: " << in[(longerTape + 1) % 2]->filename << "\n";
						std::cout << "Output tape: " << out->filename << "\n";
					}
					prevProb[0] = -1.0f;
					prevProb[1] = -1.0f;
				}
			}
			else
			{
				if (record[0] == nullptr || record[0]->probProd < prevProb[0])
				{
					while (record[1] != nullptr && record[1]->probProd >= prevProb[1])
					{
						out->putRecord(record[1]);
						prevProb[1] = record[1]->probProd;
						record[1] = in[1]->fetchRecord();
					}
					prevProb[0] = -1.0f;
					prevProb[1] = -1.0f;
					if (record[1] == nullptr && (longerTape + 1) % 2 == 1)
						isPhaseEnd = true;
				}
				else if (record[1] == nullptr || record[1]->probProd < prevProb[1])
				{
					while (record[0] != nullptr && record[0]->probProd >= prevProb[0])
					{
						out->putRecord(record[0]);
						prevProb[0] = record[0]->probProd;
						record[0] = in[0]->fetchRecord();
					}
					prevProb[0] = -1.0f;
					prevProb[1] = -1.0f;
					if (record[0] == nullptr && (longerTape + 1) % 2 == 0)
						isPhaseEnd = true;
				}
				else {
					if (record[0]->probProd < record[1]->probProd)
					{
						out->putRecord(record[0]);
						prevProb[0] = record[0]->probProd;
						record[0] = in[0]->fetchRecord();
					}
					else
					{
						out->putRecord(record[1]);
						prevProb[1] = record[1]->probProd;
						record[1] = in[1]->fetchRecord();
					}
				}
			}
		}
	}

	std::cout << "\nStatistics:\n";
	std::cout << "Number of reads: " << stats.reads << "\n";
	std::cout << "Number of writes: " << stats.writes << "\n";
	std::cout << "Number of phases: " << phase << "\n";

	InputBuffer finalIn(tapes[outTape], &stats);
	std::cout << "\nAfter merging:\n";
	Record* finalRecord = finalIn.fetchRecord();
	while (finalRecord != nullptr)
	{
		finalRecord->print();
		finalRecord = finalIn.fetchRecord();
	}

}