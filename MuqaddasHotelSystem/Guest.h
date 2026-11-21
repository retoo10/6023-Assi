#ifndef GUEST_H
#define GUEST_H

#include <iostream>
#include <string>

// Type-safe Loyalty Program tiers matching Hilton/Muqaddas Tower tiers
enum class LoyaltyTier {
    Member,
    Silver,
    Gold,
    Diamond
};

class Guest {
private:
    int guestId;
    std::string name;
    LoyaltyTier loyaltyTier;
    int partySize;
    bool needsAccessibleRoom;

    // Concept #8: Manual Memory Management inside a class (dynamic array on the heap)
    std::string* specialRequests;
    int requestCount;
    int requestCapacity;

    void resizeRequestsArray(); // Private helper to expand dynamic array when full

public:
    // Constructors, Copy Constructor, Copy Assignment, and Destructor (Rule of Three)
    Guest();
    Guest(int id, const std::string& guestName, LoyaltyTier tier,
          int guestsInParty, bool accessibleNeeded = false);
    Guest(const Guest& other);            // Deep copy constructor
    Guest& operator=(const Guest& other); // Deep copy assignment operator
    ~Guest();                             // Destructor frees heap memory with delete[]

    // Manage special requests
    void addSpecialRequest(const std::string& request);
    void displayRequests() const;

    // Getters
    int getGuestId() const;
    std::string getName() const;
    LoyaltyTier getLoyaltyTier() const;
    std::string getLoyaltyTierString() const;
    int getPartySize() const;
    bool getNeedsAccessibleRoom() const;

    // Operator Overloading
    bool operator==(const Guest& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Guest& g);
};

#endif