#include "ReservationList.h"

// Link constructor assigns Reservation* data and sets next pointer to null
ReservationLink::ReservationLink(Reservation* resData) {
    this->data = resData; // Store the Reservation pointer inside this link
    this->next = nullptr; // Initialise next link pointer to null
}

// Displays the Reservation object held by this link
void ReservationLink::Display() const {
    if (this->data != nullptr) {                  // Verify Reservation pointer is valid
        std::cout << *(this->data) << "\n\n";     // Print using overloaded Reservation::operator<<
    }
}

// Getter returning the Reservation pointer stored in this link
Reservation* ReservationLink::GetData() const {
    return this->data; // Return stored Reservation*
}

// LinkedList constructor starts with an empty list
ReservationList::ReservationList() {
    first = nullptr; // Set head pointer to null
}

// Destructor iterates through the list and deletes every link and Reservation
ReservationList::~ReservationList() {
    ReservationLink* current = first;         // Start at the head of the list
    while (current != nullptr) {              // Continue until end of list is reached
        ReservationLink* nextNode = current->next; // Save pointer to next link before deleting current
        delete current->data;                 // Free the Reservation object
        delete current;                       // Free the ReservationLink wrapper
        current = nextNode;                   // Advance to the next link
    }
    first = nullptr;                          // Reset head pointer to null
}

// Checks whether the linked list contains any links
bool ReservationList::IsEmpty() const {
    return (first == nullptr); // Returns true when head pointer is null
}

// Inserts a new link at the beginning of the list in O(1) constant time (Session 13)
void ReservationList::Insert(ReservationLink* newLink) {
    newLink->next = first; // Point new link's next to the current first link
    first = newLink;       // Update first to point to the newly inserted link
}

// Traverses from first to end, displaying every reservation (Session 13)
void ReservationList::Display() const {
    if (IsEmpty()) {                          // Check if list has no bookings
        std::cout << "No active reservations in the list.\n"; // Inform user if list is empty
        return;                               // Exit early
    }
    ReservationLink* current = first;         // Start traversal at the first link
    while (current != nullptr) {              // Loop until null reference at end of list
        current->Display();                   // Print current reservation
        current = current->next;              // Move pointer to the next link
    }
}

// Linearly searches the list for a reservation matching bookingId (Session 13 Exercise 3)
ReservationLink* ReservationList::Find(int bookingId) const {
    if (IsEmpty()) {                          // Guard against searching an empty list
        return nullptr;                       // Return null when list is empty
    }
    ReservationLink* current = first;         // Start at the first link
    while (current->data->getBookingId() != bookingId) { // Loop while ID does not match
        if (current->next == nullptr) {       // Reached end of list without a match
            return nullptr;                   // Return null indicating booking not found
        } else {
            current = current->next;          // Advance to next link
        }
    }
    return current;                           // Return matching ReservationLink*
}

// Removes and returns the first link from the list (Session 13 Slide 12)
ReservationLink* ReservationList::Delete() {
    if (IsEmpty()) {                          // Guard against deleting from an empty list
        return nullptr;                       // Nothing to remove
    }
    ReservationLink* temp = first;            // Save pointer to current first link
    first = first->next;                      // Move head pointer forward to second link
    temp->next = nullptr;                     // Detach removed link from the list
    return temp;                              // Return removed link to caller
}

// Finds and removes a specific reservation by bookingId (Session 13 Exercise 4)
ReservationLink* ReservationList::Delete(int bookingId) {
    if (IsEmpty()) {                          // Guard against deleting from an empty list
        return nullptr;                       // Return null when list is empty
    }

    ReservationLink* current = first;         // Pointer used to find the target link
    ReservationLink* previous = first;        // Pointer trailing one step behind current

    while (current->data->getBookingId() != bookingId) { // Search until bookingId matches
        if (current->next == nullptr) {       // Reached end without finding bookingId
            return nullptr;                   // Return null indicating not found
        } else {
            previous = current;               // Advance previous to current
            current = current->next;          // Advance current to next link
        }
    }

    if (current == first) {                   // If target is the very first link
        first = first->next;                  // Update head to point to the second link
    } else {
        previous->next = current->next;       // Bypass current by linking previous directly to next
    }
    current->next = nullptr;                  // Detach removed link from remaining list
    return current;                           // Return removed link to caller
}