//Joel Gurivireddy
//jxg220051
#ifndef BINTREE_H
#define BINTREE_H

#include <iostream>
#include "Node.h"
#include "DVD.h"
#include <iomanip>

template<typename T>
class BinTree {
  private:
    //Create private functions that will help the public functions
    Node<T>* root;
    Node<T>* getParent(std::string title);
    void helper(Node<T>*, Node<T>*, Node<T>*);
    void helper(Node<T>* node);
    void deletionHelper(Node<T>* node);

  public:
    BinTree() {
      //Default constructor
      root = nullptr;
    }
    //Create the public functions that are used to manipulate the binary tree
    Node<T>* search(std::string);
    void insert(T*);
    void deleteNode(std::string);
    void outputInOrderTraversal();
    void deleteBinaryTree();
};

template<typename T> //Create a function to search for the node containing the DVD with the given title
Node<T>* BinTree<T>::search(std::string title) {
  //Create a traverser node to traverse the binary tree
  Node<T>* curr = root;
  while(curr) {
    //If we found the node with the DVD object containing the given title, return a copy of that node to ensure the user doesn't get direct access to nodes in the actual binary tree
    if(curr -> getPayload() -> getTitle() == title) {
      Node<T>* currCopy = new Node<T>(curr -> getPayload());
      return currCopy;
    }
    //If the title we're searching for is alphabetically greater than the title we're looking at, traverse to the right subtree, since that subtree contains all titles greater than the one we are looking at, possibly containing node with the title that we are trying to find
    else if(curr -> getPayload() -> getTitle() < title) {
      curr = curr -> getRight();
    }
    //Otherwise if the the title we're searching for is alphabetically smaller than the title we're currently looking at, traverse to the left subtree, since that subtree contains all titles smaller than the one we are looking at, possibly containing the node with the title that we are trying to find 
    else {
      curr = curr -> getLeft();
    }
  }
  //If we traverse through all reasonable node candidates and reach the bottom of the tree without finding the title we're looking for, it means that that title is not in the tree at all, so we just return null
  return nullptr;
}

template<typename T> //Function to insert nodes with new movie titles into the binary tree
void BinTree<T>::insert(T* payload) {
  //Create a traverser node to traverse the binary tree
  Node<T>* curr = root;
  //If the binary tree is empty, we make the root equal to the new node
  if(root == nullptr) {
    root = new Node<T>(payload);
    return;
  }
  while(curr) {
    //If the title we are inserting is alphabetically smaller than the title we are currently looking at, check to see if the node we are looking at has a left subtree
    if(*payload < *(curr -> getPayload())) {
      //If the node we are looking at does not have a left subtree, we insert our given title as the left child of the node we are currently looking at
      if(curr -> getLeft() == nullptr) {
        Node<T>* newNode = new Node<T>(payload);
        curr -> setLeft(newNode);
        return;
      }
      //Otherwise if the current node has a left subtree, we traverse down that left subtree, looking for a spot to insert the DVD object with our title there, since the left subtree contains all titles alphabetically less than the current title, which is the category in which the title we want to insert belongs to
      curr = curr -> getLeft();
    }
      //Otherwise if the title we are inserting is alphabetically greater than the title we are currently looking at, check to see if the node we are looking at has a right subtree. 
    else {
      //If the node we are looking at does not have a right subtree, we insert our given title as the right child of the node we are current looking at
      if(curr -> getRight() == nullptr) {
        Node<T>* newNode = new Node<T>(payload);
        curr -> setRight(newNode);
        return;
      }
      //Otherwise if the current node has a right subtree, we traverse down that right subtree, looking for a spot to insert the DVD object with our title there, since the right subtree contains all titles alphabetically less than the current title, which is the category in which the title we want to insert belongs to
      curr = curr -> getRight();
    }
  }
}

template<typename T> //Function to delete a node (if both the number of available and number of rented copies of the movie that the node contains are 0)
void BinTree<T>::deleteNode(std::string title) {
  if(root == nullptr) {
    //If the binary tree is empty, there is no node to delete, so we just terminate the function
    return;
  }
  else if(root -> getRight() == nullptr && root -> getLeft() == nullptr) {
    //If the root is the only node in the binary tree, check to see if the root contains the movie title we are looking for 
    if(root -> getPayload() -> getTitle() == title) {
      //If it does, delete the root as well as its payload, then terminate the function
      delete root -> getPayload();
      delete root;
      root = nullptr;
      return;
    }
    else {
      //If it doesn't, just terminate the function anyways without doing anything, since the binary tree doesn't have the node with the title that we want to delete
      return;
    }
  }

  //Otherwise, create a traverser node and find the parent of the node containing the title that we are looking for
  Node<T>* curr = root;
  Node<T>* parent = getParent(title);

  if(parent == nullptr) {
    //If the title doesn't exist, then the parent will be null, so we just terminate the function, not having to do any deletion
    return;
  }

  //If the title we want to delete is in the node that is the left child of the parent, we set the traverser node equal to that left child
  if(parent -> getLeft() != nullptr && parent -> getLeft() -> getPayload() -> getTitle() == title) {
    curr = parent -> getLeft();
  }
  //Otherwise if it is the right child that contains the title we are looking for, we set the traverser node equal to that right child
  if(parent -> getRight() != nullptr && parent -> getRight() -> getPayload() -> getTitle() == title) {
    curr = parent -> getRight();
  }
  //Call the helper function to finisht deleting the node
  helper(root, parent, curr);
}

template<typename T>
void BinTree<T>::helper(Node<T>* root, Node<T>* parent, Node<T>* curr) {
  //CASE 1: The current node we want to delete has either 0 children or 1 child

  //If there is no left subtree of the current node we want to delete, and that current node isn't the root itself (a.k.a no parent node), check if that current node is either the left or right child of the parent node
  if(curr -> getLeft() == nullptr && curr != root) {
    //If it is the left child, then set the left child of the parent node equal to the right child of the current node (since the left child of the current node is null)
    if(parent -> getLeft() == curr) {
      parent -> setLeft(curr -> getRight());
    }
    //Otherwise if it is the right child, set the right child of the parent node now to the right child of the current node
    else if(parent -> getRight() == curr) {
      parent -> setRight(curr -> getRight());
    }
    //After connecting the parent to the current node's child, severe the connection from the current node to its right child, then delete it, and terminate the function there, as we are done
    curr -> setRight(nullptr);
    delete curr;
    return;
  }
  //Otherwise, if there is no right subtree of the current node we want to delete, and that current node isn't the root itself (a.k.a no parent node), check once again if that node is either the left or right child of the parent node
  else if(curr -> getRight() == nullptr && curr != root) {
    //If it is the left child, then set the left child of the parent node now equal to the left child of the current node (since the right child of the current node is null)
    if(parent -> getLeft() == curr) {
      parent -> setLeft(curr -> getLeft());
    }
    //Otherwise if it is the right child, set the right child of the parent node now to the left child of the current node
    else if(parent -> getRight() == curr) {
      parent -> setRight(curr -> getLeft());
    }
    //After connecting the parent to the current node's child, severe the connection from the current node to its right child, then delete it, and terminate the function there since we are now done
    curr -> setLeft(nullptr);
    delete curr;
    return;
  }

  //CASE 2: The node we want to delete has 2 children. In this case, we have to find either the node containing the alphabetically smallest title in the right subtree, or find the node containing the alphabetically largest title in the left subtree. This algorithm will go with the former option
  else if((curr -> getLeft() != nullptr && curr -> getRight() != nullptr) || curr == root) {
    //First check if current node is the root itself. In that case, check if the root has a left or right pointer.
    if(curr == root && curr -> getRight() == nullptr) {
      //If the root has only a left child, delete the value of the root and make the root equal to the left child, and then terminate the function
      root = curr -> getLeft();
      delete curr;
      return;
    }
    else if(curr == root && curr -> getLeft() == nullptr) {
      //Otherwise if the root only has a right child, delete the value of the root and make the root equal to the right child, and then terminate the function 
      root = curr -> getRight();
      delete curr;
      return;
    }
    //If the current node that we want to delete is NOT a root, we then proceed to go down its right subtree and find the alphabetically smallest title in it
    parent = curr;
    Node<T>* minFinder = curr -> getRight();
    while(minFinder -> getLeft() != nullptr) {
      //Use a minFinder node traverser to reach the leftmost node of the right subtree, and update keep updating the parent to become the parent of that minFinder traverser
      parent = minFinder;
      minFinder = minFinder -> getLeft();
    }
    //After finding node with the smallest title in that right subtree, set the current node's payload equal to the payload of that node, thus replacing curr while keeping the Binary Search Tree ordering property (since the left subtree will still only have values less than the current node's title, while the right subtree will only have values greater than the current node's title).Before that however, we first delete the current node's original payload to ensure we free up that memory
    delete curr -> getPayload();
    curr -> setPayload(minFinder -> getPayload());
    //After, recursively call the function again to delete the minFinder node (since we don't need it anymore). Recursively calling this again will only go to Case 1, since our new current node is gauranteed not to have 2 children (since it is the leftmost node of the right subtree)
    helper(root, parent, minFinder);
  }
}

template<typename T> //Templated function to find the parent of the node containing the title we are looking for
Node<T>* BinTree<T>::getParent(std::string title) {
  //Set up traverser nodes to find the parent and the actual node containing the title we are looking for 
  Node<T>* parent = root;
  Node<T>* curr = root;

  while(curr) {
    if(curr -> getPayload() -> getTitle() == title) {
      //If we found the node containing the title we are looking for, return its parent (which will be one level above it )
      return parent;
    }
    //Otherwise, set the parent equal to the current node, and then update the current node to go another level down the binary tree
    parent = curr;
    if(title > curr -> getPayload() -> getTitle()) {
      //If the title we are looking for is alphabetically greater than the current title we are looking at, traverse down the right subtree to look for alphabetically greater titles, one of which might be the one we are looking for 
      curr = curr -> getRight();
    }
    //Otherwise if the title we are looking for is alphabetically smaller than the current title we are looking at, traverse down the left subtree to look for alphabetically smaller titles, one of which might be the one we are looking for 
    else {
      curr = curr -> getLeft();
    }
  }
  //If we traverse down all the levels of the binary tree and still haven't found the node, just return null
  return nullptr;
}

template<typename T> //Templated function to output the binary tree in alphabetical order, based on the movie titles
void BinTree<T>::outputInOrderTraversal() {
  if(root == nullptr) {
     return;
  }
  //Separate function calls helper function to do the in-order traversal to ensure that the user doesn't get any access to the nodes in the tree
  helper(root);
}

template<typename T> //Templated function to make sure that
void BinTree<T>::helper(Node<T>* node) {
  //Make the base case of the recursive function, so that the function will terminate at null nodes and go back up to leaf nodes and then other nodes, printing their contents

  if(node != nullptr && node -> getPayload() != nullptr) { 
    //Use in-order traversal, visiting the left subtree, then printing the current node, then visiting the right subtree (since left subtree nodes are alphabetically smaller, while right subtree nodes are alphabetically greater than the current node, thus ensuring the movie titles are printed in alphabetical order)
    helper(node -> getLeft());
    std::cout << node;
    helper(node -> getRight());
  }
}

template<typename T> //Templated Function to delete the nodes in the binary tree once the program completes
void BinTree<T>::deleteBinaryTree() {
  //Call the deletionHelper function to delete the binary tree to ensure no direct access to the nodes in the actual binary tree
  deletionHelper(root);
}

template<typename T> //Templated Function to help delete the nodes in the binary tree
void BinTree<T>::deletionHelper(Node<T>* node) {
  //Start with the base case of the recursive function so that it does not recursively call itself until there is no more memory left. We terminate when we reach a null node. This makes sure that the function ends up going back to the leaf node after seeing that its left and right children are null, so that the leaf node will be deleted
  if(node != nullptr && node -> getPayload() != nullptr) {
    //Use a recursive post-order traversal (recursively visit the left nodes and delete those, then recursively visit the right nodes and delete those, and finally delete the current node that we are looking at)
    deletionHelper(node -> getLeft());
    deletionHelper(node -> getRight());
    //When deleting each node, delete its payload (the DVD object it contains) as well as the node itself
    delete node -> getPayload();
    delete node;
  }
}

#endif 