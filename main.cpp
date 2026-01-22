#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <unordered_map>
#include <map>
#include "Doctor.h"
#include "Patient.h"
#include "DoctorManager.h"
#include "MedicalHistoryManager.h"
#include "AppointmentFileHandler.h"
#include "MissedAppointmentManager.h"
#include "UserFileHandler.h"
#include "Graph.h"
#include "Utils.h"
#include "NearestDoctorFinder.h"
#include "CancelAppointmentManager.h"

using namespace std;

// Helper function to find nearest doctor
void findNearestDoctor(const Graph& city, const string& patientLocation, const string& specialization, const vector<Doctor*>& doctors) {
    unordered_map<string, int> distances;
    city.dijkstra(patientLocation, distances);

    cout << "\nDoctors with specialization '" << specialization << "' sorted by distance:\n";
    cout << "----------------------------------------\n";

    // Filter doctors by specialization and sort by distance
    vector<pair<Doctor*, int>> availableDoctors;
    for (auto doc : doctors) {
        if (doc->getSpecialization() == specialization) {
            availableDoctors.push_back({doc, distances[doc->getLocation()]});
        }
    }

    // Sort by distance
    sort(availableDoctors.begin(), availableDoctors.end(),
         [](const auto& a, const auto& b) { return a.second < b.second; });

    if (availableDoctors.empty()) {
        cout << "No doctors found with specialization " << specialization << ".\n";
        return;
    }

    for (const auto& [doc, dist] : availableDoctors) {
        cout << "\nDoctor Details:\n";
        cout << "  Name: Dr. " << doc->getName() << "\n";
        cout << "  Location: " << doc->getLocation() << "\n";
        cout << "  Distance: " << dist << " units\n";
        doc->displayAvailableSlots();
        cout << "----------------------------------------\n";
    }
}

int main() {
    cout << "Press Enter to start the program...";
    cin.get();

    vector<Patient*> patients;
    DoctorManager doctorManager;
    MedicalHistoryManager historyManager;
    AppointmentFileHandler fileHandler;
    MissedAppointmentManager missedManager;
    CancelAppointmentManager cancelManager;
    UserFileHandler userHandler;
    Graph city;

    // Load saved user data at startup
    userHandler.loadUserData(doctorManager, patients);

    // Setup city sectors
    city.addEdge("G-9", "G-10", 2);
    city.addEdge("G-9", "F-9", 3);
    city.addEdge("F-9", "F-10", 2);
    city.addEdge("F-10", "F-8", 2);
    city.addEdge("F-8", "G-10", 3);

    int choice;
    do {
        cout << "\nAppointment Management System\n";
        cout << "1. Add Doctor\n";
        cout << "2. Add Patient\n";
        cout << "3. Book Regular Appointment\n";
        cout << "4. Book Emergency Appointment\n";
        cout << "5. Find Nearest Doctor\n";
        cout << "6. Mark Missed Appointment\n";
        cout << "7. View Appointments\n";
        cout << "8. View All Doctors\n";
        cout << "9. Rebook Missed Appointment\n";
        cout << "10. Patient Medical History\n";
        cout << "11. Save Data\n";
        cout << "12. Load Data\n";
        cout << "13. Cancel Appointment\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        
        string input;
        getline(cin, input);
        try {
            choice = stoi(input);
        } catch (...) {
            cout << "Invalid choice. Please try again.\n";
            continue;
        }

        if (choice == 1) {
            string id = Utils::getLineInput("Doctor ID: ");
            if (!Utils::isValidID(id)) {
                cout << "Invalid Doctor ID. Must be a positive integer.\n";
                continue;
            }

            string name = Utils::getLineInput("Doctor Name: ");
            if (!Utils::isValidName(name)) {
                cout << "Invalid name. Must be alphanumeric and maximum 30 characters.\n";
                continue;
            }

            string spec = Utils::getLineInput("Specialization: ");
            if (!Utils::isValidSpecialization(spec)) {
                cout << "Invalid specialization. Must be alphanumeric and maximum 20 characters.\n";
                continue;
            }

            string loc = Utils::getLineInput("Location (e.g., G-9): ");
            if (!Utils::isValidSector(loc)) {
                cout << "Invalid sector. Valid sectors are: G-9, G-10, F-8, F-9, F-10\n";
                continue;
            }

            int slots = Utils::getSafeInt("Regular Slots: ");
            if (slots <= 0) {
                cout << "Invalid number of slots. Must be positive.\n";
                continue;
            }

            int emSlots = Utils::getSafeInt("Emergency Slots: ");
            if (emSlots <= 0) {
                cout << "Invalid number of emergency slots. Must be positive.\n";
                continue;
            }

            Doctor* doc = new Doctor(id, name, spec, loc, slots, emSlots);
            bool addedSuccessfully = true;
            set<string> usedTimes;  // Using a set for faster lookup and unique values
            
            cout << "\nAdding regular slots:\n";
            // Add regular slots
            int regularSlotsAdded = 0;
            while (regularSlotsAdded < slots) {
                string slotTime = Utils::getLineInput("  Regular slot time (e.g. 10:00): ");
                if (!Utils::isValidTime(slotTime)) {
                    cout << "Invalid time format. Use HH:MM (e.g. 10:00)\n";
                    continue;
                }

                // Check if this time was already used
                if (usedTimes.count(slotTime) > 0) {
                    cout << "\nError: Time " << slotTime << " has already been used. Current slots:\n";
                    for (const auto& time : usedTimes) {
                        cout << "  - " << time << "\n";
                    }
                    continue;
                }

                try {
                    doc->addSlot(slotTime);
                    usedTimes.insert(slotTime);
                    regularSlotsAdded++;
                    cout << "  Regular slot " << regularSlotsAdded << "/" << slots << " added successfully.\n";
                } catch (const invalid_argument& e) {
                    cout << "\nError: " << e.what() << "\n";
                    string retry;
                    cout << "Would you like to try another time? (y/n): ";
                    getline(cin, retry);
                    if (retry != "y" && retry != "Y") {
                        addedSuccessfully = false;
                        break;
                    }
                }
            }

            // Only proceed with emergency slots if regular slots were added successfully
            if (addedSuccessfully) {
                cout << "\nAdding emergency slots:\n";
                int emergencySlotsAdded = 0;
                while (emergencySlotsAdded < emSlots) {
                    string emSlotTime = Utils::getLineInput("  Emergency slot time (e.g. 10:00): ");
                    if (!Utils::isValidTime(emSlotTime)) {
                        cout << "Invalid time format. Use HH:MM (e.g. 10:00)\n";
                        continue;
                    }

                    // Check if this time was already used
                    if (usedTimes.count(emSlotTime) > 0) {
                        cout << "\nError: Time " << emSlotTime << " has already been used. Current slots:\n";
                        for (const auto& time : usedTimes) {
                            cout << "  - " << time << "\n";
                        }
                        continue;
                    }

                    try {
                        doc->addEmergencySlot(emSlotTime);
                        usedTimes.insert(emSlotTime);
                        emergencySlotsAdded++;
                        cout << "  Emergency slot " << emergencySlotsAdded << "/" << emSlots << " added successfully.\n";
                    } catch (const invalid_argument& e) {
                        cout << "\nError: " << e.what() << "\n";
                        string retry;
                        cout << "Would you like to try another time? (y/n): ";
                        getline(cin, retry);
                        if (retry != "y" && retry != "Y") {
                            addedSuccessfully = false;
                            break;
                        }
                    }
                }
            }

            if (addedSuccessfully) {
                doctorManager.addDoctor(doc);
                userHandler.saveUserData(doctorManager, patients);
                cout << "\nDoctor " << name << " added successfully with the following schedule:\n";
                doc->displayAvailableSlots();
            } else {
                delete doc;  // Clean up if we failed to add all slots
                cout << "\nFailed to add doctor due to slot configuration issues.\n";
            }
        }
        else if (choice == 2) {
            string id = Utils::getLineInput("Patient ID: ");
            if (!Utils::isValidID(id)) {
                cout << "Invalid Patient ID. Must be a positive integer.\n";
                continue;
            }

            // Check if patient ID is unique
            bool idExists = false;
            for (const auto& p : patients) {
                if (p->getId() == id) {
                    idExists = true;
                    break;
                }
            }
            if (idExists) {
                cout << "Patient ID already exists. Please use a unique ID.\n";
                continue;
            }

            string name = Utils::getLineInput("Patient Name: ");
            if (!Utils::isValidName(name)) {
                cout << "Invalid name. Must be alphanumeric and maximum 30 characters.\n";
                continue;
            }

            string location = Utils::getLineInput("Patient Location (e.g., G-9): ");
            if (!Utils::isValidSector(location)) {
                cout << "Invalid sector. Valid sectors are: G-9, G-10, F-8, F-9, F-10\n";
                continue;
            }

            patients.push_back(new Patient(id, name, location));
            cout << "Patient added successfully.\n";
            userHandler.saveUserData(doctorManager, patients);
        }
        else if (choice == 3) {
            string pid = Utils::getLineInput("Patient ID: ");
            Patient* pat = nullptr;
            for (auto p : patients) {
                if (p->getId() == pid) {
                    pat = p;
                    break;
                }
            }
            if (!pat) {
                cout << "Patient not found.\n";
                continue;
            }

            string spec = Utils::getLineInput("Required Specialization: ");
            if (!Utils::isValidSpecialization(spec)) {
                cout << "Invalid specialization format.\n";
                continue;
            }
            
            // Get all doctors with the required specialization
            vector<Doctor*> availableDoctors;
            for (auto doc : doctorManager.getAllDoctors()) {
                if (doc->getSpecialization() == spec) {
                    availableDoctors.push_back(doc);
                }
            }

            if (availableDoctors.empty()) {
                cout << "No doctors found with specialization " << spec << ".\n";
                continue;
            }

            // Get appointment date and validate
            string date = Utils::getLineInput("Appointment date (DD-MM-YYYY): ");
            if (!Utils::isValidDate(date)) {
                cout << "Invalid date format. Use DD-MM-YYYY (e.g. 25-12-2024)\n";
                continue;
            }

            // Get appointment time and validate
            string time = Utils::getLineInput("Appointment time (HH:MM): ");
            if (!Utils::isValidTime(time)) {
                cout << "Invalid time format. Use HH:MM (e.g. 10:00)\n";
                continue;
            }

            // Sort doctors by distance from patient
            sort(availableDoctors.begin(), availableDoctors.end(), 
                [&city, &pat](Doctor* a, Doctor* b) {
                    unordered_map<string, int> distancesA, distancesB;
                    city.dijkstra(pat->getLocation(), distancesA);
                    city.dijkstra(pat->getLocation(), distancesB);
                    return distancesA[a->getLocation()] < distancesB[b->getLocation()];
                });

            // Try each doctor in order of distance
            Doctor* selectedDoctor = nullptr;
            for (auto doc : availableDoctors) {
                cout << "\nChecking availability with Dr. " << doc->getName() << "\n";
                cout << "----------------------------------------\n";
                cout << "Doctor Details:\n";
                cout << "  Name: Dr. " << doc->getName() << "\n";
                cout << "  Specialization: " << doc->getSpecialization() << "\n";
                cout << "  Location: " << doc->getLocation() << "\n";
                
                // Display available slots
                doc->displayAvailableSlots();

                // Check if there are any available slots
                if (doc->isSlotAvailable(date, time)) {
                    char choice;
                    cout << "\nWould you like to book with Dr. " << doc->getName() << "? (y/n): ";
                    cin >> choice;
                    cin.ignore();  // Clear the newline from input buffer

                    if (tolower(choice) == 'y') {
                        selectedDoctor = doc;
                        break;
                    }
                } else {
                    cout << "\nSorry, Dr. " << doc->getName() << " has no available slots at this time.\n";
                    cout << "Checking next available doctor...\n";
                }
            }

            if (!selectedDoctor) {
                cout << "\nNo doctors with available slots found for specialization " << spec << ".\n";
                continue;
            }

            selectedDoctor->bookRegularAppointment(pat, date, time);
        }
        else if (choice == 4) {
            string id = Utils::getLineInput("Enter patient ID: ");
            Patient* pat = nullptr;
            for (auto& p : patients) {
                if (p->getId() == id) {
                    pat = p;
                    break;
                }
            }

            if (!pat) {
                cout << "Patient not found.\n";
                continue;
            }

            string spec = Utils::getLineInput("Required Specialization: ");
            if (!Utils::isValidSpecialization(spec)) {
                cout << "Invalid specialization format.\n";
                continue;
            }

            // Get urgency level
            int urgencyLevel;
            bool validInput = false;
            do {
                try {
                    cout << "\nPlease enter urgency level:\n";
                    cout << "1 - Critical (Immediate attention required)\n";
                    cout << "2 - High (Urgent attention needed)\n";
                    cout << "3 - Medium (Attention needed soon)\n";
                    cout << "4 - Low (Can wait if necessary)\n";
                    cout << "Enter level (1-4): ";
                    
                    string input;
                    getline(cin, input);
                    
                    try {
                        urgencyLevel = stoi(input);
                        if (Utils::isValidUrgencyLevel(urgencyLevel)) {
                            validInput = true;
                        } else {
                            cout << "Error: Urgency level must be between 1 and 4.\n";
                        }
                    } catch (const invalid_argument& e) {
                        cout << "Error: Please enter a valid number.\n";
                    }
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << "\n";
                }
            } while (!validInput);

            try {
                pat->setUrgencyLevel(urgencyLevel);
            } catch (const exception& e) {
                cout << "Error setting urgency level: " << e.what() << "\n";
                continue;
            }

            // Find nearest doctors with the required specialization
            vector<Doctor*> availableDoctors;
            for (auto& doc : doctorManager.getAllDoctors()) {
                if (doc->getSpecialization() == spec) {
                    availableDoctors.push_back(doc);
                }
            }

            if (availableDoctors.empty()) {
                cout << "\nNo doctors found with specialization: " << spec << "\n";
                continue;
            }

            // Use NearestDoctorFinder to get sorted list of doctors by distance
            vector<pair<Doctor*, int>> sortedDoctors;
            unordered_map<string, int> distances;
            city.dijkstra(pat->getLocation(), distances);
            
            for (auto& doc : availableDoctors) {
                sortedDoctors.push_back({doc, distances[doc->getLocation()]});
            }
            
            sort(sortedDoctors.begin(), sortedDoctors.end(),
                [](const auto& a, const auto& b) { return a.second < b.second; });

            // Try to book with nearest available doctor
            bool appointmentBooked = false;
            for (const auto& [doc, distance] : sortedDoctors) {
                cout << "\nChecking emergency availability with Dr. " << doc->getName() << "\n";
                cout << "----------------------------------------\n";
                cout << "Doctor Details:\n";
                cout << "  Name: Dr. " << doc->getName() << "\n";
                cout << "  Specialization: " << doc->getSpecialization() << "\n";
                cout << "  Location: " << doc->getLocation() << "\n";
                cout << "  Distance: " << distance << " units\n";
                
                // Display available slots
                doc->displayAvailableSlots();

                if (doc->checkEmergencySlotAvailability()) {
                    try {
                        doc->assignEmergencyAppointment(pat);
                        appointmentBooked = true;
                        break;
                    } catch (const exception& e) {
                        cout << "Error assigning emergency appointment: " << e.what() << "\n";
                        cout << "Checking next nearest doctor...\n";
                        continue;
                    }
                } else {
                    cout << "\nDr. " << doc->getName() << " has no emergency slots available.\n";
                    cout << "Checking next nearest doctor...\n";
                }
            }

            if (!appointmentBooked) {
                cout << "\nNo emergency slots available with any doctor in specialization " << spec << ".\n";
                cout << "Patient has been added to the emergency queue.\n";
            }
        }
        else if (choice == 5) {
            string sector = Utils::getLineInput("Enter your sector (G-9/G-10/F-8/F-9/F-10): ");
            if (!Utils::isValidSector(sector)) {
                cout << "Invalid sector.\n";
                continue;
            }
            string spec = Utils::getLineInput("Enter specialization: ");
            Doctor* nearest = NearestDoctorFinder::findNearestDoctor(city, sector, spec, doctorManager.getAllDoctors());
            if (nearest) {
                cout << "\nYou can book an appointment with Dr. " << nearest->getName() << "\n";
            }
        }
        else if (choice == 6) {
            string did = Utils::getLineInput("Doctor ID: ");
            string pid = Utils::getLineInput("Patient ID: ");
            Doctor* doc = doctorManager.getDoctorByID(did);
            if (doc)
                missedManager.markAppointmentAsMissed(doc, pid);
            else
                cout << "Doctor not found.\n";
        }
        else if (choice == 7) {
            string did = Utils::getLineInput("Doctor ID: ");
            Doctor* doc = doctorManager.getDoctorByID(did);
            if (doc)
                doc->viewAppointments();
            else
                cout << "Doctor not found.\n";
        }
        else if (choice == 8) {
            auto doctors = doctorManager.getAllDoctors();
            if (doctors.empty()) {
                cout << "No doctors registered yet.\n";
                continue;
            }
            cout << "\nRegistered Doctors:\n";
            cout << "----------------------------------------\n";
            for (const auto& doc : doctors) {
                cout << "ID: " << doc->getId() << "\n";
                cout << "Name: Dr. " << doc->getName() << "\n";
                cout << "Specialization: " << doc->getSpecialization() << "\n";
                cout << "Location: " << doc->getLocation() << "\n";
                cout << "----------------------------------------\n";
            }
        }
        else if (choice == 9) {
            string did = Utils::getLineInput("Doctor ID: ");
            string pid = Utils::getLineInput("Patient ID: ");
            Doctor* doc = doctorManager.getDoctorByID(did);
            Patient* pat = nullptr;
            for (auto p : patients) if (p->getId() == pid) pat = p;
            if (doc && pat)
                missedManager.rebookMissedAppointment(doc, pat);
            else
                cout << "Doctor or patient not found.\n";
        }
        else if (choice == 10) {
            string pid = Utils::getLineInput("Patient ID: ");
            Patient* p = nullptr;
            for (auto pat : patients) {
                if (pat->getId() == pid) {
                    p = pat;
                    break;
                }
            }
            if (!p) {
                cout << "Patient not found.\n";
                continue;
            }
            cout << "1. View  2. Add: ";
            string input;
            getline(cin, input);
            try {
                int sub = stoi(input);
                if (sub == 1)
                    historyManager.viewMedicalHistory(p);
                else if (sub == 2)
                    historyManager.addMedicalHistory(p, Utils::getLineInput("Enter history record: "));
                else
                    cout << "Invalid choice.\n";
            } catch (...) {
                cout << "Invalid choice.\n";
            }
        }
        else if (choice == 11) {
            // Save appointments for each doctor
            auto doctors = doctorManager.getAllDoctors();
            for (const auto& doc : doctors) {
                string filename = "appointments_" + doc->getId() + ".txt";
                fileHandler.saveAppointmentsToFile(doc, filename);
            }
            // Also save user data
            userHandler.saveUserData(doctorManager, patients);
            cout << "All data saved successfully.\n";
        }
        else if (choice == 12) {
            // Load appointments for each doctor
            auto doctors = doctorManager.getAllDoctors();
            vector<bool> arr(patients.size());
            for (const auto& doc : doctors) {
                string filename = "appointments_" + doc->getId() + ".txt";
                fileHandler.loadAppointmentsFromFile(doc, arr, patients.size(), filename);
            }
            cout << "All appointments loaded successfully.\n";
        }
        else if (choice == 13) {
            string did = Utils::getLineInput("Doctor ID: ");
            string pid = Utils::getLineInput("Patient ID: ");
            Doctor* doc = doctorManager.getDoctorByID(did);
            if (doc) {
                cancelManager.cancelAppointment(doc, pid);
            } else {
                cout << "Doctor not found.\n";
            }
        }
        else if (choice != 0) {
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    // Cleanup
    for (auto pat : patients) delete pat;
    return 0;
}

