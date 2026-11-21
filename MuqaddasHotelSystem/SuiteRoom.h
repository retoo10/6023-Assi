#ifndef SUITE_ROOM_H
#define SUITE_ROOM_H

#include "Room.h"

class SuiteRoom : public Room {
private:
    std::string suiteType; // e.g., "Junior Suite" or "Suite"
    bool separateLivingArea;

public:
    SuiteRoom();
    SuiteRoom(int num, const std::string& type, const std::string& bed,
              const std::string& zone, int capacity, double rate,
              bool livingArea = true, bool isAccessible = false, int connectingNum = 0);

    double calculateNightlyRate() const override;
    std::string getRoomCategory() const override;
    bool hasLoungeAccess() const override;
    void displayDetails() const override;
};

#endif