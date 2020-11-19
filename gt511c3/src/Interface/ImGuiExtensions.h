#pragma once

#include <vector>
#include <string>
#include <variant>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "Core/Utils.h"

namespace ImGui {

	static void SetNextWindowCentered(const ImVec2& size) {
		auto win_size = ImGui::GetWindowSize();
		auto win_pos = ImGui::GetWindowPos();

		ImGui::SetNextWindowPos(ImVec2(
			win_pos.x + (win_size.x * 0.5f) - (size.x * 0.5f),
			win_pos.y + (win_size.y * 0.5f) - (size.y * 0.5f)
		), ImGuiCond_Appearing);

		ImGui::SetNextWindowSize(size);
	}

	static void TextTooltip(const std::string& text, const std::string& description) {
		ImGui::Text(text.c_str());

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(description.c_str());
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	};

	class Table {
	public:
		Table(
			const std::string& name,
			int columns = 1,
			bool borders = true
		) :
			name(name),
			columns(columns),
			borders(borders),
			draw_list(nullptr),
			headers({}),
			body({}),
			row_height(0.0f),
			screen_pos(ImVec2(0.0f, 0.0f)),
			win_width(0.0f),
			cell_padding(ImVec2(5.0f, 5.0f)),
			border_color(ImColor(255, 255, 255, 80)),
			header_bg_color(ImColor(61, 61, 61, 255)),
			row_hovered_color(ImColor(255, 255, 255, 80)),
			row_selected_color(ImColor(255, 255, 255, 10)),
			row_hovered(-1),
			margin_top(0.0f),
			row_selected(-1),
			row_selected_callback(nullptr)
		{
		}

		inline void set_headers(const std::vector<std::string>& headers) {
			this->headers = headers;
		}

		inline void set_body(const std::vector<std::vector<std::string>>& body) {
			this->body = body;
		}

		inline void set_draw_list(ImDrawList* draw_list) {
			this->draw_list = draw_list;
		}

		inline void set_margin_top(float margin_top) {
			this->margin_top = margin_top;
		}

		inline void set_row_selected_callback(std::function<void(int)> function) {
			row_selected_callback = function;
		}

		inline int get_selected_row() { return row_selected; }
		inline void set_selected_row(int row) { row_selected = row; }

		inline void draw_headers() {
			for (unsigned int i = 0; i < headers.size(); ++i) {

				if (i > 0)
					ImGui::Indent(-cell_padding.x);

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + cell_padding.y);
				ImGui::Text(headers.at(i).c_str());
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - cell_padding.y);

				if (i > 0)
					ImGui::Indent(cell_padding.x);

				ImGui::NextColumn();
			}
		}

		inline void draw_body() {
			ImGuiStyle& style = ImGui::GetStyle();
			float y_start = ImGui::GetWindowPos().y;
			float offset_scroll_x = 0.0f;
			float offset_scroll_y = 0.f;

			if (ImGui::GetScrollMaxY() > 0)
				offset_scroll_x = style.ScrollbarSize;

			for (unsigned int i = 0; i < body.size(); ++i) {
				auto row = body.at(i);

				for (unsigned int j = 0; j < row.size(); ++j) {
					if (j > 0)
						ImGui::Indent(-cell_padding.x);

					auto column = row.at(j);

					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + cell_padding.y);
					ImGui::Text(column.c_str());
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - cell_padding.y);

					if (j > 0)
						ImGui::Indent(cell_padding.x);

					ImGui::NextColumn();
				}

				y_start = ImGui::GetWindowPos().y;
				auto mouse_pos = ImGui::GetMousePos();
				auto base_x = mouse_pos.x - ImGui::GetWindowPos().x;
				auto base_y = mouse_pos.y - y_start;
				float top = margin_top + row_height;
			

				if ((base_x > 8.0f && base_x < win_width - ImGui::GetWindowPos().x + 16.0f - offset_scroll_x)
					&& (base_y > top + row_height + row_height * i - ImGui::GetScrollY()
					&& base_y < (top) + row_height + row_height + row_height * i - ImGui::GetScrollY())) {

					y_start = ImGui::GetWindowPos().y + top + row_height * 2.0f - ImGui::GetScrollY();

					if (ImGui::IsMouseClicked(0)) {
						row_selected = i;

						if (row_selected_callback != nullptr)
							row_selected_callback(i);
					}

					row_hovered = i;
				}
				else {
					row_hovered = -1;
				}

				ImGui::Columns(1);

				if (row_hovered != -1) {
					draw_list->AddRectFilled(
						ImVec2(ImGui::GetWindowPos().x + 8.0f, y_start - row_height + (row_height * row_hovered)),
						ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - 8.0f - offset_scroll_x, y_start + (row_height * row_hovered) + 1.0f),
						row_hovered_color
					);
				}

				if (row_selected != -1) {
					y_start = ImGui::GetWindowPos().y + top + row_height * 2.0f - ImGui::GetScrollY();
					draw_list->AddRectFilled(
						ImVec2(ImGui::GetWindowPos().x + 8.0f, y_start - row_height + (row_height * row_selected)),
						ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - 8.0f - offset_scroll_x, y_start + (row_height * row_selected) + 1.0f),
						row_selected_color
					);
				}

				ImGui::Columns((int)headers.size(), name.c_str(), borders);
			}

			ImGui::Columns(1);
			
			for (unsigned int i = 0; i < body.size(); ++i) {
				// Bottom row border
				draw_border(
					ImVec2(ImGui::GetWindowPos().x + style.WindowPadding.x + 1.0f, screen_pos.y + (row_height * (i + 2)) + 1.0f),
					ImVec2(win_width + style.WindowPadding.x * 2.0f - offset_scroll_x, screen_pos.y + (row_height * (i + 2)) + 2.0f)
				);
				// Left row border
				draw_border(
					ImVec2(ImGui::GetWindowPos().x + style.WindowPadding.x, screen_pos.y + (row_height * (i + 1)) + 1.0f),
					ImVec2(ImGui::GetWindowPos().x + style.WindowPadding.x + 1.0f, screen_pos.y + (row_height * (i + 1)) + row_height + 2.0f)
				);
				// right row border
				draw_border(
					ImVec2(win_width + style.WindowPadding.x * 2.0f - offset_scroll_x, screen_pos.y + (row_height * (i + 1)) + 1.0f),
					ImVec2(win_width + style.WindowPadding.x * 2.0f + 1.0f - offset_scroll_x, screen_pos.y + (row_height * (i + 1)) + row_height + 2.0f)
				);
			}
		}

		inline void draw_border(const ImVec2& min, const ImVec2& max) {
			draw_list->AddRectFilled(min, max, border_color);
		}

		inline void draw() {
	
			ImGuiStyle& style = ImGui::GetStyle();

			row_height = ImGui::CalcTextSize("a").y + style.ItemSpacing.y;
			screen_pos = ImGui::GetCursorScreenPos();
			win_width = ImGui::GetWindowPos().x + (ImGui::GetWindowWidth() - 24.0f);

			float offset_scrollbar = 0.0f;

			if (ImGui::GetScrollMaxY() > 0)
				offset_scrollbar = style.ScrollbarSize;

			// Header background

			draw_list->AddRectFilled(
				ImVec2(ImGui::GetWindowPos().x + style.WindowPadding.x + 1.0f, screen_pos.y + 1.0f),
				ImVec2(win_width + style.WindowPadding.x * 2.0f - offset_scrollbar, screen_pos.y + row_height),
				header_bg_color
			);

			// Header top border
			draw_border(
				ImVec2(ImGui::GetWindowPos().x + style.WindowPadding.x + 1.0f, screen_pos.y + 1.0f),
				ImVec2(win_width + style.WindowPadding.x * 2.0f - offset_scrollbar, screen_pos.y + 2.0f)
			);
			// Header bottom border
			draw_border(
				ImVec2(ImGui::GetWindowPos().x + style.WindowPadding.x, screen_pos.y + row_height + 1.0f),
				ImVec2(win_width + style.WindowPadding.x * 2.0f - offset_scrollbar, screen_pos.y + row_height + 2.0f)
			);
			// Header left border
			draw_border(
				ImVec2(ImGui::GetWindowPos().x + style.WindowPadding.x, screen_pos.y + 1.0f),
				ImVec2(ImGui::GetWindowPos().x + style.WindowPadding.x + 1.0f, screen_pos.y + row_height + 1.0f)
			);
			// Header right border
			draw_border(
				ImVec2(win_width + style.WindowPadding.x * 2.0f - offset_scrollbar, screen_pos.y + 1.0f),
				ImVec2(win_width + style.WindowPadding.x * 2.0f + 1.0f - offset_scrollbar, screen_pos.y + row_height + 2.0f)
			);

			if (headers.size() > 0) {
				ImGui::Columns((int)headers.size(), name.c_str(), borders);
				draw_headers();
			}

			if (body.size() > 0)
				draw_body();

			ImGui::Columns(1);
		}

	private:
		std::string name;
		int columns;
		bool borders; 
		ImDrawList* draw_list;
		std::vector<std::string> headers;
		std::vector<std::vector<std::string>> body;
		float row_height;
		ImVec2 screen_pos;
		float win_width;
		ImVec2 cell_padding;
		ImColor border_color;
		ImColor header_bg_color;
		ImColor row_hovered_color;
		ImColor row_selected_color;
		int row_hovered;
		int row_selected;
		float margin_top;
		std::function<void(int)> row_selected_callback;
	};

}
