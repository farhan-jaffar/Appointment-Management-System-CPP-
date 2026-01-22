#include "NearestDoctorFinder.h"
#include "Graph.h"
#include <queue>
#include <algorithm>
#include <iostream>
#include <climits>
#include <unordered_map>

using namespace std;

void NearestDoctorFinder::displayDoctorInfo(Doctor* doctor, int distance) {
    cout << "\nDoctor Details:\n";
    cout << "----------------------------------------\n";
    cout << "Name: Dr. " << doctor->getName() << "\n";
    cout << "Specialization: " << doctor->getSpecialization() << "\n";
    cout << "Location: " << doctor->getLocation() << "\n";
    cout << "Distance: " << distance << " units\n";
    doctor->displayAvailableSlots();
    cout << "----------------------------------------\n";
}

Doctor* NearestDoctorFinder::findNearestDoctor(Graph& city, const string& mySector, 
                                              const string& specialization, 
                                              const vector<Doctor*>& doctors) {
    unordered_map<string, int> distances;
    city.dijkstra(mySector, distances);

    Doctor* nearest = nullptr;
    int minDistance = INT_MAX;
    vector<pair<Doctor*, int>> availableDoctors;

    // Find all doctors with matching specialization
    for (Doctor* doc : doctors) {
        if (doc && doc->getSpecialization() == specialization) {
            string docSector = doc->getLocation();
            if (distances.count(docSector)) {
                availableDoctors.push_back({doc, distances[docSector]});
                if (distances[docSector] < minDistance) {
                    minDistance = distances[docSector];
                    nearest = doc;
                }
            }
        }
    }

    // Sort doctors by distance
    sort(availableDoctors.begin(), availableDoctors.end(),
         [](const auto& a, const auto& b) { return a.second < b.second; });

    // Display all available doctors sorted by distance
    if (!availableDoctors.empty()) {
        cout << "\nAvailable doctors with specialization '" << specialization << "' sorted by distance:\n";
        for (const auto& [doc, dist] : availableDoctors) {
            displayDoctorInfo(doc, dist);
        }
    } else {
        cout << "No doctors found with specialization '" << specialization << "'.\n";
    }

    return nearest;
}

void NearestDoctorFinder::findNearestDoctors(const std::string& patientLocation,
                                            const std::string& specialization,
                                            const std::vector<Doctor*>& doctors,
                                            const Graph& city) {
    std::unordered_map<std::string, int> distances;
    city.dijkstra(patientLocation, distances);

    std::vector<std::pair<Doctor*, int>> availableDoctors;
    for (auto doc : doctors) {
        if (doc->getSpecialization() == specialization) {
            availableDoctors.push_back({doc, distances[doc->getLocation()]});
        }
    }

    std::sort(availableDoctors.begin(), availableDoctors.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    displayResults(availableDoctors);
}

void NearestDoctorFinder::displayResults(const std::vector<std::pair<Doctor*, int>>& doctors) {
    if (doctors.empty()) {
        std::cout << "No doctors found with the specified specialization.\n";
        return;
    }

    std::cout << "\nNearest Doctors:\n";
    std::cout << "----------------------------------------\n";
    for (const auto& [doc, dist] : doctors) {
        std::cout << "Doctor: Dr. " << doc->getName() << "\n";
        std::cout << "Location: " << doc->getLocation() << "\n";
        std::cout << "Distance: " << dist << " units\n";
        doc->displayAvailableSlots();
        std::cout << "----------------------------------------\n";
    }
}

