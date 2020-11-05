#include "Clock.h"
#include "GLFW/glfw3.h"

Clock::Clock()
{

}

double Clock::getTime()
{
	return time = glfwGetTime();
}

Clock::~Clock()
{

}
