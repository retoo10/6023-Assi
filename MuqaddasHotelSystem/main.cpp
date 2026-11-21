#include <iostream>
#include <iomanip>
#include "HotelExceptions.h"
#include "Date.h"
#include "HotelSystem.h"

int main() {
    std::cout << "=================================================================\n";
    std::cout << "      " << HotelSystem::getHotelName() << " - Front Desk & Operations\n";
    std::cout << "=================================================================\n\n";

    HotelSystem hotel;

    // 1. Load 12 Rooms into the Binary Search Tree (inserted in non-sorted order to keep BST balanced!)
    hotel.addRoom(new ExecutiveRoom(202, "Twin", "Far End (Quiet)", 2, 165.0, true));
    hotel.addRoom(new StandardRoom(103, "King", "Near Lift", 2, 115.0, true, 0));               // Accessible
    hotel.addRoom(new SuiteRoom(302, "Junior Suite", "King", "Mid-Corridor (Quiet)", 3, 235.0, false, false, 303));
    hotel.addRoom(new StandardRoom(101, "King", "Mid-Corridor (Quiet)", 2, 115.0));
    hotel.addRoom(new StandardRoom(105, "Twin", "Mid-Corridor (Quiet)", 2, 120.0, false, 104)); // Connects to 104
    hotel.addRoom(new ExecutiveRoom(201, "King", "Mid-Corridor (Quiet)", 2, 165.0, true));
    hotel.addRoom(new SuiteRoom(501, "Suite", "Super King", "Top Floor", 4, 340.0, true));
    hotel.addRoom(new StandardRoom(102, "Twin", "Near Lift", 2, 115.0));
    hotel.addRoom(new StandardRoom(104, "King", "Mid-Corridor (Quiet)", 2, 120.0, false, 105)); // Connects to 105
    hotel.addRoom(new ExecutiveRoom(203, "King", "Near Lift", 2, 165.0, true, true, 0));        // Accessible Exec
    hotel.addRoom(new SuiteRoom(301, "Junior Suite", "King", "Mid-Corridor (Quiet)", 3, 230.0, false));
    hotel.addRoom(new StandardRoom(303, "Twin", "Mid-Corridor (Quiet)", 2, 120.0, false, 302)); // Connects to 302

    // Display all 12 rooms sorted by room number (BST DisplayInOrder)
    hotel.displayAllRoomsSorted();

    // 2. Create Guests across all 4 Loyalty Tiers (Member, Silver, Gold, Diamond)
    Guest* guest1 = new Guest(1, "Aisha Khan", LoyaltyTier::Gold, 2, false);
    guest1->addSpecialRequest("Mid-corridor quiet room");
    guest1->addSpecialRequest("Feather-free pillows");
    guest1->addSpecialRequest("Late check-out"); // Triggers dynamic array resize

    Guest* guest2 = new Guest(2, "James Miller", LoyaltyTier::Diamond, 1, false);
    Guest* guest3 = new Guest(3, "Sarah Jenkins", LoyaltyTier::Silver, 2, true); // Needs accessible room
    Guest* guest4 = new Guest(4, "Tariq Ali", LoyaltyTier::Member, 3, false);

    // Demonstrate Copy Constructor (Concept #5 & #8)
    Guest guest1Copy(*guest1);
    std::cout << "[Copy Constructor Check] Copied guest: " << guest1Copy << " | Requests: ";
    guest1Copy.displayRequests();
    std::cout << "\n\n";

    // 3. Save Bookings for Specific Check-In and Check-Out Dates
    Date bookedToday(19, 11, 2026);
    std::cout << "--- Creating Reservations (Saved by Date) ---\n";

    try {
        // Overload 1: Book specific Room 101 (Standard King) for Gold guest + Paid Lounge
        Reservation* r1 = hotel.bookRoom(guest1, 101, bookedToday,
                                         Date(25, 11, 2026), Date(28, 11, 2026),
                                         true, "Anniversary");

        // Overload 1: Book Room 101 again on NON-OVERLAPPING dates (28/11 -> 30/11) -> Allowed!
        Reservation* r2 = hotel.bookRoom(guest2, 101, bookedToday,
                                         Date(28, 11, 2026), Date(30, 11, 2026),
                                         true, "Business"); // Diamond gets FREE lounge!

        // Overload 2: Book by category ("Executive King") -> auto-picks accessible Room 203!
        Reservation* r3 = hotel.bookRoom(guest3, "Executive King", bookedToday,
                                         Date(26, 11, 2026), Date(29, 11, 2026),
                                         true, "Leisure");

        // Overload 1: Book Suite 501 for party of 3
        Reservation* r4 = hotel.bookRoom(guest4, 501, bookedToday,
                                         Date(27, 11, 2026), Date(30, 11, 2026),
                                         false, "Birthday");

        // Calculate bills using Function Pointer chosen dynamically by hotel occupancy
        OccupancyPricingFn activePricing = hotel.getDynamicPricingStrategy();
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Booking #" << r1->getBookingId() << " (Gold, Paid Lounge) Bill: £"
                  << r1->calculateTotalBill(activePricing) << "\n";
        std::cout << "Booking #" << r2->getBookingId() << " (Diamond, Free Lounge) Bill: £"
                  << r2->calculateTotalBill(activePricing) << "\n";
        std::cout << "Booking #" << r3->getBookingId() << " (Exec Accessible) Bill: £"
                  << r3->calculateTotalBill(activePricing) << "\n";
        std::cout << "Booking #" << r4->getBookingId() << " (Suite 501) Bill: £"
                  << r4->calculateTotalBill(activePricing) << "\n\n";

    } catch (const std::exception& ex) {
        std::cout << "Booking Error: " << ex.what() << "\n";
    }

    // 4. Test Exception Handling (Double-Booking, Invalid Date, Accessible Upgrade Conflict)
    std::cout << "--- Testing Error & Conflict Protection ---\n";
    try {
        // Try double-booking Room 101 on overlapping dates (26/11 -> 29/11 overlaps 25/11 -> 28/11!)
        hotel.bookRoom(guest4, 101, bookedToday, Date(26, 11, 2026), Date(29, 11, 2026));
    } catch (const OverbookingException& ex) {
        std::cout << "Caught OverbookingException: " << ex.what() << "\n";
    }

    try {
        // Try invalid date
        Date bad(31, 2, 2026);
    } catch (const InvalidDateException& ex) {
        std::cout << "Caught InvalidDateException: " << ex.what() << "\n";
    }

    try {
        // Try upgrading Accessible guest3 (Booking #1003) into non-accessible Room 301
        hotel.dutyManagerUpgrade(1003, 301);
    } catch (const UpgradeConflictException& ex) {
        std::cout << "Caught UpgradeConflictException: " << ex.what() << "\n";
    }

    // 5. Valid Duty Manager Override (Friend class modifying Booking #1001 to Junior Suite 301 @ £210)
    std::cout << "\n--- Duty Manager VIP Upgrade & Room Lookup ---\n";
    try {
        hotel.dutyManagerUpgrade(1001, 301, 210.0);
        Room* lookup = hotel.findRoom(301); // Uses RoomTree::Find
        if (lookup != nullptr) {
            std::cout << "BST Find(301) updated details -> ";
            lookup->displayDetails();
        }
    } catch (const std::exception& ex) {
        std::cout << "Override Error: " << ex.what() << "\n";
    }

    // 6. Display all saved bookings (Linked List Display), then test Find, Cancel, & Check-Out
    std::cout << "\n";
    hotel.displayAllBookings();

    std::cout << "--- Linked List Find / Cancel / Check-Out Operations ---\n";
    Reservation* foundBooking = hotel.findBooking(1002);
    if (foundBooking != nullptr) {
        std::cout << "Found Booking #1002 for guest: " << foundBooking->getGuest()->getName() << "\n";
    }

    // Cancel Booking #1004 (Suite 501) using LinkedList Delete(id)
    hotel.cancelBooking(1004);

    // Check out first booking in LinkedList using Session 13 Delete()
    hotel.checkOutFirstBooking();

    // 7. Remove unbooked Room 102 for maintenance using BST Delete(roomNumber)
    std::cout << "\n--- BST Room Deletion (Maintenance) ---\n";
    if (hotel.removeRoomForMaintenance(102)) {
        std::cout << "Room 102 removed from BST for refurbishment.\n";
    }
    hotel.displayAllRoomsSorted();

    // Clean up Guest objects
    delete guest1;
    delete guest2;
    delete guest3;
    delete guest4;

    return 0;
}