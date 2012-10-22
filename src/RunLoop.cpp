#include "RunLoop.h"

RunLoop::RunLoop(RunLoopFunc loop)
{
	_cancel = false;
	_loop_thread = std::thread([this, loop]{
		while(!_cancel)
		{
			loop();
		}
	});
}

RunLoop::~RunLoop()
{
	if (!_cancel)
	{
		stop();
	}
}

void RunLoop::stop()
{
	_cancel = true;
	_loop_thread.join();
}

