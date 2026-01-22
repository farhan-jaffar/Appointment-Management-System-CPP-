#ifndef CANCEL_APPOINTMENT_MANAGER_H
#define CANCEL_APPOINTMENT_MANAGER_H

#include <string>
#include "Doctor.h"
#include "Patient.h"

class CancelAppointmentManager {
public:
    void cancelAppointment(Doctor* doctor, const std::string& patientID);
private:
    void removeFromRegularAppointments(Doctor* doctor, const std::string& patientID);
    void removeFromEmergencyAppointments(Doctor* doctor, const std::string& patientID);
    void freeUpSlot(Doctor* doctor, const std::string& time, bool isEmergency);
};

#endif 