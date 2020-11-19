#pragma once

#include "Application/Controller.h"
#include "Interfaces/UsersInterface.h"
#include "Interface/ImGuiExtensions.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Core/Utils.h"
#define CSV_DELIMITER ';'

struct User {
	unsigned int id;
	std::string name;
};

class UsersController : public Controller {
public:
	UsersController();

	void OnEvent(Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;

	bool add_user(const User& user);
	void remove_user(unsigned int id);
	void clear_users();
	void update_user(size_t index, const User& data);
	int get_user_id(size_t index);

	inline std::vector<User>& get_users() { return users; }
	inline ImGui::Table& get_table() { return table; }

	std::vector<User>& load_users(const std::string& filename);
	void save_users(const std::string& filename);

	void set_table_data();

private:
	std::vector<User> users;
	ImGui::Table table;
};
