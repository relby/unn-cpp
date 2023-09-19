#ifndef PATIENT_H
#define PATIENT_H

#include <string>

class parse_error: public std::exception
{
protected:
    std::string _msg;
public:
    explicit parse_error(const char* message)
        : _msg(message) {}

    explicit parse_error(const std::string& message)
        : _msg(message) {}

    virtual ~parse_error() noexcept {}

    virtual const char* what() const noexcept {
       return this->_msg.c_str();
    }

};

enum PatientState {
    NORMAL = 0,
    WARNING,
    CRITICAL,
};

std::string patient_state_to_string(PatientState state) {
    switch (state) {
        case PatientState::NORMAL:
            return "NORMAL";
        case PatientState::WARNING:
            return "WARNING";
        case PatientState::CRITICAL:
            return "CRITICAL";
        default:
            throw std::logic_error("Unreachable code");
    }
}

PatientState string_to_patient_state(const std::string& state) {
    if (state == "NORMAL") {
        return PatientState::NORMAL;
    } else if (state == "WARNING") {
        return PatientState::WARNING;
    } else if (state == "CRITICAL") {
        return PatientState::CRITICAL;
    } else {
        throw parse_error("Can't parse patient state");
    }
}

class Patient {
private:
    std::string _name;
    PatientState _state;
public:
    Patient(const std::string& name, PatientState state);
    const std::string& get_name() const;
    PatientState get_state() const;
    virtual std::string to_string() const;

    friend std::ostream& operator<<(std::ostream& stream, const Patient& patient);
};

Patient::Patient(const std::string& name, PatientState state)
    : _name(name)
    , _state(state)
{}

const std::string& Patient::get_name() const {
    return this->_name;
}

PatientState Patient::get_state() const {
    return this->_state;
}
std::string Patient::to_string() const {
    return "Name: " + this->_name + ", State: " + patient_state_to_string(this->_state);
}

std::ostream& operator<<(std::ostream& stream, const Patient& patient) {
    stream << patient.to_string();
    return stream;
};

#endif // PATIENT_H
