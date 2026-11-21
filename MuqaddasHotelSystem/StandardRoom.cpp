#include "StandardRoom.h"

StandardRoom::StandardRoom() : Room() {}

StandardRoom::StandardRoom(int num, const std::string& bed, const std::string& zone,
                           int capacity, double rate, bool isAccessible, int connectingNum)
    : Room(num, bed, zone, capacity, rate, isAccessible, connectingNum) {}

double StandardRoom::calculateNightlyRate() const {
    return baseRate;
}

std::string StandardRoom::getRoomCategory() const {
    return "Standard " + bedType;
}

bool StandardRoom::hasLoungeAccess() const {
    return false; // Standard rooms do not include free lounge unless guest is Diamond
}

void StandardRoom::displayDetails() const {
    Room::displayDetails();
    std::cout << "\n";
}