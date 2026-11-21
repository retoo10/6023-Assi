#include "Reservation.h"
#include "HotelExceptions.h"
#include <iomanip>

int Reservation::nextBookingId = 1001;
int Reservation::activeBookingsCount = 0;

// Occupancy pricing functions (pointed to by OccupancyPricingFn)
double lowOccupancyDiscount(double roomSubtotal) {
    return roomSubtotal * 0.90; // 10% discount
}

double midOccupancyDiscount(double roomSubtotal) {
    return roomSubtotal * 0.95; // 5% discount
}

double peakOccupancyStandard(double roomSubtotal) {
    return roomSubtotal;        // 100% standard rate
}

Reservation::Reservation(Guest* g, Room* r, const Date& bookedOn,
                         const Date& checkIn, const Date& checkOut,
                         bool requestLounge, const std::string& specialOccasion)
    : bookingId(nextBookingId++), guest(g), room(r),
      bookingDate(bookedOn), checkInDate(checkIn), checkOutDate(checkOut),
      loungeRequested(requestLounge), occasion(specialOccasion) {

    if (guest == nullptr || room == nullptr) {
        throw OverbookingException("Cannot create reservation without a valid Guest and Room.");
    }
    if (!(checkInDate < checkOutDate)) {
        throw InvalidDateException("Check-out date must be after check-in date.");
    }
    if (guest->getPartySize() > room->getMaxCapacity()) {
        throw OverbookingException("Party size (" + std::to_string(guest->getPartySize()) +
                                   ") exceeds room capacity (" + std::to_string(room->getMaxCapacity()) + ").");
    }
    if (guest->getNeedsAccessibleRoom() && !room->isAccessible()) {
        throw OverbookingException("Guest requires an accessible room, but Room " +
                                   std::to_string(room->getRoomNumber()) + " is not accessible.");
    }

    activeBookingsCount++;
}

Reservation::~Reservation() {
    activeBookingsCount--;
}

int Reservation::getNights() const {
    return checkOutDate - checkInDate;
}

double Reservation::calculateLoungeFee() const {
    if (!loungeRequested) {
        return 0.0;
    }
    // Free if room includes lounge (Executive/Suite) OR if guest is Diamond
    if (room->isLoungeEligible(guest->getLoyaltyTierString())) {
        return 0.0;
    }
    // Otherwise Member / Silver / Gold pays £35 per night for lounge access
    return getNights() * LOUNGE_SUPPLEMENT_PER_NIGHT;
}

double Reservation::calculateTotalBill() const {
    // Default overload uses standard peak rate
    return calculateTotalBill(peakOccupancyStandard);
}

double Reservation::calculateTotalBill(OccupancyPricingFn pricingRule) const {
    double roomSubtotal = getNights() * room->calculateNightlyRate();
    double discountedRoomTotal = pricingRule(roomSubtotal); // Calls the function pointer!
    return discountedRoomTotal + calculateLoungeFee();
}

int Reservation::getBookingId() const { return bookingId; }
Guest* Reservation::getGuest() const { return guest; }
Room* Reservation::getRoom() const { return room; }
Date Reservation::getBookingDate() const { return bookingDate; }
Date Reservation::getCheckInDate() const { return checkInDate; }
Date Reservation::getCheckOutDate() const { return checkOutDate; }
bool Reservation::isLoungeRequested() const { return loungeRequested; }
std::string Reservation::getOccasion() const { return occasion; }
int Reservation::getActiveBookingsCount() { return activeBookingsCount; }

bool Reservation::operator==(const Reservation& other) const {
    return this->bookingId == other.bookingId;
}

bool Reservation::operator<(const Reservation& other) const {
    return this->bookingId < other.bookingId;
}

std::ostream& operator<<(std::ostream& os, const Reservation& res) {
    os << "Booking #" << res.bookingId
       << " | Guest: " << *res.guest
       << "\n   Room: " << *res.room
       << "\n   Booked On: " << res.bookingDate
       << " | Stay: " << res.checkInDate << " -> " << res.checkOutDate
       << " (" << res.getNights() << " nights)"
       << "\n   Occasion: " << res.occasion
       << " | Lounge Requested: " << (res.loungeRequested ? "Yes" : "No")
       << " (Lounge Fee: £" << std::fixed << std::setprecision(2) << res.calculateLoungeFee() << ")";
    return os;
}