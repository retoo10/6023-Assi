#include "ExecutiveRoom.h"

ExecutiveRoom::ExecutiveRoom()
    : Room(0, "King", "Mid-Corridor (Quiet)", 2, 165.0), highFloorView(true) {}

ExecutiveRoom::ExecutiveRoom(int num, const std::string& bed, const std::string& zone,
                             int capacity, double rate, bool highFloor,
                             bool isAccessible, int connectingNum)
    : Room(num, bed, zone, capacity, rate, isAccessible, connectingNum),
      highFloorView(highFloor) {}

double ExecutiveRoom::calculateNightlyRate() const {
    return baseRate; // Lounge access is included free in Executive rooms
}

std::string ExecutiveRoom::getRoomCategory() const {
    return "Executive " + bedType;
}

bool ExecutiveRoom::hasLoungeAccess() const {
    return true; // All Executive rooms come with complimentary Executive Lounge access
}

void ExecutiveRoom::displayDetails() const {
    Room::displayDetails();
    std::cout << " | High Floor: " << (highFloorView ? "Yes" : "No") << "\n";
}