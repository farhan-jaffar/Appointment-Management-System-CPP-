#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>

using namespace std;

Patient::Patient(string id, string name, string location)
    : patientID(id), name(name), location(location), urgencyLevel(3) {}  // Default to medium urgency (3)

void Patient::setUrgencyLevel(int level) {
    try {
        Utils::validateOrThrow(Utils::isValidUrgencyLevel(level), 
            "Invalid urgency level. Must be between 1 (highest) and 4 (lowest).");
        urgencyLevel = level;
    } catch (const invalid_argument& e) {
        cerr << "Error setting urgency level: " << e.what() << endl;
        throw;  // Re-throw to let caller handle the error
    }
}

void Patient::addAppointment(const Appointment& app) {
    appointments.push_back(app);
}

bool Patient::cancelAppointment(const string& dateTime) {
    auto it = find_if(appointments.begin(), appointments.end(), [&](const Appointment& app) {
        return app.getAppointmentTime() == dateTime;
    });

    if (it != appointments.end()) {
        appointments.erase(it);
        cout << "Appointment on " << dateTime << " cancelled for " << name << ".\n";
        return true;
    }
    return false;
}

void Patient::addMedicalHistory(const string& record) {
    if (!record.empty()) {
        medicalHistory.push_back(record);
        cout << "Medical history added for " << name << ".\n";
    } else {
        throw invalid_argument("Empty record not allowed.");
    }
}

void Patient::viewMedicalHistory() const {
    cout << "Medical History for " << name << ":\n";
    if (medicalHistory.empty()) {
        cout << "  No records available.\n";
    } else {
        for (const auto& record : medicalHistory) {
            cout << "  - " << record << endl;
        }
    }
}

void Patient::viewAppointments() const {
    cout << "Appointments for " << name << ":\n";
    if (appointments.empty()) {
        cout << "  No appointments scheduled.\n";
        return;
    }
    for (const auto& app : appointments) {
        cout << "  - " << app.getAppointmentTime()
             << " with Dr. " << (app.doctor ? app.doctor->name : "Unknown");
        if (app.isMissed) cout << " [MISSED]";
        cout << endl;
    }
}

void Patient::requestEmergencyAppointment(Doctor* doctor) {
    doctor->addEmergencyPatient(this);
}

