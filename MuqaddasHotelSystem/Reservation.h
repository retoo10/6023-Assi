#ifndef RESERVATION_H
#define RESERVATION_H

#include <iostream>
#include <string>
#include "Date.h"
#include "Guest.h"
#include "Room.h"

class HotelSystem; // Forward declaration for friendship

// Concept #7: Function Pointer type for dynamic occupancy-based pricing
using OccupancyPricingFn = double (*)(double roomSubtotal);

// Standalone pricing strategy functions compatible with OccupancyPricingFn
double lowOccupancyDiscount(double roomSubtotal);  // 10% off when hotel is < 50% full
double midOccupancyDiscount(double roomSubtotal);  // 5% off when hotel is 50-79% full
double peakOccupancyStandard(double roomSubtotal); // Full rate when hotel is 80%+ full

class Reservation {
private:
    int bookingId;
    Guest* guest;        // Concept #6: Pointer to Guest
    Room* room;          // Concept #6: Polymorphic pointer to Room
    Date bookingDate;
    Date checkInDate;
    Date checkOutDate;
    bool loungeRequested;
    std::string occasion;

    static int nextBookingId;
    static int activeBookingsCount;
    static constexpr double LOUNGE_SUPPLEMENT_PER_NIGHT = 35.0;

public:
    Reservation(Guest* g, Room* r, const Date& bookedOn,
                const Date& checkIn, const Date& checkOut,
                bool requestLounge = false, const std::string& specialOccasion = "None");
    ~Reservation();

    int getNights() const;

    // Concept #2: Overloading + Concept #7: Function Pointer parameter
    double calculateTotalBill() const;
    double calculateTotalBill(OccupancyPricingFn pricingRule) const;
    double calculateLoungeFee() const;

    // Getters
    int getBookingId() const;
    Guest* getGuest() const;
    Room* getRoom() const;
    Date getBookingDate() const;
    Date getCheckInDate() const;
    Date getCheckOutDate() const;
    bool isLoungeRequested() const;
    std::string getOccasion() const;

    static int getActiveBookingsCount();

    // Concept #10: Operator Overloading
    bool operator==(const Reservation& other) const;
    bool operator<(const Reservation& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Reservation& res);

    // Concept #3: Friendship (allows Duty Manager / HotelSystem to upgrade rooms directly)
    friend class HotelSystem;
};

#endif