#include "Engine.h"
#include "Application.h"
#include "LayerStack.h"
#include "Event.h"
#include "KeyCodes.h"
#include "GLFW/glfw3.h"
#include <glad/glad.h>

Engine::Engine(Application* application) : 
	application(application)
{
	gameLoop = new GameLoop(this);
	gameLoop->setUpdateCallback(&Engine::update);
	gameLoop->setRenderCallback(&Engine::render);
}

Engine::~Engine()
{
	delete gameLoop;
}

void Engine::start()
{
	gameLoop->start();
}

void Engine::stop()
{
	gameLoop->stop();
}

void Engine::OnEvent(Event& event)
{
	if (event.GetEventType() == EventType::MouseButtonPressed) {
		MouseButtonEvent& e = (MouseButtonEvent&)event;
		//event.Handled = true;
	}

	if (event.GetEventType() == EventType::MouseButtonReleased) {
		MouseButtonEvent& e = (MouseButtonEvent&)event;
		//event.Handled = true;
	}

	if (event.GetEventType() == EventType::MouseMoved) {
		MouseMovedEvent& e = (MouseMovedEvent&)event;
		//event.Handled = true;
	}

	if (event.GetEventType() == EventType::MouseScrolled) {
		MouseScrolledEvent& e = (MouseScrolledEvent&)event;
		//event.Handled = true;
	}

	if (event.GetEventType() == EventType::WindowResize) {
		WindowResizeEvent& e = (WindowResizeEvent&)event;
		//event.Handled = true;
	}

	if (event.GetEventType() == EventType::KeyPressed) {
		KeyPressedEvent& e = (KeyPressedEvent&)event;
		//event.Handled = true;
	}

	if (event.GetEventType() == EventType::KeyReleased) {
		KeyReleasedEvent& e = (KeyReleasedEvent&)event;
		//event.Handled = true;
	}
}

void Engine::update(GameLoop* loop, Engine* self)
{
	double delta = loop->getPassedTime();
	Window& window = self->application->GetWindow();

	for (Layer* layer : self->application->getLayerStack())
		layer->OnUpdate((float)delta);

	//self->forward_renderer->setViewport(0, 0, window.GetWidth(), window.GetHeight());
	//self->forward_renderer->update((float)loop->getPassedTime());
}

void Engine::render(GameLoop* loop, Engine* self)
{
	self->application->getImGuiLayer()->Begin();

	for (Layer* layer : self->application->getLayerStack())
		layer->OnImGuiRender();

	self->application->getImGuiLayer()->End();

	self->application->GetWindow().OnUpdate();

	glfwPollEvents();
}

