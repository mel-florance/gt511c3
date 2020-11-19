#include "StatusbarController.h"
#include "Interfaces/StatusbarInterface.h"

StatusbarController::StatusbarController() : Controller()
{
	this->ui = new StatusbarInterface();
}

void StatusbarController::OnEvent(Event& event)
{
}

void StatusbarController::OnUpdate(float delta)
{
}

void StatusbarController::OnRender()
{
	this->ui->render();
}
