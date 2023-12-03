// Joel Gurivireddy
// jxg220051

#include "Node.h"
#include <iostream>

//If no informatio give about the Node created, set its linker pointers to null (meaning they don't link anywhere else)
Node::Node() {
    next = nullptr;
    down = nullptr;
}
//Create Node object with information if information given
Node::Node(Node* newNext, Node* newDown, Seat* newPayload) {
    next = newNext;
    down = newDown;
    payload = newPayload;
}

//access information about the current Node object
Node* Node::getNext() {
    return next;
}
Node* Node::getDown() {
    return down;
}
Seat* Node::getPayload() {
    return payload;
}

//change information about the given Node object
void Node::setNext(Node* newNext) {
    next = newNext;
}
void Node::setDown(Node* newDown) {
    down = newDown;
}
void Node::setPayload(Seat* newPayload) {
    payload = newPayload;
}

//print out the ticket type of the payload of the currenot node
char Node::operator<<(Node* nodePointer) {
    Seat* currentPayload = nodePointer -> getPayload();
    return *currentPayload << currentPayload;
}