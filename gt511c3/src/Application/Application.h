#pragma once

#include <memory>

#include "Core/Core.h"
#include "Interface/Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Interface/LayerStack.h"
#include "Interface/ImGuiLayer.h"
#include "Controller.h"

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

	template<typename T>
	inline void register_controller(const std::string& name) {
		auto controller = std::make_shared<T>();
		controllers[name] = controller;
		register_state(name);
	}

	template<typename T>
	inline std::shared_ptr<T> get_controller(const std::string& name) {
		auto it = controllers.find(name);

		if (it != controllers.end())
			return std::dynamic_pointer_cast<T>(it->second);

		return nullptr;
	}

	inline void controllers_update(float delta = 0.0f) {
		for (auto it = controllers.begin(); it != controllers.end(); ++it) {
			if (it->second->active)
				it->second->OnUpdate(delta);
		}
	}

	inline void controllers_event(Event& event) {
		for (auto it = controllers.begin(); it != controllers.end(); ++it) {
			if (it->second->active)
				it->second->OnEvent(event);
		}
	}

	inline void controllers_render() {
		for (auto it = controllers.begin(); it != controllers.end(); ++it) {
			if (it->second->active)
				it->second->OnRender();
		}
	}

	inline Window& GetWindow() { return *m_Window; }
	inline Window* GetWindowPtr() { return m_Window.get(); }
	inline static Application& Get() { return *s_Instance; }
	inline LayerStack& getLayerStack() { return m_LayerStack; }
	inline ImGuiLayer* getImGuiLayer() { return m_ImGuiLayer; }
	inline Engine* getEngine() { return m_Engine; }

	inline void register_state(const std::string& name) {
		auto it = controllers.find(name);

		if (it != controllers.end()) {
			int state = states_index;
			states.push_back(state);
			states_names[state] = name;

			it->second->state = state;

			++states_index;
		}
	}

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

	int current_state;
	int states_index;
	std::vector<int> states;
	std::unordered_map<int, std::string> states_names;

	std::unordered_map<std::string, std::shared_ptr<Controller>> controllers;
};

