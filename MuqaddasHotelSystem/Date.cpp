#include "Date.h"
#include "HotelExceptions.h"
#include <iomanip>

// Default constructor sets a sensible default date
Date::Date() : day(1), month(1), year(2026) {}

// Parameterised constructor validates the date and throws an exception if invalid
Date::Date(int d, int m, int y) {
    if (!isValidDate(d, m, y)) {
        throw InvalidDateException("Invalid date entered: " +
                                   std::to_string(d) + "/" +
                                   std::to_string(m) + "/" +
                                   std::to_string(y));
    }
    day = d;
    month = m;
    year = y;
}

bool Date::isLeapYear(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int Date::daysInMonth(int m, int y) {
    switch (m) {
        case 2: return isLeapYear(y) ? 29 : 28;
        case 4: case 6: case 9: case 11: return 30;
        default: return 31;
    }
}

bool Date::isValidDate(int d, int m, int y) {
    if (y < 1900 || y > 2100) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > daysInMonth(m, y)) return false;
    return true;
}

// Converts date into total days since 01/01/1900 for accurate night calculations
int Date::toTotalDays() const {
    int total = 0;
    for (int y = 1900; y < year; ++y) {
        total += isLeapYear(y) ? 366 : 365;
    }
    for (int m = 1; m < month; ++m) {
        total += daysInMonth(m, year);
    }
    total += day;
    return total;
}

int Date::getDay() const { return day; }
int Date::getMonth() const { return month; }
int Date::getYear() const { return year; }

bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}

bool Date::operator<(const Date& other) const {
    return this->toTotalDays() < other.toTotalDays();
}

int Date::operator-(const Date& other) const {
    return this->toTotalDays() - other.toTotalDays();
}

std::ostream& operator<<(std::ostream& os, const Date& d) {
    os << std::setfill('0') << std::setw(2) << d.day << "/"
       << std::setw(2) << d.month << "/"
       << d.year;
    return os;
}

bool Date::datesOverlap(const Date& startA, const Date& endA,
                        const Date& startB, const Date& endB) {
    // Two bookings overlap if Booking A starts before Booking B checks out,
    // AND Booking B starts before Booking A checks out.
    // (Same-day check-out / check-in is allowed!)
    return (startA < endB) && (startB < endA);
}