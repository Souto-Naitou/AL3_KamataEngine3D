#include "TimedCall.h"

TimedCall::TimedCall(std::function<void(void)> _function, uint32_t _time) : function_(_function), time_(_time) {}

void TimedCall::Update()
{
	if (isFinished_) return;
	time_--;
	if (time_ <= 0)
	{
		isFinished_ = true;
		// コールバック関数呼び出し
		function_();
	}
}
