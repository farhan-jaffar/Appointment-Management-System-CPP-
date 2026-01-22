#ifndef APPOINTMENT_FILE_HANDLER_H
#define APPOINTMENT_FILE_HANDLER_H

#include <string>
#include <vector>
#include "Doctor.h"
#include "Patient.h"

class AppointmentFileHandler {
public:
    void saveAppointmentsToFile(const Doctor* doctor, const std::string& filename);
    void loadAppointmentsFromFile(Doctor* doctor, std::vector<bool>& patientFlags, int patientCount, const std::string& filename);
};

#endif
