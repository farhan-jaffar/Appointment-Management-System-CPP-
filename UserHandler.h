#ifndef USERHANDLER_H
#define USERHANDLER_H

#include <string>
#include <map>
#include <utility>

class UserHandler {
private:
    std::map<std::string, std::pair<std::string, std::string>> users;
    void loadUsers();
    void saveUsers();

public:
    UserHandler();
    bool userExists(const std::string& id) const;
    bool addUser(const std::string& id, const std::string& password, const std::string& role);
    std::pair<std::string, std::string> getUser(const std::string& id) const;
};

#endif // USERHANDLER_H 