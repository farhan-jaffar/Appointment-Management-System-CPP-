#ifndef DOCTORMANAGER_H
#define DOCTORMANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include "Doctor.h"

class DoctorManager {
private:
    std::unordered_map<std::string, Doctor*> allDoctors;
    std::unordered_map<std::string, std::vector<Doctor*>> doctorsBySpecialization;

public:
    DoctorManager();
    ~DoctorManager();

    void addDoctor(Doctor* doctor);
    void deleteDoctor(const std::string& doctorID);
    Doctor* getDoctorByID(const std::string& doctorID) const;
    Doctor* getDoctorByName(const std::string& name) const;
    std::vector<Doctor*> getDoctorsBySpecialization(const std::string& specialization) const;
    std::vector<Doctor*> getAllDoctors() const;
    void listAllDoctors() const;
    void clearDoctors();  // Added for proper cleanup
};

#endif // DOCTORMANAGER_H

