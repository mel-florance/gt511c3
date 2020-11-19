#include "LoggerController.h"
#include "Interfaces/LoggerInterface.h"

LoggerController::LoggerController() : Controller()
{
	this->ui = new LoggerInterface();
}

void LoggerController::OnEvent(Event& event)
{
}

void LoggerController::OnUpdate(float delta)
{
}

void LoggerController::OnRender()
{
	this->ui->render();
}
