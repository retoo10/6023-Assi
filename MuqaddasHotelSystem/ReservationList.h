#ifndef RESERVATION_LIST_H
#define RESERVATION_LIST_H

#include "Reservation.h"

// Adapted from Session 13 Link class to store a Reservation* object
class ReservationLink {
public:
    Reservation* data;     // Pointer to the Reservation stored in this link
    ReservationLink* next; // Pointer to the next link in the singly linked list

    explicit ReservationLink(Reservation* resData); // Constructor initialises link with Reservation*
    void Display() const;                           // Displays the reservation stored in this link
    Reservation* GetData() const;                   // Returns the stored Reservation*
};

// Adapted from Session 13 LinkedList class to manage hotel reservations dynamically
class ReservationList {
public:
    ReservationLink* first; // Pointer to the first link (head) of the list

    ReservationList();                                // Constructor sets first to nullptr
    ~ReservationList();                               // Destructor frees all links and reservations
    bool IsEmpty() const;                             // Returns true if list has no links
    void Insert(ReservationLink* newLink);            // Inserts a new link at the front of the list
    void Display() const;                             // Traverses and displays all links in the list
    ReservationLink* Find(int bookingId) const;       // Searches sequentially for a matching bookingId
    ReservationLink* Delete();                        // Removes and returns the first link (Session 13 Slide 12)
    ReservationLink* Delete(int bookingId);           // Removes and returns a link matching bookingId
};

#endif