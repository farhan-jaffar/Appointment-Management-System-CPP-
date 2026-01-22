#include "Appointment.h"
#include "Doctor.h"
#include "Patient.h"
#include <string>

using namespace std;

Appointment::Appointment() 
    : date(""), time(""), doctor(nullptr), patient(nullptr), isEmergency(false), isMissed(false) {}

Appointment::Appointment(const string& date, const string& time, Doctor* doc, Patient* pat, bool isEmergency)
    : date(date), time(time), doctor(doc), patient(pat), isEmergency(isEmergency), isMissed(false) {}

string Appointment::getAppointmentTime() const {
    return date + " " + time;
}

void Appointment::markMissed() {
    isMissed = true;
}

bool Appointment::equals(const Appointment& other) const {
    return date == other.date && 
           time == other.time && 
           doctor == other.doctor && 
           patient == other.patient;
}

