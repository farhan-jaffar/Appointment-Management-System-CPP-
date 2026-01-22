#include "DoctorManager.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cctype>

using namespace std;

DoctorManager::DoctorManager() {}

DoctorManager::~DoctorManager() {
    clearDoctors();
}

void DoctorManager::addDoctor(Doctor* doctor) {
    if (!doctor) {
        cerr << "Error: Cannot add null doctor pointer\n";
        return;
    }

    string id = doctor->getId();
    if (allDoctors.find(id) != allDoctors.end()) {
        cerr << "Error: Doctor with ID " << id << " already exists\n";
        return;
    }

    allDoctors[id] = doctor;
    doctorsBySpecialization[doctor->getSpecialization()].push_back(doctor);
    cout << "Doctor " << doctor->getName() << " added successfully.\n";
}

void DoctorManager::deleteDoctor(const string& doctorID) {
    auto it = allDoctors.find(doctorID);
    if (it == allDoctors.end()) {
        cerr << "Error: Doctor with ID " << doctorID << " not found\n";
        return;
    }

    Doctor* doctor = it->second;
    string spec = doctor->getSpecialization();

    // Remove from specialization map
    auto& specList = doctorsBySpecialization[spec];
    specList.erase(
        remove(specList.begin(), specList.end(), doctor),
        specList.end()
    );
    if (specList.empty()) {
        doctorsBySpecialization.erase(spec);
    }

    // Remove from main map and delete
    allDoctors.erase(it);
    delete doctor;
    cout << "Doctor removed successfully.\n";
}

Doctor* DoctorManager::getDoctorByID(const string& doctorID) const {
    auto it = allDoctors.find(doctorID);
    if (it == allDoctors.end()) {
        return nullptr;
    }
    return it->second;
}

Doctor* DoctorManager::getDoctorByName(const string& name) const {
    for (const auto& pair : allDoctors) {
        if (pair.second->getName() == name) {
            return pair.second;
        }
    }
    return nullptr;
}

vector<Doctor*> DoctorManager::getDoctorsBySpecialization(const string& specialization) const {
    auto it = doctorsBySpecialization.find(specialization);
    if (it == doctorsBySpecialization.end()) {
        return vector<Doctor*>();
    }
    return it->second;
}

vector<Doctor*> DoctorManager::getAllDoctors() const {
    vector<Doctor*> doctors;
    doctors.reserve(allDoctors.size());
    for (const auto& pair : allDoctors) {
        doctors.push_back(pair.second);
    }
    return doctors;
}

void DoctorManager::listAllDoctors() const {
    if (allDoctors.empty()) {
        cout << "No doctors registered in the system.\n";
        return;
    }

    cout << "\n=== Registered Doctors ===\n";
    for (const auto& pair : allDoctors) {
        Doctor* doc = pair.second;
        cout << "ID: " << doc->getId()
             << "\nName: Dr. " << doc->getName()
             << "\nSpecialization: " << doc->getSpecialization()
             << "\nLocation: " << doc->getLocation()
             << "\n-------------------\n";
    }
}

void DoctorManager::clearDoctors() {
    for (const auto& pair : allDoctors) {
        delete pair.second;
    }
    allDoctors.clear();
    doctorsBySpecialization.clear();
}

