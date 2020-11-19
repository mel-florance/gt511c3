#pragma once

#include "Interface/Interface.h"

class MenuInterface : public Interface
{
public:
	MenuInterface();

	virtual void render() override;
};

