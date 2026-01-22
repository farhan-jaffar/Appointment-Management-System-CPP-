#include "Slot.h"
#include <string>

using namespace std;

Slot::Slot(const string& time) : time(time) {}

bool Slot::isAvailable() const {
    return !isBooked;
}

void Slot::assignAppointment(const Appointment& app) {
    if (isAvailable()) {
        appointment = app;
        isBooked = true;
    }
}

