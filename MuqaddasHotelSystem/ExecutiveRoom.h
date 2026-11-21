#ifndef EXECUTIVE_ROOM_H
#define EXECUTIVE_ROOM_H

#include "Room.h"

class ExecutiveRoom : public Room {
private:
    bool highFloorView;

public:
    ExecutiveRoom();
    ExecutiveRoom(int num, const std::string& bed, const std::string& zone,
                  int capacity, double rate, bool highFloor = true,
                  bool isAccessible = false, int connectingNum = 0);

    double calculateNightlyRate() const override;
    std::string getRoomCategory() const override;
    bool hasLoungeAccess() const override;
    void displayDetails() const override;
};

#endif