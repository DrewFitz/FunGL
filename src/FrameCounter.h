#ifndef __FRAMECOUNTER_H__
#define __FRAMECOUNTER_H__

#include <iostream>
#include <mutex>

class FrameCounter
{
private:
	std::mutex fc_mutex;
	unsigned int count;
public:
	FrameCounter() : count(0) {};
	~FrameCounter()=default;

	FrameCounter& operator++ (int x);
	friend FrameCounter& operator++ (FrameCounter& fc);

	void increment();
	void print();
};

#endif
