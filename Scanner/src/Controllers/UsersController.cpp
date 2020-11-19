#include "UsersController.h"
#include "Interfaces/UsersInterface.h"
#include "Interfaces/DetailsInterface.h"

UsersController::UsersController() :
	Controller(),
	users({}),
	table(ImGui::Table("users"))
{
	this->ui = new UsersInterface();

	table.set_headers({ "ID", "Name" });
	table.set_row_selected_callback(&DetailsInterface::on_user_selected);
	load_users("./data/users.csv");
	set_table_data();
}

void UsersController::OnEvent(Event& event)
{
}

void UsersController::OnUpdate(float delta)
{
}

void UsersController::OnRender()
{
	this->ui->render();
}

bool UsersController::add_user(const User& user)
{
	auto exists = std::find_if(users.begin(), users.end(), [=](const User& item) {
		return item.id == user.id;
		});

	if (exists == users.end()) {
		users.push_back(user);
		return true;
	}

	return false;
}

void UsersController::remove_user(unsigned int id)
{
	auto condition = [&](const User& user) { return user.id == id; };
	users.erase(std::remove_if(users.begin(), users.end(), condition), users.end());
}

void UsersController::clear_users()
{
	users.clear();
}

void UsersController::update_user(size_t index, const User& data)
{
	if (index >= 0 && index < users.size() && users.size() > 0) {
		auto user = users.at(index);
		user = std::move(data);
	}
}

int UsersController::get_user_id(size_t index)
{
	if (index >= 0 && index < users.size()) {
		auto user = users.at(index);
		return user.id;
	}

	return -1;
}

std::vector<User>& UsersController::load_users(const std::string& filename)
{
	std::ifstream file;
	file.open(filename);
	std::string line;
	size_t index = 0;

	std::cout << "Loading from file: " << filename << std::endl;

	while (std::getline(file, line)) {
		if (index > 0) {
			auto parts = Utils::splitString(line, std::string(1, CSV_DELIMITER));

			User user;
			user.id = std::atoi(parts[0].c_str());
			user.name = parts[1];

			std::cout << "Loaded user " << user.id << std::endl;
			users.push_back(user);
		}

		++index;
	}

	file.close();

	return users;
}

void UsersController::save_users(const std::string& filename)
{
	std::ofstream file;
	file.open(filename, std::ios::out | std::ios::trunc);

	file
		<< "Id" << CSV_DELIMITER
		<< "Name\n";

	for (auto& user : users) {
		file
			<< user.id << CSV_DELIMITER
			<< user.name << "\n";
	}

	file.close();
}

void UsersController::set_table_data()
{
	std::vector<std::vector<std::string>> body = {};

	for (auto& user : users) {
		std::vector<std::string> columns;
		columns.push_back(std::to_string(user.id));
		columns.push_back(user.name);
		body.push_back(columns);
	}

	table.set_body(body);
}
