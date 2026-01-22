# Appointment Management System (AMS)

A robust C++ console application designed to streamline healthcare scheduling. This system manages doctor and patient data, tracks medical histories, and uses graph-based algorithms to find the nearest healthcare providers.

## 🚀 Key Features

* **Smart Doctor Search:** Uses **Dijkstra’s Algorithm** to calculate the shortest distance between a patient's sector and available doctors.
* **Emergency Queueing:** Prioritizes patients based on four urgency levels (Critical, High, Medium, Low).
* **Medical History Tracking:** Maintains detailed, persistent records of patient diagnoses and treatments.
* **Persistent Data Storage:** Saves and loads all appointments and user records using flat-file databases (`.txt`).
* **Missed Appointment Management:** Dedicated system for marking and rebooking missed slots.



## 🗺️ Proximity Logic (Dijkstra's Algorithm)

The system represents the city as a weighted graph where nodes are sectors (e.g., G-9, F-10) and edges are travel "units" between them. When a patient seeks a doctor, the `NearestDoctorFinder` runs Dijkstra's algorithm to find the doctor with the lowest travel cost.

### Sector Map Example:
* **G-9 ↔ G-10:** 2 units
* **G-9 ↔ F-9:** 3 units
* **F-9 ↔ F-10:** 2 units

## 🛠️ Project Structure

To maintain a clean repository, ensure you only upload the source code and headers. Avoid uploading `.o` or `.exe` files.

| Category | Files |
| :--- | :--- |
| **Core Logic** | `main.cpp`, `Doctor.h/.cpp`, `Patient.h/.cpp`, `Slot.h/.cpp` |
| **Management** | `DoctorManager.h/.cpp`, `MedicalHistoryManager.h/.cpp`, `MissedAppointmentManager.h/.cpp` |
| **Utilities** | `Graph.h/.cpp`, `Utils.h/.cpp`, `NearestDoctorFinder.h/.cpp` |
| **Data Handling**| `UserFileHandler.h/.cpp`, `AppointmentFileHandler.h/.cpp` |



## 💻 Compilation and Usage

To compile the project using `g++`, run the following command in your terminal:

```bash
g++ *.cpp -o AppointmentSystem
./AppointmentSystem
