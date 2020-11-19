#pragma once

#include "Interface/Interface.h"

class LoggerInterface : public Interface
{
public:
	LoggerInterface();

	virtual void render() override;
};

