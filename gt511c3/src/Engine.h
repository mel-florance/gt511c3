#pragma once

#include "GameLoop.h"

class Window;
class Application;
class Event;

class Engine
{
public:
	Engine(Application* application);
	~Engine();

	void start();
	void stop();
	void OnEvent(Event& event);
	static void update(GameLoop* loop, Engine* self);
	static void render(GameLoop* loop, Engine* self);

	inline float getFrameTiming() { return (float)gameLoop->getProfiler()->getReport("frame"); }
	inline float getUpdateTiming() { return (float)gameLoop->getProfiler()->getReport("update"); }
	inline float getRenderTiming() { return (float)gameLoop->getProfiler()->getReport("render"); }
	inline float getSleepTiming() { return (float)gameLoop->getProfiler()->getReport("sleep"); }

	inline GameLoop* getGameLoop() { return gameLoop; }
	inline float getFPS() { return gameLoop->getFps(); }
	inline std::map<int, float>& getFpsArray() { return gameLoop->getFpsList(); }

private:
	Application* application;
	GameLoop* gameLoop;
};
