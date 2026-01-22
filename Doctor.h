#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>
#include <vector>
#include <queue>
#include "Slot.h"
#include "Patient.h"
#include "Appointment.h"
#include "Utils.h"

class Doctor {
public:
    std::string doctorID;
    std::string name;
    std::string specialization;
    std::string location;

    int maxNormalSlots;
    int maxEmergencySlots;

    std::vector<Slot> normalSlots;
    std::vector<Slot> emergencySlots;
    std::queue<Appointment> regularAppointments;
    std::vector<Appointment> appointments;

    struct CompareUrgency {
        bool operator()(const Patient* a, const Patient* b) const {
            return a->urgencyLevel > b->urgencyLevel;
        }
    };

    std::priority_queue<Patient*, std::vector<Patient*>, CompareUrgency> emergencyQueue;

    Doctor(std::string id, std::string name, std::string spec, std::string loc, int normal, int emergency);

    // Getter methods
    std::string getId() const { return doctorID; }
    std::string getName() const { return name; }
    std::string getSpecialization() const { return specialization; }
    std::string getLocation() const { return location; }

    bool hasAvailableSlot() const;
    bool checkEmergencySlotAvailability() const;
    bool hasSlotOverlap(const std::string& time) const;
    void addSlot(const std::string& time);
    void addEmergencySlot(const std::string& time);
    void bookRegularAppointment(Patient* patient, const std::string& date, const std::string& time);
    void bookEmergencySlot(Patient* patient, const std::string& date);
    void addEmergencyPatient(Patient* patient);
    void viewAppointments() const;
    void assignEmergencyAppointment(Patient* patient);

    // New methods for slot management
    void displayAvailableSlots() const;
    bool isSlotAvailable(const std::string& time) const;
    bool isSlotAvailable(const std::string& date, const std::string& time) const;

    // For other modules:
    std::queue<Appointment>& getRegularAppointments();
    std::vector<Slot>& getNormalSlots();
    std::vector<Slot>& getEmergencySlots();
};

#endif

