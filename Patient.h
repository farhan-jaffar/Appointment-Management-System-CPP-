#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <vector>

// Forward declarations
class Doctor;
class Appointment;

class Patient {
public:
    std::string patientID;
    std::string name;
    std::string location;
    std::string appointmentDate;
    std::string appointmentTime;
    int urgencyLevel;  // 1 is highest priority, 10 is lowest priority

    std::vector<Appointment> appointments;
    std::vector<std::string> medicalHistory;

    Patient(std::string id, std::string name, std::string location);

    // Getter methods
    std::string getId() const { return patientID; }
    std::string getName() const { return name; }
    std::string getLocation() const { return location; }
    int getUrgencyLevel() const { return urgencyLevel; }

    void setUrgencyLevel(int level);
    void addAppointment(const Appointment& app);
    bool cancelAppointment(const std::string& dateTime);
    void addMedicalHistory(const std::string& record);
    void viewMedicalHistory() const;
    void viewAppointments() const;
    void requestEmergencyAppointment(Doctor* doctor);
};

#endif
