#ifndef VIP_PATIENT_H
#define VIP_PATIENT_H

#include <string>

#include "patient.h"

class VipPatient : public Patient {
private:
    double _money;
public:
    VipPatient(const std::string& name, PatientState state, double _money);

    std::string to_string() const override;
};

VipPatient::VipPatient(const std::string& name, PatientState state, double money)
    : Patient(name, state)
    , _money(money)
{}

std::string VipPatient::to_string() const {
    return Patient::to_string() + ", Money: " + std::to_string(this->_money);
}

#endif // VIP_PATIENT_H
