#ifndef ROOM_TREE_H
#define ROOM_TREE_H

#include "Room.h"

// Adapted from Session 15 Node class to store a Room* object instead of an int
class RoomNode {
public:
    RoomNode* leftChild;  // Pointer to left child node (smaller room numbers)
    RoomNode* rightChild; // Pointer to right child node (larger room numbers)
    Room* data;           // Pointer to the Room object stored in this tree node

    explicit RoomNode(Room* roomData); // Constructor to initialise node with a Room*
    void Display() const;              // Displays the Room stored in this node
};

// Adapted from Session 15 Tree class to manage RoomNode objects in a Binary Search Tree
class RoomTree {
private:
    RoomNode* getInOrderSuccessor(RoomNode* delNode); // Finds replacement node when deleting a node with 2 children
    void destroyTree(RoomNode* localRoot);            // Recursively frees all nodes and rooms on destruction

public:
    RoomNode* root; // Pointer to the root (top) node of the Binary Search Tree

    RoomTree();                               // Default constructor creates an empty tree
    ~RoomTree();                              // Destructor cleans up heap memory
    void Insert(Room* newRoom);               // Inserts a new Room* into the BST by roomNumber
    RoomNode* Find(int roomNumber) const;     // Searches the BST for a matching roomNumber
    bool Delete(int roomNumber);              // Deletes a room by roomNumber (handles 0, 1, or 2 children)
    void DisplayInOrder(RoomNode* localRoot) const; // Traverses Left -> Root -> Right (sorted room numbers)
};

#endif