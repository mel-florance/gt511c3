#include "MenuController.h"
#include "Interfaces/MenuInterface.h"

MenuController::MenuController() : Controller()
{
	this->ui = new MenuInterface();
}

void MenuController::OnEvent(Event& event)
{
}

void MenuController::OnUpdate(float delta)
{
}

void MenuController::OnRender()
{
}
