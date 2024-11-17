#include "RecordInfo.h"

void RecordInfo::reset()
{
	prev.reset();
}

void RecordInfo::fetchRecord()
{
	now = in->fetchRecord();
}

void RecordInfo::putRecord()
{
	if (now != nullptr)
	{
		out->putRecord(now);
		prev = *now;
		now = in->fetchRecord();
	}
}

void RecordInfo::putRest()
{
	while (now && prev <= *now)
	{
		putRecord();
	}
}

bool RecordInfo::seriesEnd()
{
	return now == nullptr || *now < prev;
}