#include "Utils.h"
#include <imgui.h>

void Utils::initColumns(float size)
{
	ImGui::Columns(2, "twoColumns", true);

	static unsigned short initial_column_spacing = 0;
	if (initial_column_spacing < 2)
	{
		ImGui::SetColumnWidth(0, size);
		initial_column_spacing++;
	}
}

