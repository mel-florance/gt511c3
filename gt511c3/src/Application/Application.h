#pragma once

#include <memory>

#include "Core/Core.h"
#include "Interface/Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Interface/LayerStack.h"
#include "Interface/ImGuiLayer.h"

class Engine;

class Application
{
public:
	Application(
		bool headless = false,
		const WindowProps& props = WindowProps()
	);

	virtual ~Application();
	void run();
	void OnEvent(Event& e);
	void close();

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);

	inline Window& GetWindow() { return *m_Window; }
	inline Window* GetWindowPtr() { return m_Window.get(); }
	inline static Application& Get() { return *s_Instance; }
	inline LayerStack& getLayerStack() { return m_LayerStack; }
	inline ImGuiLayer* getImGuiLayer() { return m_ImGuiLayer; }
	inline Engine* getEngine() { return m_Engine; }

private:
	bool OnWindowClose(WindowCloseEvent& e);
	std::unique_ptr<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;

	bool m_Running;
	LayerStack m_LayerStack;

	static Application* s_Instance;
	bool headless;
	bool is_editor;
	WindowProps m_windowProps;
	Engine* m_Engine;
};

