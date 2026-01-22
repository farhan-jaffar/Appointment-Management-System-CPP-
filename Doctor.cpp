#include "Doctor.h"
#include <iostream>
#include <iomanip>

using namespace std;

Doctor::Doctor(string id, string name, string spec, string loc, int normal, int emergency)
    : doctorID(id), name(name), specialization(spec), location(loc),
      maxNormalSlots(normal), maxEmergencySlots(emergency) {}

bool Doctor::hasAvailableSlot() const {
    for (const auto& slot : normalSlots) {
        if (!slot.isBooked) {
            return true;
        }
    }
    return false;
}

bool Doctor::checkEmergencySlotAvailability() const {
    for (const auto& slot : emergencySlots) {
        if (!slot.isBooked) {
            return true;
        }
    }
    return false;
}

bool Doctor::hasSlotOverlap(const string& newTime) const {
    // First check regular slots
    for (const auto& slot : normalSlots) {
        if (slot.time == newTime) {
            cout << "Debug: Found overlap with regular slot " << slot.time << endl;
            return true;
        }
    }
    
    // Then check emergency slots
    for (const auto& slot : emergencySlots) {
        if (slot.time == newTime) {
            cout << "Debug: Found overlap with emergency slot " << slot.time << endl;
            return true;
        }
    }
    
    return false;
}

void Doctor::addSlot(const string& time) {
    Utils::validateOrThrow(Utils::isValidTime(time), "Invalid time format");
    
    // Check for overlaps with both regular and emergency slots
    if (hasSlotOverlap(time)) {
        throw invalid_argument("Cannot add slot at " + time + " - this time is already taken by another slot");
    }
    
    if (normalSlots.size() >= maxNormalSlots) {
        throw invalid_argument("Maximum number of regular slots (" + to_string(maxNormalSlots) + ") reached");
    }

    normalSlots.push_back(Slot(time));
    cout << "Regular slot added: " << time << "\n";
}

void Doctor::addEmergencySlot(const string& time) {
    Utils::validateOrThrow(Utils::isValidTime(time), "Invalid time format");
    
    // Check for overlaps with both regular and emergency slots
    if (hasSlotOverlap(time)) {
        throw invalid_argument("Cannot add emergency slot at " + time + " - this time is already taken by another slot");
    }
    
    if (emergencySlots.size() >= maxEmergencySlots) {
        throw invalid_argument("Maximum number of emergency slots (" + to_string(maxEmergencySlots) + ") reached");
    }

    emergencySlots.push_back(Slot(time));
    cout << "Emergency slot added: " << time << "\n";
}

void Doctor::displayAvailableSlots() const {
    cout << "\nAvailable slots for Dr. " << name << ":\n";
    
    // Display regular slots
    cout << "Regular slots:\n";
    bool hasRegularSlots = false;
    for (const auto& slot : normalSlots) {
        if (!slot.isBooked) {
            cout << "  - " << slot.time << "\n";
            hasRegularSlots = true;
        }
    }
    if (!hasRegularSlots) {
        cout << "  No regular slots available.\n";
    }

    // Display emergency slots
    cout << "\nEmergency slots:\n";
    bool hasEmergencySlots = false;
    for (const auto& slot : emergencySlots) {
        if (!slot.isBooked) {
            cout << "  - " << slot.time << "\n";
            hasEmergencySlots = true;
        }
    }
    if (!hasEmergencySlots) {
        cout << "  No emergency slots available.\n";
    }
}

bool Doctor::isSlotAvailable(const string& time) const {
    Utils::validateOrThrow(Utils::isValidTime(time), "Invalid time format");
    for (const auto& slot : normalSlots) {
        if (slot.time == time && !slot.isBooked) {
            return true;
        }
    }
    return false;
}

bool Doctor::isSlotAvailable(const string& date, const string& time) const {
    Utils::validateOrThrow(Utils::isValidDate(date), "Invalid date format");
    Utils::validateOrThrow(Utils::isValidTime(time), "Invalid time format");
    
    // First check if the slot time exists and is available
    bool slotExists = false;
    for (const auto& slot : normalSlots) {
        if (slot.time == time) {
            slotExists = true;
            if (slot.isBooked) return false;
            break;
        }
    }
    if (!slotExists) return false;

    // Then check if there's no appointment at this date and time
    string dateTime = date + " " + time;
    for (const auto& app : appointments) {
        if (app.getAppointmentTime() == dateTime) {
            return false;
        }
    }
    return true;
}

void Doctor::bookRegularAppointment(Patient* patient, const string& date, const string& time) {
    Utils::validateOrThrow(patient != nullptr, "Invalid patient");
    Utils::validateOrThrow(Utils::isValidDate(date), "Invalid date format");
    Utils::validateOrThrow(Utils::isValidTime(time), "Invalid time format");

    if (!isSlotAvailable(date, time)) {
        cout << "The requested time " << time << " is not available.\n";
        cout << "Please choose from the following available slots:\n";
        displayAvailableSlots();
        return;
    }

    // Find and mark the slot as booked
    for (auto& slot : normalSlots) {
        if (slot.time == time && !slot.isBooked) {
            slot.isBooked = true;
            break;
        }
    }

    Appointment app(date, time, this, patient);
    regularAppointments.push(app);
    appointments.push_back(app);
    patient->addAppointment(app);
    cout << "\nAppointment confirmed!\n";
    cout << "--------------------\n";
    cout << "Patient: " << patient->name << "\n";
    cout << "Doctor: Dr. " << name << "\n";
    cout << "Date: " << date << "\n";
    cout << "Time: " << time << "\n";
    cout << "Location: " << location << "\n";
    cout << "Specialization: " << specialization << "\n";
    cout << "--------------------\n";
}

void Doctor::assignEmergencyAppointment(Patient* patient) {
    Utils::validateOrThrow(patient != nullptr, "Invalid patient");

    // Get current date
    time_t now = time(0);
    tm* ltm = localtime(&now);
    string currentDate = to_string(ltm->tm_mday) + "-" + 
                        to_string(1 + ltm->tm_mon) + "-" + 
                        to_string(1900 + ltm->tm_year);

    // Add patient to emergency queue
    emergencyQueue.push(patient);

    // Try to book immediately if slots are available
    if (checkEmergencySlotAvailability()) {
        Patient* topPatient = emergencyQueue.top();
        emergencyQueue.pop();
        bookEmergencySlot(topPatient, currentDate);
    } else {
        cout << "\nNo emergency slots available at the moment. Patient " 
             << patient->getName() << " has been added to the emergency queue with urgency level " 
             << patient->getUrgencyLevel() << ".\n";
    }
}

void Doctor::bookEmergencySlot(Patient* patient, const string& date) {
    Utils::validateOrThrow(patient != nullptr, "Invalid patient");
    Utils::validateOrThrow(Utils::isValidDate(date), "Invalid date format");

    if (!checkEmergencySlotAvailability()) {
        cout << "\nSorry, no emergency slots are available with Dr. " << name << ".\n";
        cout << "Please try another doctor for emergency consultation.\n";
        return;
    }

    // Find first available emergency slot
    for (auto& slot : emergencySlots) {
        if (slot.isAvailable()) {
            slot.isBooked = true;
            Appointment app(date, slot.time, this, patient, true);
            slot.assignAppointment(app);
            appointments.push_back(app);
            patient->addAppointment(app);
            
            cout << "\nEmergency Appointment confirmed!\n";
            cout << "--------------------\n";
            cout << "Patient: " << patient->name << "\n";
            cout << "Doctor: Dr. " << name << "\n";
            cout << "Date: " << date << "\n";
            cout << "Time: " << slot.time << "\n";
            cout << "Location: " << location << "\n";
            cout << "Specialization: " << specialization << "\n";
            cout << "Type: EMERGENCY\n";
            cout << "Urgency Level: " << patient->getUrgencyLevel() << "\n";
            cout << "--------------------\n";
            return;
        }
    }
}

void Doctor::addEmergencyPatient(Patient* patient) {
    emergencyQueue.push(patient);
    cout << "Emergency patient added: " << patient->name << endl;

    if (checkEmergencySlotAvailability()) {
        Patient* topPatient = emergencyQueue.top();
        emergencyQueue.pop();
        bookEmergencySlot(topPatient, topPatient->appointmentDate);
    }
}

void Doctor::viewAppointments() const {
    cout << "Appointments for Dr. " << name << ":\n";
    if (appointments.empty()) {
        cout << "  No appointments scheduled.\n";
        return;
    }

    // First show regular appointments
    queue<Appointment> tempQueue = regularAppointments;
    while (!tempQueue.empty()) {
        const Appointment& app = tempQueue.front();
        cout << app.patient->name << " at " << app.time << " on " << app.date;
        if (app.isMissed) cout << " [MISSED]";
        cout << endl;
        tempQueue.pop();
    }

    // Then show emergency appointments
    for (const auto& slot : emergencySlots) {
        if (slot.isBooked) {
            const Appointment& app = slot.appointment;
            cout << app.patient->name << " at " << app.time << " on " << app.date;
            if (app.isMissed) cout << " [MISSED]";
            cout << " [EMERGENCY]" << endl;
        }
    }
}

std::queue<Appointment>& Doctor::getRegularAppointments() {
    return regularAppointments;
}

vector<Slot>& Doctor::getNormalSlots() {
    return normalSlots;
}

vector<Slot>& Doctor::getEmergencySlots() {
    return emergencySlots;
}

