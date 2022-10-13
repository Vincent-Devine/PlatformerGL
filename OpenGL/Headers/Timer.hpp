#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Core::Time
{
	class Timer
	{
	public:
		Timer();
		~Timer();

		void Update();

		double GetDeltaTime();



	private:
		double _deltaTime = 0;
		double _currentFrame = 0; 
		double _lastFrame = 0;
	};

}
