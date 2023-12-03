//Joel Gurivireddy
//jxg220051
#ifndef NODE_H
#define NODE_H

#include <iostream>

template<typename T>
class Node {
  private:  
    Node<T>* left = nullptr;
    Node<T>* right = nullptr;
    T* payload = nullptr;

  public:
    Node();
    Node(T* payload) {this -> payload = payload;}

    //Accesors to access the left and right children, as well as the node's payload (a DVD object)
    Node<T>* getLeft() const {return left;}
    Node<T>* getRight() const {return right;}
    T* getPayload() const {return payload;}

    //Mutators to change the left and right children, as well as the node's payload
    void setLeft(Node<T>* left) {this -> left = left;}
    void setRight(Node<T>* right) {this -> right = right;}
    void setPayload(T* payload) {this -> payload = payload;}

    //Overloaded insertion operator to output the node's content, which calls the payload DVD object's overloaded insertion operator
    friend std::ostream& operator<<(std::ostream& output, const Node<T>* node) {
      std::cout << node -> getPayload();
      return output;
    }
};

#endif
