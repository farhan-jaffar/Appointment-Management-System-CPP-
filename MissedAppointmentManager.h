#ifndef MISSEDAPPOINTMENTMANAGER_H
#define MISSEDAPPOINTMENTMANAGER_H

#include <string>
#include "Doctor.h"
#include "Patient.h"

class MissedAppointmentManager {
public:
    void markAppointmentAsMissed(Doctor* doctor, const std::string& patientID);
    void rebookMissedAppointment(Doctor* doctor, Patient* patient);
};

#endif

