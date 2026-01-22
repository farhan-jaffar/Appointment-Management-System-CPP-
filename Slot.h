#ifndef SLOT_H
#define SLOT_H

#include <string>
#include "Appointment.h"

class Slot {
public:
    std::string time;
    bool isBooked = false;
    Appointment appointment;

    Slot(const std::string& time);
    bool isAvailable() const;
    void assignAppointment(const Appointment& app);
};

#endif

