#ifndef NEAREST_DOCTOR_FINDER_H
#define NEAREST_DOCTOR_FINDER_H

#include <string>
#include <vector>
#include "Doctor.h"
#include "Graph.h"

class NearestDoctorFinder {
public:
    static Doctor* findNearestDoctor(Graph& city, const std::string& mySector, 
                                   const std::string& specialization, 
                                   const std::vector<Doctor*>& doctors);
    
    static void findNearestDoctors(const std::string& patientLocation,
                                  const std::string& specialization,
                                  const std::vector<Doctor*>& doctors,
                                  const Graph& city);
private:
    static void displayDoctorInfo(Doctor* doctor, int distance);
    static void displayResults(const std::vector<std::pair<Doctor*, int>>& doctors);
};

#endif

