#ifndef HOTEL_EXCEPTIONS_H
#define HOTEL_EXCEPTIONS_H

#include <exception>
#include <string>

// Base hotel exception inheriting from C++ standard std::exception
class InvalidDateException : public std::exception {
private:
    std::string message; // Stores the custom error description

public:
    explicit InvalidDateException(const std::string& msg);
    // 'noexcept override' ensures compatibility with std::exception::what()
    const char* what() const noexcept override;
};

// Thrown when dates overlap (double booking) or room capacity is exceeded
class OverbookingException : public std::exception {
private:
    std::string message;

public:
    explicit OverbookingException(const std::string& msg);
    const char* what() const noexcept override;
};

// Thrown when an invalid upgrade is attempted (e.g., moving an accessible booking)
class UpgradeConflictException : public std::exception {
private:
    std::string message;

public:
    explicit UpgradeConflictException(const std::string& msg);
    const char* what() const noexcept override;
};

#endif