#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <string>

// Forward declaration so Room can grant friendship later
class HotelSystem;

class Room {
protected:
    int roomNumber;
    std::string bedType;      // e.g., "King", "Twin", "Super King"
    std::string corridorZone; // e.g., "Mid-Corridor (Quiet)", "Near Lift", "Far End"
    int maxCapacity;
    double baseRate;          // Nightly base rate in GBP
    bool accessible;          // Wheelchair-accessible room flag
    int connectingRoomNumber; // 0 if standalone, or linked room number (e.g., 105)

private:
    static int totalRooms;    // Shared counter across all Room objects

public:
    Room();
    Room(int num, const std::string& bed, const std::string& zone,
         int capacity, double rate, bool isAccessible = false, int connectingNum = 0);
    virtual ~Room();

    // Pure virtual methods -> makes Room an Abstract Base Class
    virtual double calculateNightlyRate() const = 0;
    virtual std::string getRoomCategory() const = 0;
    virtual bool hasLoungeAccess() const = 0;

    // Virtual display method extended by child classes
    virtual void displayDetails() const;

    // Concept #2: Method Overloading (same name, different parameters)
    // 1) Checks if the room itself includes lounge access
    bool isLoungeEligible() const;
    // 2) Checks if room OR guest's loyalty tier (Diamond) grants free lounge access
    bool isLoungeEligible(const std::string& loyaltyTier) const;

    // Getters
    int getRoomNumber() const;
    std::string getBedType() const;
    std::string getCorridorZone() const;
    int getMaxCapacity() const;
    double getBaseRate() const;
    bool isAccessible() const;
    int getConnectingRoomNumber() const;

    static int getTotalRooms();

    // Operator Overloading
    bool operator==(const Room& other) const;
    bool operator<(const Room& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Room& r);

    // Friendship: allows HotelSystem (Duty Manager overrides) to adjust private room details
    friend class HotelSystem;
};

#endif