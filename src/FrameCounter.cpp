#include "FrameCounter.h"

void FrameCounter::increment()
{
	std::unique_lock<std::mutex> lk(fc_mutex);
	count++;
}

void FrameCounter::print()
{
	std::unique_lock<std::mutex> lk(fc_mutex);
	printf("%d\n", count);
	count = 0;
}

FrameCounter& FrameCounter::operator++ (int x)
{
	std::unique_lock<std::mutex> lk(fc_mutex);
	count++;
	return *this;
}

FrameCounter& operator++ (FrameCounter& fc)
{
	std::unique_lock<std::mutex> lk(fc.fc_mutex);
	fc.count++;
	return fc;
}
