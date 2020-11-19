#include "StatusbarInterface.h"
#include "Application/Application.h"
#include "Interface/ImGuiExtensions.h"
#include "Controllers/AppController.h"
#include "Controllers/ScannerController.h"
#include "Controllers/IconsController.h"
#include "Core/Utils.h"

StatusbarInterface::StatusbarInterface()
{
}

void StatusbarInterface::render()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Status", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	ImGui::PopStyleVar();

	auto style = ImGui::GetStyle();
	auto scanner = Application::Get().get_controller<ScannerController>("scanner");
	auto icons = Application::Get().get_controller<IconsController>("icons");

	ImGui::Columns(5);
	ImGui::SetColumnWidth(0, 650);
	ImGui::SetColumnWidth(1, 135);
	ImGui::SetColumnWidth(2, 120);
	ImGui::SetColumnWidth(3, 120);

	ImGui::SetCursorPosY(style.WindowPadding.y + 1.0f);
	ImGui::Text("Press F1 for help");

	ImGui::NextColumn();

	ImGui::SetCursorPosY(style.WindowPadding.y + 1.0f);

	icons->drawIcon(
		scanner->serial_connected ? "connected" : "disconnected",
		{ 15.0f, 15.0f },
		IconsController::IconType::IMAGE
	);

	ImGui::SameLine();
	ImGui::TextTooltip(scanner->serial_connected ? "Connected" : "Disconnected", "Serial connection status");

	ImGui::NextColumn();
	ImGui::SetCursorPosY(style.WindowPadding.y + 1.0f);
	ImGui::Text("TX");
	ImGui::SameLine();
	ImGui::TextTooltip(Utils::bytesToSize(scanner->get()->getTX()).c_str(), "Transmitted bytes");

	ImGui::NextColumn();
	ImGui::SetCursorPosY(style.WindowPadding.y + 1.0f);
	ImGui::Text("RX");
	ImGui::SameLine();
	ImGui::TextTooltip(Utils::bytesToSize(scanner->get()->getRX()).c_str(), "Received bytes");

	ImGui::NextColumn();
	ImGui::SetCursorPosY(style.WindowPadding.y + 1.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 0.0f));

	// Baudrate
	auto baudrate = scanner->serial_connected
		? std::to_string(scanner->get()->getBaudRate())
		: "0";

	ImGui::TextTooltip(baudrate, "Baudrate");
	ImGui::SameLine();

	// Parity
	auto parity = scanner->serial_connected
		? scanner->parity_to_str(scanner->get()->getParity())
		: "0";

	ImGui::Text("-");
	ImGui::SameLine();
	ImGui::TextTooltip(parity, "Parity");
	ImGui::SameLine();

	// Stopbits
	auto stopbits = scanner->serial_connected
		? scanner->stopbits_to_str(scanner->get()->getStopbits())
		: "0";

	ImGui::Text("-");
	ImGui::SameLine();
	ImGui::TextTooltip(parity, "Stopbits");
	ImGui::SameLine();

	// flow_control
	auto flowcontrol = scanner->serial_connected
		? scanner->flowcontrol_to_str(scanner->get()->getFlowcontrol())
		: "0";

	ImGui::Text("-");
	ImGui::SameLine();
	ImGui::TextTooltip(flowcontrol, "Flow control");

	ImGui::PopStyleVar();

	ImGui::Columns(1);
	ImGui::End();
}
