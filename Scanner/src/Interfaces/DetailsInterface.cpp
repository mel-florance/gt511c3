#include "DetailsInterface.h"
#include "Application/Application.h"
#include "Controllers/AppController.h"
#include "Controllers/ScannerController.h"

User* DetailsInterface::current_user = nullptr;
Texture* DetailsInterface::current_image = nullptr;
std::shared_ptr<UsersController> DetailsInterface::users = nullptr;

DetailsInterface::DetailsInterface()
{
	users = Application::Get().get_controller<UsersController>("users");
}

void DetailsInterface::render()
{
	ImGui::Begin("Details");
	ImGui::Columns(2, "details_panels");

	auto scanner = Application::Get().get_controller<ScannerController>("scanner");

	if (current_user != nullptr)
	{
		if (ImGui::Button("Delete user", ImVec2(150.0f, 25.0f))) {
			if (scanner->get()->delete_user(current_user->id)) {
				users->get_table().set_selected_row(-1);
				users->remove_user(current_user->id);
				users->save_users("./data/users.csv");
				users->set_table_data();

				std::cout << "User #" << current_user->id << " deleted." << std::endl;
			}
		}

		ImGui::SetCursorPosX(165.0f);
		ImGui::SetCursorPosY(33.0f);

		if (ImGui::Button("Verify", ImVec2(150.0f, 25.0f))) {
			if (scanner->get()->capture_finger(1)) {
				if (scanner->get()->verify_user(current_user->id)) {
					std::cout << "User verified" << std::endl;
				}
				else
					std::cout << "Failed to verify user." << std::endl;
			}
			else
				std::cout << "Can't capture user finger to verify." << std::endl;
		}

		ImGui::SetCursorPosX(161.0f * 2.0f);
		ImGui::SetCursorPosY(33.0f);

		if (ImGui::Button("Identify", ImVec2(150.0f, 25.0f))) {
			if (scanner->get()->capture_finger(1))
			{
				auto index = scanner->get()->identify_user();
				if (index >= 0) {
					std::cout << "User #" << current_user->id << " identified." << std::endl;
				}
				else
					std::cout << "Failed to identify user." << std::endl;
			}
			else
				std::cout << "Can't capture user finger to identify." << std::endl;
		}

		ImGui::Text(("User " + current_user->name).c_str());
	}

	ImGui::NextColumn();

	if (current_image != nullptr) {

		float ratio = current_image->getWidth() / current_image->getHeight();
		float w = ImGui::GetColumnWidth();

		ImGui::Image(
			(void*)(uintptr_t)current_image->getId(),
			ImVec2(w, w * ratio),
			ImVec2(0.04f, 0.0f),
			ImVec2(1.0f, 1.0f)
		);
	}
	
	ImGui::Columns(1);

	ImGui::End();
}

void DetailsInterface::on_user_selected(int id)
{
	current_user = &users->get_users().at(id);

	if (current_image != nullptr) {
		delete current_image;
		current_image = nullptr;
	}

	current_image = new Texture(
		std::string("./data/fingerprints/") + std::to_string(id) + ".bmp",
		false,
		false,
		Texture::ChannelType::RGB,
		true
	);

	current_image->load();
}
