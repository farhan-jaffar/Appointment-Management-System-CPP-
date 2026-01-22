#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <string>

class Doctor;
class Patient;

class Appointment {
public:
    std::string date;
    std::string time;
    Doctor* doctor;
    Patient* patient;
    bool isMissed = false;
    bool isEmergency = false;

    Appointment();
    Appointment(const std::string& date, const std::string& time, Doctor* doctor, Patient* patient, bool emergency = false);

    std::string getAppointmentTime() const;
    void markMissed();
    bool equals(const Appointment& other) const;
};

#endif

