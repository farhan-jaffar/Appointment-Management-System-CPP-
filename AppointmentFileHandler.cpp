#include "AppointmentFileHandler.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>

using namespace std;

void AppointmentFileHandler::saveAppointmentsToFile(const Doctor* doctor, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    // Save regular appointments
    queue<Appointment> tempQueue = doctor->regularAppointments; // Create a copy to preserve const correctness
    file << "REGULAR_APPOINTMENTS\n";
    file << tempQueue.size() << "\n";
    while (!tempQueue.empty()) {
        const auto& appt = tempQueue.front();
        file << appt.patient->getId() << " " << appt.date << " " << appt.time << "\n";
        tempQueue.pop();
    }

    // Save emergency appointments
    file << "EMERGENCY_APPOINTMENTS\n";
    file << doctor->appointments.size() << "\n";
    for (const auto& appt : doctor->appointments) {
        file << appt.patient->getId() << " " << appt.date << " " << appt.time << "\n";
    }

    file.close();
    cout << "Appointments saved to " << filename << " successfully.\n";
}

void AppointmentFileHandler::loadAppointmentsFromFile(Doctor* doctor, vector<bool>& patientFlags, int patientCount, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << " for reading.\n";
        return;
    }

    string section;
    int count;

    // Load regular appointments
    file >> section;
    if (section == "REGULAR_APPOINTMENTS") {
        file >> count;
        for (int i = 0; i < count; i++) {
            string patientId, date, time;
            file >> patientId >> date >> time;
            
            // Create and add appointment
            Appointment appt;
            appt.patient = nullptr; // Will be set later when patient object is available
            appt.date = date;
            appt.time = time;
            doctor->regularAppointments.push(appt);
        }
    }

    // Load emergency appointments
    file >> section;
    if (section == "EMERGENCY_APPOINTMENTS") {
        file >> count;
        for (int i = 0; i < count; i++) {
            string patientId, date, time;
            file >> patientId >> date >> time;
            
            // Create and add appointment
            Appointment appt;
            appt.patient = nullptr; // Will be set later when patient object is available
            appt.date = date;
            appt.time = time;
            doctor->appointments.push_back(appt);
        }
    }

    file.close();
    cout << "Appointments loaded from " << filename << " successfully.\n";
}


