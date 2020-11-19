#pragma once

#include "Interface/Interface.h"

class StatusbarInterface : public Interface
{
public:
	StatusbarInterface();

	virtual void render() override;
};

