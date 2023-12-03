//Joel Gurivireddy
//jxg220051
#include <iostream>
#include <fstream>
#include <sstream>
#include "BinTree.h"
#include "DVD.h"
using namespace std;

void addDVDCopies(BinTree<DVD>&, int, string);
void removeDVDCopies(BinTree<DVD>&, int, string);
void rentDVD(BinTree<DVD>&, string);
void returnDVD(BinTree<DVD>&, string);

int main() {
  //Read in the inventory data file
  cout << "Enter the inventory file: ";
  string invFile;
  cin >> invFile;
  //Open the inventory file, make sure that it opened successfully before continuing
  ifstream input(invFile);
  if(!input.is_open()) {
    cout << "File failed to open." << endl;
    return 0;
  }
  //Create the binary search tree
  BinTree<DVD> dvdTree;

  //Read in the movie inputs from the inventory file
  string currentLine;
  while(getline(input, currentLine)) {//Keep taking in input lines representing movies until there are no more movies left
    //Read in the movie name
    string movieName = "";
    int i = 1;
    while(currentLine.at(i) != '"') {
      movieName += currentLine.at(i++);
    }
    i+=2;
    //Read in the number of copies available by bypassing the commas, and reading until we hit another comma (thus reaching the end of the number)
    string temp = "";
    while(currentLine.at(i) != ',') {temp += currentLine.at(i++);}
    int numAvailable = stoi(temp);
    i++;
    //Read in the number of copies rented by reading the rest of the input line, which should just be that number 
    temp = currentLine.substr(i, currentLine.size() - i);
    int numRented = stoi(temp);
    //Create a new DVD object with the movie name, number of copies available, and number of copies rented, and insert that DVD object into the binary search tree (which will insert it as a node)
    DVD *dvd = new DVD(movieName, numAvailable, numRented);
    dvdTree.insert(dvd);
  }
  input.close();
  //dvdTree.outputInOrderTraversal();

  //Read in the name of the transaction log file
  cout << "Enter transaction log file: ";
  string transactionLog;
  cin >> transactionLog;

  //open the transaction log file, but make sure it opens successfully before continuing
  input.open(transactionLog);
  if(!input.is_open()) {
    cout << "Trasaction log failed to open." << endl;
    return 0;
  }

  while(getline(input, currentLine)) {//Keep reading in input lines representing transactions until there are no more transactions left
    stringstream ss(currentLine);
    //Read in the title of the movie 
    string title = "";
    //Read in the command to be exceuted
    string command;
    ss >> command;
    if(command == "add") {
      //If the command is to add copies, first read in movie title
      int i = 5;
      while(currentLine.at(i) != '"') {
        title += currentLine.at(i++);
      }

      i+=2;
      string temp;

      //Next, read in the number of copies to be added to the total number available
      while(i < (int)currentLine.size()) {
        temp += currentLine.at(i++);
      }
      //Add the given number of copies to the node in the binary search tree containing the given movie title
      int numCopies = stoi(temp);
      addDVDCopies(dvdTree, numCopies, title);

    }

    else if(command == "rent") { //Next case: command is to rent a movie
      //Read in the title of the movie to be rented
      int i = 6;
      while(currentLine.at(i) != '"') {
        title += currentLine.at(i++);
      }
      //Rent the movie
      rentDVD(dvdTree, title);
    }

    else if(command == "return") {//Next case: command is to return a movie 
      //Read in the movie title
      int i = 8;
      while(currentLine.at(i) != '"') {
        title += currentLine.at(i++);
      }
      //Return the movie after being rented
      returnDVD(dvdTree, title);
    }

    else if(command == "remove") {//Next case: command is to remove a certain number of copies of the movie
      //Read in the movie name 
      int i = 8;
      while(currentLine.at(i) != '"') {
        title += currentLine.at(i++);
      }

      i += 2;
      //Read in the number of copies to remove 
      string temp;
      while(i < (int)currentLine.size()) {
        temp += currentLine.at(i++);
      }
      //Remove the number of copies desired
      int numCopies = stoi(temp);
      removeDVDCopies(dvdTree, numCopies, title);
    }
  }
  //After reading the transaction log file, close the input file reader, output all the movies in the binary search tree in alphabetical order, and finally all the nodes in the binary tree
  input.close();
  dvdTree.outputInOrderTraversal();
  dvdTree.deleteBinaryTree();

}

void addDVDCopies(BinTree<DVD> &dvdTree, int numCopies, string title) {//Function to add copies of a given movie 
  //Access a copy of the node in the binary tree containing the desired movie
  Node<DVD>* nodeCopy = dvdTree.search(title);
  //If the movie wasn't found, terminate the function
  if(nodeCopy == nullptr) {
    DVD* newDVD = new DVD(title, numCopies, 0);
    dvdTree.insert(newDVD);
    return;
  }
  //Add the number of desired copies to the current number of copies available
  nodeCopy -> getPayload() -> setAvailable(nodeCopy -> getPayload() -> getAvailable() + numCopies);
}

void rentDVD(BinTree<DVD> &dvdTree, string title) {//Function rent a movie
  //Access a copy of the node in the binary tree containing the desired movie
  Node<DVD>* nodeCopy = dvdTree.search(title);
  //If the movie wasn't found, terminate the function
  if(nodeCopy == nullptr) return;
  //Rent a movie by deducting 1 from the current number of copies available and adding 1 to the current number of copies rented
  nodeCopy -> getPayload() -> setAvailable(nodeCopy -> getPayload() -> getAvailable() - 1);
  nodeCopy -> getPayload() -> setRented(nodeCopy -> getPayload() -> getRented() + 1);
}

void returnDVD(BinTree<DVD> &dvdTree, string title) {//Function to return a movie
  //Access a copy of the node in the binary tree containing the desired movie
  Node<DVD>* nodeCopy = dvdTree.search(title);
  //If the movie wasn't found, terminate the function
  if(nodeCopy == nullptr) return;
  //Return a movie by adding 1 to the current number of copies available and deducting  1 from the current number of copies rented
  nodeCopy -> getPayload() -> setRented(nodeCopy -> getPayload() -> getRented() - 1);
  nodeCopy -> getPayload() -> setAvailable(nodeCopy -> getPayload() -> getAvailable() + 1);
}

void removeDVDCopies(BinTree<DVD> &dvdTree, int numCopies, string title) {//Function to remove copies of a given movie
  //Access a copy of the node in the binary tree containing the desired movie
  Node<DVD>* nodeCopy = dvdTree.search(title);
  //If the movie wasn't found, terminate the function 
  if(nodeCopy == nullptr) return;
  //Remove the number of desired copies from the current number of copies available
  nodeCopy -> getPayload() -> setAvailable(nodeCopy -> getPayload() -> getAvailable() - numCopies);
  //After removing copies, if the movie has no copies available AND no copies rented, delete it from the binary search tree
  if(nodeCopy -> getPayload() -> getAvailable() == 0 && nodeCopy -> getPayload() -> getRented() == 0) {
      dvdTree.deleteNode(title);
  }
}

