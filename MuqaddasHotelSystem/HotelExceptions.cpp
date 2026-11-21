#include "HotelExceptions.h"

InvalidDateException::InvalidDateException(const std::string& msg) : message(msg) {}

const char* InvalidDateException::what() const noexcept {
    return message.c_str();
}

OverbookingException::OverbookingException(const std::string& msg) : message(msg) {}

const char* OverbookingException::what() const noexcept {
    return message.c_str();
}

UpgradeConflictException::UpgradeConflictException(const std::string& msg) : message(msg) {}

const char* UpgradeConflictException::what() const noexcept {
    return message.c_str();
}