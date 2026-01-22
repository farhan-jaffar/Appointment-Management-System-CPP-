#include "UserFileHandler.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iostream>

const std::string UserFileHandler::USERS_FILE = "users.dat";
const std::string UserFileHandler::DOCTORS_FILE = "doctors.dat";
const std::string UserFileHandler::PATIENTS_FILE = "patients.dat";

UserFileHandler::UserFileHandler() {
    loadUsers();
}

bool UserFileHandler::loadUsers() {
    std::string filepath = Utils::getDataPath(USERS_FILE);
    if (!Utils::isFileValid(filepath)) {
        std::cout << "No existing users file found. Starting with empty user database.\n";
        return false;
    }

    try {
        std::ifstream file(filepath);
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string id, password, role;
            if (std::getline(ss, id, ',') && 
                std::getline(ss, password, ',') && 
                std::getline(ss, role, ',')) {
                users[id] = std::make_pair(password, role);
            } else {
                std::cerr << "Warning: Skipped malformed line in users file\n";
            }
        }
        file.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading users: " << e.what() << std::endl;
        return false;
    }
}

bool UserFileHandler::saveUsers() {
    std::string filepath = Utils::getDataPath(USERS_FILE);
    Utils::backupFile(filepath);  // Create backup before saving

    try {
        std::ofstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open users file for writing\n";
            return false;
        }

        for (const auto& pair : users) {
            file << pair.first << "," << pair.second.first << "," << pair.second.second << "\n";
        }
        file.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving users: " << e.what() << std::endl;
        return false;
    }
}

bool UserFileHandler::addUser(const std::string& id, const std::string& password, const std::string& role) {
    if (users.find(id) != users.end()) {
        return false;
    }
    users[id] = std::make_pair(password, role);
    return saveUsers();
}

bool UserFileHandler::removeUser(const std::string& id) {
    if (users.find(id) == users.end()) {
        return false;
    }
    users.erase(id);
    return saveUsers();
}

std::pair<std::string, std::string> UserFileHandler::getUser(const std::string& id) const {
    auto it = users.find(id);
    if (it != users.end()) {
        return it->second;
    }
    return std::make_pair("", "");
}

bool UserFileHandler::userExists(const std::string& id) const {
    return users.find(id) != users.end();
}

void UserFileHandler::saveUserData(const DoctorManager& doctorManager, const std::vector<Patient*>& patients) {
    // Save doctors
    std::string doctorPath = Utils::getDataPath(DOCTORS_FILE);
    Utils::backupFile(doctorPath);

    try {
        std::ofstream doctorFile(doctorPath);
        if (doctorFile.is_open()) {
            for (const auto& doctor : doctorManager.getAllDoctors()) {
                doctorFile << doctor->getId() << ","
                          << doctor->getName() << ","
                          << doctor->getSpecialization() << ","
                          << doctor->getLocation() << ","
                          << doctor->maxNormalSlots << ","
                          << doctor->maxEmergencySlots;

                // Save regular slots
                doctorFile << ";regular:";
                for (const auto& slot : doctor->getNormalSlots()) {
                    doctorFile << slot.time << ",";
                }

                // Save emergency slots
                doctorFile << ";emergency:";
                for (const auto& slot : doctor->getEmergencySlots()) {
                    doctorFile << slot.time << ",";
                }
                
                doctorFile << "\n";
            }
            doctorFile.close();
            std::cout << "Doctors data saved successfully.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error saving doctors: " << e.what() << std::endl;
    }

    // Save patients
    std::string patientPath = Utils::getDataPath(PATIENTS_FILE);
    Utils::backupFile(patientPath);

    try {
        std::ofstream patientFile(patientPath);
        if (patientFile.is_open()) {
            for (const auto& patient : patients) {
                if (patient) {  // Check for null pointer
                    patientFile << patient->getId() << ","
                               << patient->getName() << ","
                               << patient->getLocation() << "\n";
                }
            }
            patientFile.close();
            std::cout << "Patients data saved successfully.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error saving patients: " << e.what() << std::endl;
    }
}

void UserFileHandler::loadUserData(DoctorManager& doctorManager, std::vector<Patient*>& patients) {
    // Clear existing data
    doctorManager.clearDoctors();
    for (auto* patient : patients) {
        delete patient;
    }
    patients.clear();

    // Load doctors
    std::string doctorPath = Utils::getDataPath(DOCTORS_FILE);
    try {
        if (Utils::isFileValid(doctorPath)) {
            std::ifstream doctorFile(doctorPath);
            std::string line;
            while (std::getline(doctorFile, line)) {
                std::stringstream ss(line);
                std::string id, name, specialization, location, maxNormalStr, maxEmergencyStr;
                std::string regularSlots, emergencySlots;

                // Read basic doctor info
                if (std::getline(ss, id, ',') &&
                    std::getline(ss, name, ',') &&
                    std::getline(ss, specialization, ',') &&
                    std::getline(ss, location, ',') &&
                    std::getline(ss, maxNormalStr, ',') &&
                    std::getline(ss, maxEmergencyStr, ';')) {
                    
                    int maxNormal = std::stoi(maxNormalStr);
                    int maxEmergency = std::stoi(maxEmergencyStr);
                    Doctor* doctor = new Doctor(id, name, specialization, location, maxNormal, maxEmergency);

                    // Read regular slots
                    if (std::getline(ss, regularSlots, ';')) {
                        if (regularSlots.substr(0, 8) == "regular:") {
                            std::stringstream slotStream(regularSlots.substr(8));
                            std::string slotTime;
                            while (std::getline(slotStream, slotTime, ',')) {
                                if (!slotTime.empty()) {
                                    try {
                                        doctor->addSlot(slotTime);
                                    } catch (const std::invalid_argument& e) {
                                        std::cerr << "Error adding regular slot " << slotTime << " for Dr. " << doctor->getName() << ": " << e.what() << std::endl;
                                        delete doctor;
                                        throw;  // Re-throw to prevent adding invalid doctor
                                    }
                                }
                            }
                        }
                    }

                    // Read emergency slots
                    if (std::getline(ss, emergencySlots)) {
                        if (emergencySlots.substr(0, 10) == "emergency:") {
                            std::stringstream slotStream(emergencySlots.substr(10));
                            std::string slotTime;
                            while (std::getline(slotStream, slotTime, ',')) {
                                if (!slotTime.empty()) {
                                    try {
                                        doctor->addEmergencySlot(slotTime);
                                    } catch (const std::invalid_argument& e) {
                                        std::cerr << "Error adding emergency slot " << slotTime << " for Dr. " << doctor->getName() << ": " << e.what() << std::endl;
                                        delete doctor;
                                        throw;  // Re-throw to prevent adding invalid doctor
                                    }
                                }
                            }
                        }
                    }

                    doctorManager.addDoctor(doctor);
                }
            }
            doctorFile.close();
            std::cout << "Doctors data loaded successfully.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading doctors: " << e.what() << std::endl;
    }

    // Load patients
    std::string patientPath = Utils::getDataPath(PATIENTS_FILE);
    try {
        if (Utils::isFileValid(patientPath)) {
            std::ifstream patientFile(patientPath);
            std::string line;
            while (std::getline(patientFile, line)) {
                std::stringstream ss(line);
                std::string id, name, location;
                if (std::getline(ss, id, ',') &&
                    std::getline(ss, name, ',') &&
                    std::getline(ss, location, ',')) {
                    patients.push_back(new Patient(id, name, location));
                }
            }
            patientFile.close();
            std::cout << "Patients data loaded successfully.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading patients: " << e.what() << std::endl;
    }
} 