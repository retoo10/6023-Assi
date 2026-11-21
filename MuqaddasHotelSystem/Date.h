#ifndef DATE_H
#define DATE_H

#include <iostream>

class Date {
private:
    int day;
    int month;
    int year;

    // Private helper methods (Encapsulation: internal validation logic hidden from outside)
    static bool isLeapYear(int y);
    static int daysInMonth(int m, int y);
    static bool isValidDate(int d, int m, int y);
    int toTotalDays() const;

public:
    // Constructors
    Date();                         // Default constructor
    Date(int d, int m, int y);      // Parameterised constructor with validation

    // Getters
    int getDay() const;
    int getMonth() const;
    int getYear() const;

    // Operator Overloading
    bool operator==(const Date& other) const; // Compare if two dates are the same
    bool operator<(const Date& other) const;  // Check if this date is earlier than another
    int operator-(const Date& other) const;   // Calculate number of nights: checkOut - checkIn

    // Friend function to print Date cleanly using std::cout << myDate
    friend std::ostream& operator<<(std::ostream& os, const Date& d);

    // Static utility method to check if two date ranges overlap (prevents double bookings)
    static bool datesOverlap(const Date& startA, const Date& endA,
                             const Date& startB, const Date& endB);
};

#endif