#include "ScannerApplication.h"

#ifdef GT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

ScannerApplication::ScannerApplication() : Application(false)
{
	scanner = std::make_unique<Scanner>();
	scanner->open(1);

	//blink_led();

	Engine* engine = this->getEngine();

	auto layer = new ApplicationLayer(engine);
	auto main = new ImGuiLayer();
	PushLayer(layer);
	PushOverlay(main);

	this->GetWindow().SetVSync(false);
}

ScannerApplication::~ScannerApplication()
{
}

void ScannerApplication::blink_led()
{
	Sleep(100);

	while (true) {
		scanner->toggle_led(1);
		Sleep(1000);
		scanner->toggle_led(0);
		Sleep(1000);
	}
}

ApplicationLayer::ApplicationLayer(Engine* engine) :
	Layer("Test"),
	engine(engine)
{

}

void ApplicationLayer::OnUpdate(float delta)
{

}

void ApplicationLayer::OnAttach()
{
}

void ApplicationLayer::OnEvent(Event& event) {

}

void ApplicationLayer::OnImGuiRender()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	bool p_open;
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_DockNodeHost |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("ApplicationLayer", &p_open, window_flags);
	ImGui::PopStyleVar();

	ImGui::Text("Hello");
	/*for (auto it = controllers.begin(); it != controllers.end(); ++it)
		if (it->second->state == current_state)
			it->second->OnRender();*/

	ImGui::End();
}
