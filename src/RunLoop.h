#ifndef __RUNLOOP_H__
#define __RUNLOOP_H__

#include <thread>
#include <functional>

typedef std::function<void()> RunLoopFunc;

class RunLoop
{
private:
	std::thread _loop_thread;
	bool _cancel;
	// No default, copy, or move constructor
	RunLoop()=delete;
	RunLoop(RunLoop& other)=delete;
	RunLoop(RunLoop&& other)=delete;

public:
	RunLoop(RunLoopFunc loop);
	~RunLoop();
	void stop();
};

#endif /* __RUNLOOP_H__ */
