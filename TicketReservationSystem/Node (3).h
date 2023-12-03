// Joel Gurivireddy
// jxg220051

#ifndef NODE_H
#define NODE_H

#include "Seat.h"
//Node class will carry the seat objects as "payloads". The nodes will serve to connect the seats together
class Node {
    private:
    // links to other nodes containing other seats, as well as the Seat payload for this Node object itself
        Node* next = nullptr;
        Node* down = nullptr;
        Seat* payload = nullptr;

    public:
        Node();
        Node(Node*, Node*, Seat*);

        //get information about the node object
        Node* getNext();
        Node* getDown();
        Seat* getPayload();

        //change information about the node object
        void setNext(Node*);
        void setDown(Node*);
        void setPayload(Seat*);

        char operator<<(Node*);
};

#endif