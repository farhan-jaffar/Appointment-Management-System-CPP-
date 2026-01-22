#ifndef MEDICAL_HISTORY_MANAGER_H
#define MEDICAL_HISTORY_MANAGER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "Patient.h"

class MedicalHistoryManager {
private:
    std::string getHistoryFilename(const Patient* patient) const;

public:
    void addMedicalHistory(Patient* patient, const std::string& record);
    void viewMedicalHistory(const Patient* patient) const;
};

#endif
