#include "MedicalHistoryManager.h"
#include <iostream>
#include <fstream>

using namespace std;

std::string MedicalHistoryManager::getHistoryFilename(const Patient* patient) const {
    return "medical_history_" + patient->getId() + ".txt";
}

void MedicalHistoryManager::addMedicalHistory(Patient* patient, const std::string& record) {
    std::ofstream file(getHistoryFilename(patient), std::ios::app);
    if (file.is_open()) {
        file << record << "\n";
        file.close();
        std::cout << "Medical history record added successfully.\n";
    } else {
        std::cout << "Error: Could not open medical history file.\n";
    }
}

void MedicalHistoryManager::viewMedicalHistory(const Patient* patient) const {
    std::ifstream file(getHistoryFilename(patient));
    if (file.is_open()) {
        std::cout << "\nMedical History for Patient " << patient->getName() << " (ID: " << patient->getId() << "):\n";
        std::cout << "----------------------------------------\n";
        std::string line;
        bool hasHistory = false;
        while (std::getline(file, line)) {
            std::cout << "- " << line << "\n";
            hasHistory = true;
        }
        if (!hasHistory) {
            std::cout << "No medical history records found.\n";
        }
        file.close();
    } else {
        std::cout << "No medical history records found.\n";
    }
}

