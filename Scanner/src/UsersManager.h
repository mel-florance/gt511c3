#pragma once

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

class UsersManager
{
public:
	UsersManager();
	~UsersManager();

	inline bool add_user(const User& user) {
		auto exists = std::find_if(users.begin(), users.end(), [=](const User& item) {
			return item.id == user.id;
		});

		if (exists == users.end()) {
			 users.push_back(user);
			 return true;
		}

		return false;
	}

	inline void remove_user(unsigned int id) {
		auto condition = [&](const User& user) { return user.id == id; };
		users.erase(std::remove_if(users.begin(), users.end(), condition), users.end());
	}

	inline void clear_users() {
		users.clear();
	}

	inline void update_user(size_t index, const User& data) {
		if (index >= 0 && index < users.size() && users.size() > 0) {
			auto user = users.at(index);
			user = std::move(data);
		}
	}

	inline int get_user_id(size_t index) {
		if (index >= 0 && index < users.size()) {
			auto user = users.at(index);
			return user.id;
		}

		return -1;
	}

	inline std::vector<User>& get_users() { return users; }

	inline std::vector<User>& load_users(const std::string& filename) {
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

	inline void save_users(const std::string& filename) {
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

private:
	std::vector<User> users;
};

