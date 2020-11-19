#include "LoggerInterface.h"
#include "Application/Application.h"
#include "Controllers/AppController.h"

LoggerInterface::LoggerInterface()
{
}

void LoggerInterface::render()
{
	ImGui::Begin("Logger");
	ImGui::Text("Hey i'm in the logger!");
	ImGui::End();
}
