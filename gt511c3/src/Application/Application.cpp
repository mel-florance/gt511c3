#include "Application.h"
#include "Events/ApplicationEvent.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Core/Engine.h"
#include "Events/Input.h"

Application* Application::s_Instance = nullptr;

Application::Application(bool headless, const WindowProps& props) :
	headless(headless),
	m_Running(true),
	m_windowProps(props),
	states_index(0),
	current_state(0)
{
	if (s_Instance)
		std::cout << "Application already exists!" << std::endl;

	s_Instance = this;

	if (!headless)
	{
		m_Engine = new Engine(this);
		m_Window = std::unique_ptr<Window>(Window::Create(m_windowProps));
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}
}

Application::~Application()
{
	//delete m_Editor;
}

void Application::close()
{
	m_Engine->stop();
	glfwSetWindowShouldClose((GLFWwindow*)m_Window->GetNativeWindow(), GLFW_TRUE);
}

void Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay)
{
	m_LayerStack.PushOverlay(overlay);
	overlay->OnAttach();
}

void Application::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);

	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

	m_Engine->OnEvent(event);

	for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
		(*--it)->OnEvent(event);

		if (event.Handled)
			break;
	}
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	m_Engine->stop();
	return true;
}

void Application::run()
{
	m_Engine->start();
}
