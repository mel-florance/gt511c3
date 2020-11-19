#pragma once

#ifdef GT_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#include "Scanner/Scanner.h"
#include "Scanner/Protocol.h"
#include "Application/Application.h"
#include "Application/Controller.h"
#include "Interface/Interface.h"
#include "Interface/Layer.h"
#include "Interface/ImGuiLayer.h"
#include "Interface/ImGuiExtensions.h"
#include "Events/MouseButtons.h"
#include "Events/KeyCodes.h"
#include "Core/Engine.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "Application/Entrypoint.h"
