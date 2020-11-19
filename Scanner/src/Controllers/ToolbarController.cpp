#include "Application/Application.h"
#include "Interface/ImGuiExtensions.h"
#include "Controllers/ToolbarController.h"

#include "Interfaces/ToolbarInterface.h"
#include "IconsManager.h"

ToolbarController::ToolbarController() :
	Controller()
{
	this->ui = new ToolbarInterface();
}

void ToolbarController::OnEvent(Event& event)
{
}

void ToolbarController::OnUpdate(float delta)
{
}

void ToolbarController::OnRender()
{
	ui->render();
}
