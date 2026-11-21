#include "Guest.h"

Guest::Guest()
    : guestId(0), name("Walk-in Guest"), loyaltyTier(LoyaltyTier::Member),
      partySize(1), needsAccessibleRoom(false),
      specialRequests(new std::string[2]), requestCount(0), requestCapacity(2) {}

Guest::Guest(int id, const std::string& guestName, LoyaltyTier tier,
             int guestsInParty, bool accessibleNeeded)
    : guestId(id), name(guestName), loyaltyTier(tier),
      partySize(guestsInParty), needsAccessibleRoom(accessibleNeeded),
      specialRequests(new std::string[2]), requestCount(0), requestCapacity(2) {}

// Deep Copy Constructor: allocates a separate copy of specialRequests on the heap
Guest::Guest(const Guest& other)
    : guestId(other.guestId), name(other.name), loyaltyTier(other.loyaltyTier),
      partySize(other.partySize), needsAccessibleRoom(other.needsAccessibleRoom),
      specialRequests(new std::string[other.requestCapacity]),
      requestCount(other.requestCount), requestCapacity(other.requestCapacity) {
    for (int i = 0; i < requestCount; ++i) {
        specialRequests[i] = other.specialRequests[i];
    }
}

// Deep Copy Assignment Operator
Guest& Guest::operator=(const Guest& other) {
    if (this == &other) {
        return *this; // Guard against self-assignment
    }

    delete[] specialRequests; // Free old memory before allocating new memory

    guestId = other.guestId;
    name = other.name;
    loyaltyTier = other.loyaltyTier;
    partySize = other.partySize;
    needsAccessibleRoom = other.needsAccessibleRoom;
    requestCount = other.requestCount;
    requestCapacity = other.requestCapacity;

    specialRequests = new std::string[requestCapacity];
    for (int i = 0; i < requestCount; ++i) {
        specialRequests[i] = other.specialRequests[i];
    }
    return *this;
}

Guest::~Guest() {
    delete[] specialRequests; // Always release dynamic array memory!
}

void Guest::resizeRequestsArray() {
    int newCapacity = requestCapacity * 2;
    std::string* biggerArray = new std::string[newCapacity];

    for (int i = 0; i < requestCount; ++i) {
        biggerArray[i] = specialRequests[i];
    }

    delete[] specialRequests; // Free the old smaller array
    specialRequests = biggerArray;
    requestCapacity = newCapacity;
}

void Guest::addSpecialRequest(const std::string& request) {
    if (requestCount == requestCapacity) {
        resizeRequestsArray();
    }
    specialRequests[requestCount++] = request;
}

void Guest::displayRequests() const {
    if (requestCount == 0) {
        std::cout << "None";
        return;
    }
    for (int i = 0; i < requestCount; ++i) {
        std::cout << specialRequests[i];
        if (i < requestCount - 1) std::cout << ", ";
    }
}

int Guest::getGuestId() const { return guestId; }
std::string Guest::getName() const { return name; }
LoyaltyTier Guest::getLoyaltyTier() const { return loyaltyTier; }

std::string Guest::getLoyaltyTierString() const {
    switch (loyaltyTier) {
        case LoyaltyTier::Member:  return "Member";
        case LoyaltyTier::Silver:  return "Silver";
        case LoyaltyTier::Gold:    return "Gold";
        case LoyaltyTier::Diamond: return "Diamond";
        default:                   return "Member";
    }
}

int Guest::getPartySize() const { return partySize; }
bool Guest::getNeedsAccessibleRoom() const { return needsAccessibleRoom; }

bool Guest::operator==(const Guest& other) const {
    return this->guestId == other.guestId;
}

std::ostream& operator<<(std::ostream& os, const Guest& g) {
    os << g.name << " [ID: " << g.guestId << ", Tier: " << g.getLoyaltyTierString()
       << ", Party: " << g.partySize << "]";
    return os;
}