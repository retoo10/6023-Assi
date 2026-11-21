#ifndef HOTEL_SYSTEM_H
#define HOTEL_SYSTEM_H

#include <string>
#include "RoomTree.h"
#include "ReservationList.h"
#include "StandardRoom.h"
#include "ExecutiveRoom.h"
#include "SuiteRoom.h"

class HotelSystem {
private:
    RoomTree roomInventory;          // BST storing all Room* objects sorted by roomNumber
    ReservationList activeBookings;  // Linked List storing all active Reservation* objects
    int totalRoomsLoaded;

    static std::string hotelName;    // Static member for hotel name

    // Private helper: checks if a room already has an overlapping booking for given dates
    bool hasDateConflict(int roomNumber, const Date& checkIn, const Date& checkOut) const;

    // Private recursive helper: searches the RoomTree in-order for an available room by category
    Room* findAvailableInTree(RoomNode* node, const std::string& category,
                              int partySize, bool needsAccessible,
                              const Date& checkIn, const Date& checkOut) const;

public:
    HotelSystem();
    ~HotelSystem() = default; // RoomTree & ReservationList destructors clean up their own nodes

    static std::string getHotelName();

    // Room inventory management (wraps RoomTree operations)
    void addRoom(Room* newRoom);
    Room* findRoom(int roomNumber) const;
    bool removeRoomForMaintenance(int roomNumber);
    void displayAllRoomsSorted() const;

    // Concept #2: Overloaded bookRoom methods (save booking for specific dates)
    // Overload 1: Book a specific room number
    Reservation* bookRoom(Guest* guest, int roomNumber, const Date& bookedOn,
                          const Date& checkIn, const Date& checkOut,
                          bool requestLounge = false, const std::string& occasion = "None");

    // Overload 2: Book the first available room matching a category (e.g. "Executive King")
    Reservation* bookRoom(Guest* guest, const std::string& desiredCategory,
                          const Date& bookedOn, const Date& checkIn, const Date& checkOut,
                          bool requestLounge = false, const std::string& occasion = "None");

    // Booking management (wraps ReservationList operations)
    Reservation* findBooking(int bookingId) const;
    bool cancelBooking(int bookingId);
    bool checkOutFirstBooking();
    void displayAllBookings() const;

    // Concept #3 (Friendship) + Concept #7 (Function Pointers):
    // Automatically picks occupancy pricing function pointer based on current bookings
    OccupancyPricingFn getDynamicPricingStrategy() const;

    // Uses friend access to modify private room rate & reservation room directly
    void dutyManagerUpgrade(int bookingId, int targetRoomNumber, double managerRateOverride = -1.0);
};

#endif