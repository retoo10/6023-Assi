#include "Room.h"
#include <iomanip>

int Room::totalRooms = 0;

Room::Room()
    : roomNumber(0), bedType("King"), corridorZone("Mid-Corridor (Quiet)"),
      maxCapacity(2), baseRate(110.0), accessible(false), connectingRoomNumber(0) {
    totalRooms++;
}

Room::Room(int num, const std::string& bed, const std::string& zone,
           int capacity, double rate, bool isAccessible, int connectingNum)
    : roomNumber(num), bedType(bed), corridorZone(zone),
      maxCapacity(capacity), baseRate(rate), accessible(isAccessible),
      connectingRoomNumber(connectingNum) {
    totalRooms++;
}

Room::~Room() {
    totalRooms--;
}

void Room::displayDetails() const {
    std::cout << "Room " << roomNumber << " [" << getRoomCategory() << "]"
              << " | Zone: " << corridorZone
              << " | Max: " << maxCapacity
              << " | Rate: £" << std::fixed << std::setprecision(2) << calculateNightlyRate()
              << " | Lounge: " << (hasLoungeAccess() ? "Free" : "Paid/Diamond Only");

    if (accessible) {
        std::cout << " | [Accessible]";
    }
    if (connectingRoomNumber != 0) {
        std::cout << " | [Connects to Rm " << connectingRoomNumber << "]";
    }
}

bool Room::isLoungeEligible() const {
    return hasLoungeAccess();
}

bool Room::isLoungeEligible(const std::string& loyaltyTier) const {
    // Hilton rule: Executive/Suite gets free lounge, OR Diamond members in ANY room!
    return hasLoungeAccess() || (loyaltyTier == "Diamond");
}

int Room::getRoomNumber() const { return roomNumber; }
std::string Room::getBedType() const { return bedType; }
std::string Room::getCorridorZone() const { return corridorZone; }
int Room::getMaxCapacity() const { return maxCapacity; }
double Room::getBaseRate() const { return baseRate; }
bool Room::isAccessible() const { return accessible; }
int Room::getConnectingRoomNumber() const { return connectingRoomNumber; }

int Room::getTotalRooms() { return totalRooms; }

bool Room::operator==(const Room& other) const {
    return this->roomNumber == other.roomNumber;
}

bool Room::operator<(const Room& other) const {
    return this->roomNumber < other.roomNumber;
}

std::ostream& operator<<(std::ostream& os, const Room& r) {
    os << "Room " << r.roomNumber << " (" << r.getRoomCategory() << ")";
    return os;
}