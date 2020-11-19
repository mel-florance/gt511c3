#include "DetailsController.h"
#include "Interfaces/DetailsInterface.h"

DetailsController::DetailsController() : Controller()
{
	this->ui = new DetailsInterface();
}

void DetailsController::OnEvent(Event& event)
{
}

void DetailsController::OnUpdate(float delta)
{
}

void DetailsController::OnRender()
{
	this->ui->render();
}
