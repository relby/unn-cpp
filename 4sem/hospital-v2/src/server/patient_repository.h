#pragma once

#include <memory>

#include "queue.h"
#include "patient.h"
#include "vip_patient.h"

class PatientRepository {
private:
    Queue<std::shared_ptr<Patient>> _patients;

    PatientRepository();

public:
    PatientRepository(const PatientRepository&) = delete;
    PatientRepository(PatientRepository&&) = delete;
    PatientRepository& operator=(const PatientRepository&) = delete;
    PatientRepository& operator=(PatientRepository&&) = delete;

    static PatientRepository& instance();

    void add_patient(std::shared_ptr<Patient> patient);
    std::shared_ptr<Patient> get_patient();
    std::shared_ptr<Patient> next_patient() const;
    std::vector<std::shared_ptr<Patient>> all_patients() const;
};

PatientRepository& PatientRepository::instance() {
    static PatientRepository instance;
    return instance;
}

PatientRepository::PatientRepository(): _patients(Queue<std::shared_ptr<Patient>>()) {}

void PatientRepository::add_patient(std::shared_ptr<Patient> patient) {
    this->_patients.push(patient);
}

std::shared_ptr<Patient> PatientRepository::get_patient() {
    return this->_patients.pop();
}

std::shared_ptr<Patient> PatientRepository::next_patient() const {
    return this->_patients.top();
}
std::vector<std::shared_ptr<Patient>> PatientRepository::all_patients() const {
    return this->_patients.to_vector();
}
