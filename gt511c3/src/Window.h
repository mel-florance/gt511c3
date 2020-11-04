#pragma once

#include <functional>

#include "Core.h"
#include "Event.h"

struct GLFWwindow;

struct WindowProps
{
	std::string Title;
	float Width;
	float Height;

	WindowProps(const std::string& title = "Window",
		float width = 1280.0f,
		float height = 720.0f)
		: Title(title), Width(width), Height(height)
	{
	}
};

class Window
{
public:
	Window(const WindowProps& props);
	~Window();

	using EventCallbackFn = std::function<void(Event&)>;

	void OnUpdate();

	inline float GetWidth() const { return m_Data.Width; }
	inline float GetHeight() const{ return m_Data.Height; }

	inline void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
	void SetVSync(bool enabled);
	bool IsVSync() const;

	inline void* GetNativeWindow() const { return m_Window; }
	void Maximize() const;
	void Init(const WindowProps& props);
	void Shutdown();

	static Window* Create(const WindowProps& props = WindowProps());

	GLFWwindow* m_Window;

	struct WindowData
	{
		std::string Title;
		float Width, Height;
		bool VSync;

		EventCallbackFn EventCallback;
	};

	WindowData m_Data;
};

