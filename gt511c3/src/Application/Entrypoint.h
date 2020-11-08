#pragma once

#include "Application.h"

#ifdef GT_PLATFORM_WINDOWS
#endif

extern Application* createApplication();

int main(int argc, char** argv)
{
	auto app = createApplication();
	app->run();
	delete app;
}

