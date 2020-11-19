#pragma once

#include "Interface/Interface.h"

class ToolbarInterface : public Interface
{
public:
	ToolbarInterface();

	virtual void render() override;
};

