#include "MissedAppointmentManager.h"
#include <iostream>
#include <queue>

using namespace std;

void MissedAppointmentManager::markAppointmentAsMissed(Doctor* doctor, const string& patientID) {
    if (!doctor) {
        cout << "Doctor is null.\n";
        return;
    }

    bool found = false;

    // Update in regular appointments queue
    queue<Appointment>& regularAppts = doctor->getRegularAppointments();
    queue<Appointment> tempQueue;

    while (!regularAppts.empty()) {
        Appointment app = regularAppts.front();
        regularAppts.pop();

        if (app.patient && app.patient->patientID == patientID && !app.isMissed && !found) {
            app.markMissed();
            found = true;
            cout << "Marked regular appointment for " << app.patient->name << " as missed.\n";
            
            // Also update in the main appointments vector
            for (auto& mainApp : doctor->appointments) {
                if (mainApp.patient && mainApp.patient->patientID == patientID &&
                    mainApp.date == app.date && mainApp.time == app.time) {
                    mainApp.markMissed();
                    break;
                }
            }
        }

        tempQueue.push(app);
    }

    regularAppts = tempQueue;

    // Check emergency appointments
    vector<Slot>& emergencySlots = doctor->getEmergencySlots();
    for (auto& slot : emergencySlots) {
        if (slot.isBooked && slot.appointment.patient && 
            slot.appointment.patient->patientID == patientID && 
            !slot.appointment.isMissed && !found) {
            slot.appointment.markMissed();
            found = true;
            cout << "Marked emergency appointment for " << slot.appointment.patient->name << " as missed.\n";
            
            // Also update in the main appointments vector
            for (auto& mainApp : doctor->appointments) {
                if (mainApp.patient && mainApp.patient->patientID == patientID &&
                    mainApp.date == slot.appointment.date && mainApp.time == slot.appointment.time) {
                    mainApp.markMissed();
                    break;
                }
            }
        }
    }

    if (!found) {
        cout << "No matching appointment found for patient ID: " << patientID << endl;
    }
}

void MissedAppointmentManager::rebookMissedAppointment(Doctor* doctor, Patient* patient) {
    if (!doctor || !patient) {
        cout << "Invalid input.\n";
        return;
    }

    bool found = false;

    // First, find the missed appointment in the main appointments vector
    Appointment* missedApp = nullptr;
    for (auto& app : doctor->appointments) {
        if (app.patient && app.patient->patientID == patient->patientID && app.isMissed) {
            missedApp = &app;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "No missed appointment found for " << patient->name << ".\n";
        return;
    }

    // Get new date and time
    string newDate, newTime;
    cout << "Enter new date (DD-MM-YYYY): ";
    getline(cin, newDate);
    cout << "Enter new time (HH:MM): ";
    getline(cin, newTime);

    if (!Utils::isValidDate(newDate) || !Utils::isValidTime(newTime)) {
        cout << "Invalid date or time format.\n";
        return;
    }

    // Try to find an available slot
    bool slotFound = false;
    
    // If it was a regular appointment
    if (!missedApp->isEmergency) {
        for (auto& slot : doctor->getNormalSlots()) {
            if (slot.time == newTime && !slot.isBooked) {
                // Update the appointment in all containers
                missedApp->date = newDate;
                missedApp->time = newTime;
                missedApp->isMissed = false;
                
                // Update in regular appointments queue
                queue<Appointment>& regularAppts = doctor->getRegularAppointments();
                queue<Appointment> tempQueue;
                while (!regularAppts.empty()) {
                    Appointment app = regularAppts.front();
                    regularAppts.pop();
                    if (app.patient && app.patient->patientID == patient->patientID && app.isMissed) {
                        app.date = newDate;
                        app.time = newTime;
                        app.isMissed = false;
                    }
                    tempQueue.push(app);
                }
                regularAppts = tempQueue;
                
                slot.isBooked = true;
                slotFound = true;
                cout << "Rebooked regular appointment for " << patient->name << ".\n";
                break;
            }
        }
    }
    // If it was an emergency appointment
    else {
        for (auto& slot : doctor->getEmergencySlots()) {
            if (!slot.isBooked) {
                // Update the appointment
                missedApp->date = newDate;
                missedApp->isMissed = false;
                
                // Move the appointment to the new slot
                slot.isBooked = true;
                slot.appointment = *missedApp;
                
                slotFound = true;
                cout << "Rebooked emergency appointment for " << patient->name << ".\n";
                break;
            }
        }
    }

    if (!slotFound) {
        cout << "Selected slot is not available. Please choose from:\n";
        doctor->displayAvailableSlots();
    }
}
