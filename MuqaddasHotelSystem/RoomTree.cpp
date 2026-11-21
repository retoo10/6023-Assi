#include "RoomTree.h"

// Constructor sets the stored Room* pointer and initialises both child pointers to null
RoomNode::RoomNode(Room* roomData) {
    this->data = roomData;     // Store the pointer to the Room object
    leftChild = nullptr;       // Initialise left child pointer as empty
    rightChild = nullptr;      // Initialise right child pointer as empty
}

// Calls the Room's polymorphic displayDetails() method
void RoomNode::Display() const {
    if (this->data != nullptr) {       // Ensure the Room pointer is valid before dereferencing
        this->data->displayDetails();  // Invoke polymorphic display for Standard/Executive/Suite
    }
}

// Tree constructor starts with an empty tree (root is null)
RoomTree::RoomTree() {
    root = nullptr; // Set root pointer to null so tree starts empty
}

// Tree destructor releases every node in the tree
RoomTree::~RoomTree() {
    destroyTree(root); // Begin recursive post-order cleanup from the root
}

// Helper method to delete nodes safely from bottom up (post-order traversal)
void RoomTree::destroyTree(RoomNode* localRoot) {
    if (localRoot != nullptr) {               // Base case: stop when branch is null
        destroyTree(localRoot->leftChild);    // Recursively delete left subtree
        destroyTree(localRoot->rightChild);   // Recursively delete right subtree
        delete localRoot->data;               // Free the dynamically allocated Room object
        delete localRoot;                     // Free the RoomNode wrapper itself
    }
}

// Inserts a Room* into the BST ordered by roomNumber (adapted from Session 15 Exercise 2)
void RoomTree::Insert(Room* newRoom) {
    RoomNode* newNode = new RoomNode(newRoom); // Create a new node wrapping the Room*

    if (root == nullptr) {                     // Check if the tree is currently empty
        root = newNode;                        // Make the new node the root of the tree
    } else {
        RoomNode* current = root;              // Start traversal at the root
        RoomNode* parent = nullptr;            // Track the parent node one step behind current

        while (true) {                         // Loop until an empty child slot is found
            parent = current;                  // Move parent down to current before advancing

            if (*newRoom < *(current->data)) { // Compare room numbers using overloaded Room::operator<
                current = current->leftChild;  // Move left for smaller room numbers
                if (current == nullptr) {      // Found an empty left child slot
                    parent->leftChild = newNode; // Attach the new node as the left child
                    return;                    // Exit insertion once placed
                }
            } else {
                current = current->rightChild; // Move right for larger or equal room numbers
                if (current == nullptr) {      // Found an empty right child slot
                    parent->rightChild = newNode; // Attach the new node as the right child
                    return;                    // Exit insertion once placed
                }
            }
        }
    }
}

// Searches the BST for a room by roomNumber (adapted from Session 15 Find)
RoomNode* RoomTree::Find(int roomNumber) const {
    RoomNode* current = root; // Start searching from the root node

    while (current != nullptr && current->data->getRoomNumber() != roomNumber) { // Loop while nodes remain and key doesn't match
        if (roomNumber < current->data->getRoomNumber()) { // Target room number is smaller
            current = current->leftChild;                  // Search down the left subtree
        } else {
            current = current->rightChild;                 // Search down the right subtree
        }
    }
    return current; // Returns matching RoomNode*, or nullptr if not found
}

// Helper for Case 3 of Delete: finds the smallest node in the right subtree (In-Order Successor)
RoomNode* RoomTree::getInOrderSuccessor(RoomNode* delNode) {
    RoomNode* successorParent = delNode;       // Tracks parent of the successor
    RoomNode* successor = delNode;             // Will hold the replacement node
    RoomNode* current = delNode->rightChild;   // Step once into the right subtree

    while (current != nullptr) {               // Keep moving as far left as possible
        successorParent = successor;           // Advance successorParent
        successor = current;                   // Advance successor
        current = current->leftChild;          // Move to left child to find smallest value
    }

    if (successor != delNode->rightChild) {    // If successor is not the immediate right child
        successorParent->leftChild = successor->rightChild; // Re-link successor's right subtree to its parent
        successor->rightChild = delNode->rightChild;        // Attach deleted node's right child to successor
    }
    return successor;                          // Return the successor node to replace delNode
}

// Deletes a room by roomNumber covering all 3 BST cases (Session 15 Slides 25-28)
bool RoomTree::Delete(int roomNumber) {
    if (root == nullptr) {                     // Guard against deleting from an empty tree
        return false;                          // Return false since room cannot exist
    }

    RoomNode* current = root;                  // Pointer to find the node to delete
    RoomNode* parent = root;                   // Pointer to track the parent of current
    bool isLeftChild = true;                   // Tracks whether current is parent's left or right child

    while (current->data->getRoomNumber() != roomNumber) { // Locate target node
        parent = current;                      // Move parent down before stepping
        if (roomNumber < current->data->getRoomNumber()) { // Target is in left subtree
            isLeftChild = true;                // Mark direction as left
            current = current->leftChild;      // Step left
        } else {
            isLeftChild = false;               // Mark direction as right
            current = current->rightChild;     // Step right
        }
        if (current == nullptr) {              // Reached bottom without finding roomNumber
            return false;                      // Room not found, return false
        }
    }

    // Case 1: Node has NO children (Leaf node)
    if (current->leftChild == nullptr && current->rightChild == nullptr) {
        if (current == root) {                 // If leaf is also the root
            root = nullptr;                    // Tree becomes empty
        } else if (isLeftChild) {              // If node was parent's left child
            parent->leftChild = nullptr;       // Detach parent's left pointer
        } else {                               // If node was parent's right child
            parent->rightChild = nullptr;      // Detach parent's right pointer
        }
    }
    // Case 2a: Node has ONLY a left child (no right child)
    else if (current->rightChild == nullptr) {
        if (current == root) {                 // If deleting root
            root = current->leftChild;         // Promote left child to root
        } else if (isLeftChild) {              // If node was parent's left child
            parent->leftChild = current->leftChild;  // Bypass current via left child
        } else {                               // If node was parent's right child
            parent->rightChild = current->leftChild; // Bypass current via left child
        }
    }
    // Case 2b: Node has ONLY a right child (no left child)
    else if (current->leftChild == nullptr) {
        if (current == root) {                 // If deleting root
            root = current->rightChild;        // Promote right child to root
        } else if (isLeftChild) {              // If node was parent's left child
            parent->leftChild = current->rightChild; // Bypass current via right child
        } else {                               // If node was parent's right child
            parent->rightChild = current->rightChild; // Bypass current via right child
        }
    }
    // Case 3: Node has TWO children -> replace with In-Order Successor
    else {
        RoomNode* successor = getInOrderSuccessor(current); // Find replacement node
        if (current == root) {                 // If deleting root
            root = successor;                  // Promote successor to root
        } else if (isLeftChild) {              // If node was parent's left child
            parent->leftChild = successor;     // Connect parent's left to successor
        } else {                               // If node was parent's right child
            parent->rightChild = successor;    // Connect parent's right to successor
        }
        successor->leftChild = current->leftChild; // Reconnect deleted node's left subtree to successor
    }

    delete current->data;                      // Free the Room object memory
    delete current;                            // Free the RoomNode wrapper memory
    return true;                               // Confirm successful deletion
}

// Displays all rooms sorted in ascending roomNumber order (Session 15 Slide 24)
void RoomTree::DisplayInOrder(RoomNode* localRoot) const {
    if (localRoot != nullptr) {                // Base case: stop when branch is null
        DisplayInOrder(localRoot->leftChild);  // 1. Traverse left subtree (smaller room numbers)
        localRoot->Display();                  // 2. Visit/display current node
        DisplayInOrder(localRoot->rightChild); // 3. Traverse right subtree (larger room numbers)
    }
}