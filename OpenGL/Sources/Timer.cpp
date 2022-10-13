#include "Timer.hpp"

namespace Core::Time
{
	Timer::Timer()
	{
	}

	Timer::~Timer()
	{
	}

	void Timer::Update()
	{
		_currentFrame = glfwGetTime();
		_deltaTime = _currentFrame - _lastFrame;
		_lastFrame = _currentFrame;
	}

	double Timer::GetDeltaTime()
	{
		return _deltaTime;
	}
}
