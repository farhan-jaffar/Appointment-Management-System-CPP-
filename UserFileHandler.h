#ifndef USERFILEHANDLER_H
#define USERFILEHANDLER_H

#include <string>
#include <vector>
#include <map>
#include "Doctor.h"
#include "Patient.h"
#include "DoctorManager.h"

class UserFileHandler {
public:
    UserFileHandler();
    bool loadUsers();
    bool saveUsers();
    bool addUser(const std::string& id, const std::string& password, const std::string& role);
    bool removeUser(const std::string& id);
    std::pair<std::string, std::string> getUser(const std::string& id) const;
    bool userExists(const std::string& id) const;

    // New methods for doctor and patient data
    static void saveUserData(const DoctorManager& doctorManager, const std::vector<Patient*>& patients);
    static void loadUserData(DoctorManager& doctorManager, std::vector<Patient*>& patients);
    
private:
    std::map<std::string, std::pair<std::string, std::string>> users; // ID -> (password, role)
    static const std::string USERS_FILE;
    static const std::string DOCTORS_FILE;
    static const std::string PATIENTS_FILE;
};

#endif 