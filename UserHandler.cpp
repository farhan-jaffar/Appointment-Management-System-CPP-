#include "UserHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>

UserHandler::UserHandler() {
    loadUsers();
}

void UserHandler::loadUsers() {
    std::ifstream file("users.txt");
    if (!file.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, password, role;
        if (std::getline(ss, id, ',') && 
            std::getline(ss, password, ',') && 
            std::getline(ss, role)) {
            users[id] = std::make_pair(password, role);
        }
    }
    file.close();
}

void UserHandler::saveUsers() {
    std::ofstream file("users.txt");
    if (!file.is_open()) {
        return;
    }

    for (const auto& user : users) {
        file << user.first << "," << user.second.first << "," << user.second.second << "\n";
    }
    file.close();
}

bool UserHandler::userExists(const std::string& id) const {
    return users.find(id) != users.end();
}

bool UserHandler::addUser(const std::string& id, const std::string& password, const std::string& role) {
    if (userExists(id)) {
        return false;
    }
    users[id] = std::make_pair(password, role);
    saveUsers();
    return true;
}

std::pair<std::string, std::string> UserHandler::getUser(const std::string& id) const {
    auto it = users.find(id);
    if (it != users.end()) {
        return it->second;
    }
    return std::make_pair("", "");
} 