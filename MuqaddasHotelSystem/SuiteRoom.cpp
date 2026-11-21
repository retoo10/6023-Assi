#include "SuiteRoom.h"

SuiteRoom::SuiteRoom()
    : Room(0, "King", "Mid-Corridor (Quiet)", 4, 240.0),
      suiteType("Junior Suite"), separateLivingArea(true) {}

SuiteRoom::SuiteRoom(int num, const std::string& type, const std::string& bed,
                     const std::string& zone, int capacity, double rate,
                     bool livingArea, bool isAccessible, int connectingNum)
    : Room(num, bed, zone, capacity, rate, isAccessible, connectingNum),
      suiteType(type), separateLivingArea(livingArea) {}

double SuiteRoom::calculateNightlyRate() const {
    return baseRate; // Includes complimentary Executive Lounge + suite space
}

std::string SuiteRoom::getRoomCategory() const {
    return suiteType + " (" + bedType + ")";
}

bool SuiteRoom::hasLoungeAccess() const {
    return true; // Suites always include Executive Lounge access
}

void SuiteRoom::displayDetails() const {
    Room::displayDetails();
    std::cout << " | Living Area: " << (separateLivingArea ? "Separate" : "Open-Plan") << "\n";
}