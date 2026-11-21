#ifndef STANDARD_ROOM_H
#define STANDARD_ROOM_H

#include "Room.h"

class StandardRoom : public Room {
public:
    StandardRoom();
    StandardRoom(int num, const std::string& bed, const std::string& zone,
                 int capacity, double rate, bool isAccessible = false, int connectingNum = 0);

    double calculateNightlyRate() const override;
    std::string getRoomCategory() const override;
    bool hasLoungeAccess() const override;
    void displayDetails() const override;
};

#endif