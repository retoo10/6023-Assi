#include "HotelSystem.h"
#include "HotelExceptions.h"
#include <iostream>
#include <iomanip>

std::string HotelSystem::hotelName = "Muqaddas Tower Hotel";

HotelSystem::HotelSystem() : totalRoomsLoaded(0) {}

std::string HotelSystem::getHotelName() {
    return hotelName;
}

void HotelSystem::addRoom(Room* newRoom) {
    if (newRoom != nullptr) {
        roomInventory.Insert(newRoom);
        totalRoomsLoaded++;
    }
}

Room* HotelSystem::findRoom(int roomNumber) const {
    RoomNode* found = roomInventory.Find(roomNumber);
    return (found != nullptr) ? found->data : nullptr;
}

bool HotelSystem::removeRoomForMaintenance(int roomNumber) {
    // Prevent removing a room that currently has saved bookings
    ReservationLink* current = activeBookings.first;
    while (current != nullptr) {
        if (current->GetData()->getRoom()->getRoomNumber() == roomNumber) {
            throw OverbookingException("Cannot remove Room " + std::to_string(roomNumber) +
                                       " for maintenance: it has active bookings.");
        }
        current = current->next;
    }

    if (roomInventory.Delete(roomNumber)) {
        totalRoomsLoaded--;
        return true;
    }
    return false;
}

void HotelSystem::displayAllRoomsSorted() const {
    std::cout << "--- " << hotelName << " Room Inventory (Sorted via BST In-Order) ---\n";
    roomInventory.DisplayInOrder(roomInventory.root);
    std::cout << "Total Active Rooms in Inventory: " << totalRoomsLoaded << "\n\n";
}

bool HotelSystem::hasDateConflict(int roomNumber, const Date& checkIn, const Date& checkOut) const {
    ReservationLink* current = activeBookings.first;
    while (current != nullptr) {
        Reservation* existing = current->GetData();
        if (existing->getRoom()->getRoomNumber() == roomNumber) {
            if (Date::datesOverlap(checkIn, checkOut,
                                   existing->getCheckInDate(), existing->getCheckOutDate())) {
                return true; // Overlap detected!
            }
        }
        current = current->next;
    }
    return false;
}

Room* HotelSystem::findAvailableInTree(RoomNode* node, const std::string& category,
                                         int partySize, bool needsAccessible,
                                         const Date& checkIn, const Date& checkOut) const {
    if (node == nullptr) {
        return nullptr;
    }

    // Search left subtree first (lower room numbers first)
    Room* leftMatch = findAvailableInTree(node->leftChild, category, partySize,
                                            needsAccessible, checkIn, checkOut);
    if (leftMatch != nullptr) {
        return leftMatch;
    }

    // Check current room
    Room* r = node->data;
    if (r->getRoomCategory() == category &&
        r->getMaxCapacity() >= partySize &&
        (!needsAccessible || r->isAccessible()) &&
        !hasDateConflict(r->getRoomNumber(), checkIn, checkOut)) {
        return r;
    }

    // Search right subtree
    return findAvailableInTree(node->rightChild, category, partySize,
                               needsAccessible, checkIn, checkOut);
}

// Overload 1: Book by specific room number
Reservation* HotelSystem::bookRoom(Guest* guest, int roomNumber, const Date& bookedOn,
                                   const Date& checkIn, const Date& checkOut,
                                   bool requestLounge, const std::string& occasion) {
    Room* targetRoom = findRoom(roomNumber);
    if (targetRoom == nullptr) {
        throw OverbookingException("Room " + std::to_string(roomNumber) + " does not exist.");
    }

    if (hasDateConflict(roomNumber, checkIn, checkOut)) {
        throw OverbookingException("Double-booking blocked: Room " + std::to_string(roomNumber) +
                                   " is already reserved during those dates.");
    }

    Reservation* newRes = new Reservation(guest, targetRoom, bookedOn, checkIn, checkOut,
                                          requestLounge, occasion);
    activeBookings.Insert(new ReservationLink(newRes));
    return newRes;
}

// Overload 2: Book automatically by room category (e.g., "Executive King")
Reservation* HotelSystem::bookRoom(Guest* guest, const std::string& desiredCategory,
                                   const Date& bookedOn, const Date& checkIn, const Date& checkOut,
                                   bool requestLounge, const std::string& occasion) {
    Room* availableRoom = findAvailableInTree(roomInventory.root, desiredCategory,
                                                guest->getPartySize(),
                                                guest->getNeedsAccessibleRoom(),
                                                checkIn, checkOut);
    if (availableRoom == nullptr) {
        throw OverbookingException("No available '" + desiredCategory +
                                   "' room matches requirements for those dates.");
    }

    Reservation* newRes = new Reservation(guest, availableRoom, bookedOn, checkIn, checkOut,
                                          requestLounge, occasion);
    activeBookings.Insert(new ReservationLink(newRes));
    return newRes;
}

Reservation* HotelSystem::findBooking(int bookingId) const {
    ReservationLink* link = activeBookings.Find(bookingId);
    return (link != nullptr) ? link->GetData() : nullptr;
}

bool HotelSystem::cancelBooking(int bookingId) {
    ReservationLink* removed = activeBookings.Delete(bookingId);
    if (removed == nullptr) {
        return false;
    }
    std::cout << "[Cancelled] Booking #" << bookingId << " removed; dates are now free again.\n";
    delete removed->GetData(); // Free Reservation object
    delete removed;            // Free ReservationLink node
    return true;
}

bool HotelSystem::checkOutFirstBooking() {
    if (activeBookings.IsEmpty()) {
        return false;
    }
    ReservationLink* checkedOut = activeBookings.Delete(); // Uses Session 13 Delete()
    if (checkedOut != nullptr) {
        std::cout << "[Checked Out] Booking #" << checkedOut->GetData()->getBookingId()
                  << " (" << checkedOut->GetData()->getGuest()->getName() << ") checked out.\n";
        delete checkedOut->GetData();
        delete checkedOut;
        return true;
    }
    return false;
}

void HotelSystem::displayAllBookings() const {
    std::cout << "--- Saved Reservations (Linked List) ---\n";
    activeBookings.Display();
}

OccupancyPricingFn HotelSystem::getDynamicPricingStrategy() const {
    if (totalRoomsLoaded == 0) {
        return peakOccupancyStandard;
    }
    double occupancyRatio = static_cast<double>(Reservation::getActiveBookingsCount()) / totalRoomsLoaded;
    if (occupancyRatio < 0.50) {
        return lowOccupancyDiscount;  // < 50% full -> 10% off
    } else if (occupancyRatio < 0.80) {
        return midOccupancyDiscount;  // 50%-79% full -> 5% off
    }
    return peakOccupancyStandard;     // 80%+ full -> standard rate
}

void HotelSystem::dutyManagerUpgrade(int bookingId, int targetRoomNumber, double managerRateOverride) {
    Reservation* res = findBooking(bookingId);
    if (res == nullptr) {
        throw UpgradeConflictException("Booking #" + std::to_string(bookingId) + " not found.");
    }

    Room* targetRoom = findRoom(targetRoomNumber);
    if (targetRoom == nullptr) {
        throw UpgradeConflictException("Target Room " + std::to_string(targetRoomNumber) + " not found.");
    }

    // Protect accessible guests from being moved to a non-accessible room
    if (res->guest->getNeedsAccessibleRoom() && !targetRoom->isAccessible()) {
        throw UpgradeConflictException("Upgrade blocked: Guest " + res->guest->getName() +
                                       " requires an Accessible room, but Room " +
                                       std::to_string(targetRoomNumber) + " is not accessible.");
    }

    if (hasDateConflict(targetRoomNumber, res->checkInDate, res->checkOutDate)) {
        throw OverbookingException("Cannot upgrade to Room " + std::to_string(targetRoomNumber) +
                                   ": already booked for those dates.");
    }

    // Concept #3 (Friendship): HotelSystem directly modifies private fields of Reservation & Room
    res->room = targetRoom;
    res->loungeRequested = true; // Complimentary lounge included on upgrade

    if (managerRateOverride >= 0.0) {
        targetRoom->baseRate = managerRateOverride;
    }

    std::cout << "[Duty Manager Override] Booking #" << bookingId << " upgraded to "
              << *targetRoom << "!\n";
}