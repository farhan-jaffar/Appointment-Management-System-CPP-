#include "CancelAppointmentManager.h"
#include <iostream>
#include <queue>

using namespace std;

void CancelAppointmentManager::cancelAppointment(Doctor* doctor, const string& patientID) {
    if (!doctor) {
        cout << "Doctor is null.\n";
        return;
    }

    bool found = false;

    // First try to find and remove from regular appointments
    removeFromRegularAppointments(doctor, patientID);

    // Then try emergency appointments
    removeFromEmergencyAppointments(doctor, patientID);

    // Finally, remove from the main appointments vector
    auto& appointments = doctor->appointments;
    for (auto it = appointments.begin(); it != appointments.end(); ) {
        if (it->patient && it->patient->patientID == patientID) {
            // Free up the corresponding slot
            freeUpSlot(doctor, it->time, it->isEmergency);
            
            cout << "Cancelled appointment for patient " << it->patient->name 
                 << " on " << it->date << " at " << it->time;
            if (it->isEmergency) cout << " [EMERGENCY]";
            cout << endl;
            
            it = appointments.erase(it);
            found = true;
        } else {
            ++it;
        }
    }

    if (!found) {
        cout << "No appointment found for patient ID: " << patientID << endl;
    }
}

void CancelAppointmentManager::removeFromRegularAppointments(Doctor* doctor, const string& patientID) {
    queue<Appointment>& regularAppts = doctor->getRegularAppointments();
    queue<Appointment> tempQueue;

    while (!regularAppts.empty()) {
        Appointment app = regularAppts.front();
        regularAppts.pop();

        if (!(app.patient && app.patient->patientID == patientID)) {
            tempQueue.push(app);
        }
    }

    regularAppts = tempQueue;
}

void CancelAppointmentManager::removeFromEmergencyAppointments(Doctor* doctor, const string& patientID) {
    vector<Slot>& emergencySlots = doctor->getEmergencySlots();
    for (auto& slot : emergencySlots) {
        if (slot.isBooked && slot.appointment.patient && 
            slot.appointment.patient->patientID == patientID) {
            slot.isBooked = false;
        }
    }
}

void CancelAppointmentManager::freeUpSlot(Doctor* doctor, const string& time, bool isEmergency) {
    if (isEmergency) {
        for (auto& slot : doctor->getEmergencySlots()) {
            if (slot.time == time) {
                slot.isBooked = false;
                break;
            }
        }
    } else {
        for (auto& slot : doctor->getNormalSlots()) {
            if (slot.time == time) {
                slot.isBooked = false;
                break;
            }
        }
    }
} 