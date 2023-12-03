// Joel Gurivireddy 
//jxg220051

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <sstream>

#include "Gold.h"
#include "Platinum.h"

using namespace std;

//Linked List class used in process of initializing preferred customer and regular customer arrays
class Node { 
  public:
    Node *next;
    Customer *customerPtr;

    Node(Node* next, Customer *customerPtr) {
      this -> next = next;
      this -> customerPtr = customerPtr;
    }
};

Customer** incrementPreferredList(Customer**, int&, Customer*);
Customer** decrementRegularList(Customer**, int&, int);
void destroyLinkedList(Node*);
Node* buildPreferredLinkedList(Node*, string, int&);
Node* buildLinkedList(Node*, string, int&);
double calculateCostOfOrder(string, string, double, int);
double calculateGoldOrderCost(Gold*, string, string, double, int);
double calculatePlatinumTotalCost(Platinum*, string, string, double, int);
void setGoldDiscount(Gold*);

int main() {
  //Set the preferred and regular customer arrays initially to null and their sizes to zero
  Customer **regularCustomers = nullptr;
  int numberOfRegularCustomers = 0;

  Customer **preferredCustomers = nullptr;
  int numberOfPreferredCustomers = 0;

  bool atLeastOneListOpened = false;

  //Read in regular customers from regular customer file
  string regularCustomerFile;
  cout << "Enter the regular customers file: " << endl;
  cin >> regularCustomerFile;

  Node *head = new Node(nullptr, nullptr);
  //Build list of regular customers using linked list to avoid having to resize regular customer array multiple times 
  if(buildLinkedList(head, regularCustomerFile, numberOfRegularCustomers) != nullptr) {
    //If regular customer file opened, at least one list opened, so we can continue with the orders
    atLeastOneListOpened = true;

    regularCustomers = new Customer*[numberOfRegularCustomers];
    Node *curr = head;
    int i = 0;
    //Copy customers stored in nodes of linked list over to regular customer array
    while(curr) {
      if(curr -> customerPtr != nullptr) {
        string firstName = curr -> customerPtr -> getFirstName();
        string lastName = curr -> customerPtr -> getLastName();
        string guestID = curr -> customerPtr -> getGuestID();
        double amountSpent = curr -> customerPtr -> getAmountSpent();
        regularCustomers[i++] = new Customer(firstName, lastName, guestID, amountSpent);
        cout << firstName << " " << lastName << " " << guestID << " " << amountSpent << endl;
      }
      curr = curr -> next;
    }
    //delete all the nodes of linked list after to save memory
    destroyLinkedList(head);
  }
  cout << endl;
  //Read in preferred customers from preferred customer file 
  string preferredCustomerFile;
  cout << "Enter the preferred customers file: " << endl;
  cin >> preferredCustomerFile;
  head = new Node(nullptr, nullptr);
  
  //Build linked list of preferred customers using linked list to avoid having to resize preferred customer array multiple times
  if(buildPreferredLinkedList(head, preferredCustomerFile, numberOfPreferredCustomers) != nullptr) {
    //If preferred customer file opened, at least one list opened, so we can continue with the orders
    atLeastOneListOpened = true;

    preferredCustomers = new Customer*[numberOfPreferredCustomers];
    Node *curr = head;
    int i = 0;
    //Copy customers stored in nodes of linked list over to preferred customer array
    while(curr) {
      if(curr -> customerPtr != nullptr) {
        if(dynamic_cast<Platinum*>(curr -> customerPtr)) {
          Platinum *plat = dynamic_cast<Platinum*>(curr -> customerPtr);
          string firstName = plat -> getFirstName();
          string lastName = plat -> getLastName();
          string guestID = plat -> getGuestID();
          double amountSpent = plat -> getAmountSpent();
          int bonusBucks = plat -> getBonusBucks();
          cout << firstName << " " << lastName << " " << guestID << " " << amountSpent << " " << bonusBucks << " " << endl;
          // Make new Platinum object by copying data over instead of just storing the original pointer itself to avoid issues when deleting the linked list after
          preferredCustomers[i++] = new Platinum(firstName, lastName, guestID, amountSpent, bonusBucks);
        }
        else if(dynamic_cast<Gold*>(curr -> customerPtr)) {
          Gold *gold = dynamic_cast<Gold*>(curr -> customerPtr);
          string firstName = gold -> getFirstName();
          string lastName = gold -> getLastName();
          string guestID = gold -> getGuestID();
          double amountSpent = gold -> getAmountSpent();
          double discount = gold -> getDiscountPercentage();
          cout << firstName << " " << lastName << " " << guestID << " " << amountSpent << " " << discount << "% " << endl;
          // Make new Gold object by copying data over instead of just storing the original pointer itself to avoid issues when deleting the linked list after 
          preferredCustomers[i++] = new Gold(firstName, lastName, guestID, amountSpent, discount);
        }
      }
      curr = curr -> next;
    }
    //delete all the nodes of linked list after to save memory
    destroyLinkedList(head);
  }
  cout << endl;
  if(!atLeastOneListOpened) {
    //This means no customers were found, so we terminate the program here
    cout << "No customers were found." << endl;
    return 0;
  }

  //Process Orders:
  ifstream input;
  string ordersFile;
  cout << "Enter the orders file: " << endl;
  cin >> ordersFile;
  input.open(ordersFile);
  //Open orders file, make sure it opened successfully
  if(!input.is_open()) {
    cout << "Orders file failed to open" << endl;
  }
  else {
    string currentLine;
    while(getline(input, currentLine)) {
      // Get current order, and run some validation checks to make sure the order is valid
      stringstream sstream(currentLine);

      string guestID = "";
      sstream >> guestID;

      //Make sure that guestID actually has a value and is all digits, otherwise input is invalid and we continue to next line of input
      if(guestID != "") {
        bool isAllDigits = true;
        for(unsigned int i = 0; i < guestID.size(); i++) {
          if(!isdigit(guestID.at(i))) {
            isAllDigits = false;
            break;
          }
        }
        if(!isAllDigits) {continue;}
      }
      else {continue;}

      string drinkSize = "";
      sstream >> drinkSize;
      //Make sure that drink size is a letter representing small, medium, or large, otherwise input is invalid and we continue to next line of input
      if(drinkSize != "S" && drinkSize != "M" && drinkSize != "L") {continue;}

      string drinkType = "";
      sstream >> drinkType;
      //Make sure drinkType is either soda, tea, or punch. Otherwise input is invalid and we continue to next line of input
      if(drinkType != "soda" && drinkType != "tea" && drinkType != "punch") {continue;}
      
      double squareInchPrice = -1.0;
      sstream >> squareInchPrice;
      //Make sure we actually got a valid double value for the square inch price, otherwise input is invalid for this order
      if(squareInchPrice == -1.0) {continue;}
      
      int quantity = -1;
      sstream >> quantity;
      //Make sure that we actually get valid integer value for quantity, otherwise order is invalid 
      if(quantity == -1) {continue;}

      //Check if there is any extra input in the current order line. If there is, the whole order is valid and we skip it 
      string dummy = "";
      sstream >> dummy;
      if(dummy != "") {continue;}

      cout << guestID << " " << drinkSize << " " << drinkType << " " << squareInchPrice << " " << quantity << endl;

      bool foundCustomer = false;
      Customer *currentCustomer = nullptr;
      int currentCustomerIndex = -1;

      for(int i = 0; i < numberOfRegularCustomers; i++) {
        //Search for the customer that made the current order we are looking at by matching the given guest ID
        if(regularCustomers[i] -> getGuestID() == guestID) {

          currentCustomer = regularCustomers[i];
          currentCustomerIndex = i;

          foundCustomer = true;
          break;
        }
      }
      if(!foundCustomer) {
        //If we haven't found the customer this order belongs to in the regular customer file, search the preferred customer file for the customer
        for(int i = 0; i < numberOfPreferredCustomers; i++) {
          if(preferredCustomers[i] -> getGuestID() == guestID) {
            currentCustomer = preferredCustomers[i];
            currentCustomerIndex = i;
            break;
          }
        }
      }
      //If we haven't found the customer this order belongs to in the regular or preferred customer file, we skip this order and move on to the next
      if(currentCustomerIndex == -1) {
        continue;
      }
      
      //Calculate the total cost of the order
      double orderCost = 0.0;

      if(dynamic_cast<Platinum*>(currentCustomer)) {
        //If the customer is a platinum customer, calculate the order cost for platinum customers
        Platinum *platinumCustomer = dynamic_cast<Platinum*>(currentCustomer);
        
        orderCost = calculatePlatinumTotalCost(platinumCustomer, drinkSize, drinkType, squareInchPrice, quantity);
        cout << "order cost: " << orderCost << endl;
        platinumCustomer -> setAmountSpent(platinumCustomer -> getAmountSpent() + orderCost);
      }

      else if(dynamic_cast<Gold*>(currentCustomer)) {
        //If the customer is a gold customer, calculate the order cost for gold customers
        Gold *goldCustomer = dynamic_cast<Gold*>(currentCustomer);
        orderCost = calculateGoldOrderCost(goldCustomer, drinkSize, drinkType, squareInchPrice, quantity);
        goldCustomer -> setAmountSpent(goldCustomer -> getAmountSpent() + orderCost);
        if(goldCustomer -> getAmountSpent() >= 200) {
          //If gold customer spent at least 200 dollars after the order, turn them into a platinum customer by copying all the data to a new Platinum object and deleting the old Gold object
          Platinum *platinumPromoted = new Platinum(goldCustomer -> getFirstName(), goldCustomer -> getLastName(), goldCustomer -> getGuestID(), goldCustomer -> getAmountSpent(), static_cast<int>(goldCustomer -> getAmountSpent() - 200) / 5);
          preferredCustomers[currentCustomerIndex] = platinumPromoted;
          delete goldCustomer;
        }

        else {
          //If the customer is still a gold customer, change the discount if they have spent enough money
          setGoldDiscount(goldCustomer);
        }
      }
      else if(dynamic_cast<Customer*>(currentCustomer)) {
        //If the customer is a regular customer, calculate the cost using the method for regular customers
        orderCost = calculateCostOfOrder(drinkSize, drinkType, squareInchPrice, quantity);
        currentCustomer -> setAmountSpent(currentCustomer -> getAmountSpent() + orderCost);
        if(currentCustomer -> getAmountSpent() >= 200) {
          //If the regular customer has spent enough money to bypass the Gold promotion directly to Platinum, change them to a Platinum customer by copying the data over to a new Platinum object and deleting the old Customer object
          Platinum *platCustomer = new Platinum(currentCustomer -> getFirstName(), currentCustomer -> getLastName(), currentCustomer -> getGuestID(), currentCustomer -> getAmountSpent(), static_cast<int>(currentCustomer -> getAmountSpent() - 200) / 5);
          //Increment the preferred customers array to accomodate this new platinum customer and decrement the regular customers array, since this customer is no longer a regular customer
          preferredCustomers = incrementPreferredList(preferredCustomers, numberOfPreferredCustomers, platCustomer);
          regularCustomers = decrementRegularList(regularCustomers, numberOfRegularCustomers, currentCustomerIndex);
          delete currentCustomer;
        }
        else if(currentCustomer -> getAmountSpent() > 49.99) {
          //Otherwise, if the current customer is still good enough to be promoted to Gold, turn them to a Gold customer by creating a new Gold object and copying over all the data to a new Gold object and deleting the old customer object.
          Gold *goldCustomer = new Gold(currentCustomer -> getFirstName(), currentCustomer -> getLastName(), currentCustomer -> getGuestID(), currentCustomer -> getAmountSpent(), 0.05);
          //Set the discount of the new Gold customer accordingly
          setGoldDiscount(goldCustomer);
          //Increment the preferred customers array to accomodate this new platinum customer and decrement the regular customers array, since this customer is no longer a regular customer
          preferredCustomers = incrementPreferredList(preferredCustomers, numberOfPreferredCustomers, goldCustomer);
          regularCustomers = decrementRegularList(regularCustomers, numberOfRegularCustomers, currentCustomerIndex);
          delete currentCustomer;
        }
      } 
    }
  }
  cout << endl;
  ofstream output;
  output.open("customer.dat");
  //Open a new customer file, and write the current state of the regular customers into that file
  for(int i = 0; i < numberOfRegularCustomers; i++) {
    output << regularCustomers[i] -> getGuestID() << " ";
    cout << regularCustomers[i] -> getGuestID() << " ";
    output << regularCustomers[i] -> getFirstName() << " ";
    cout << regularCustomers[i] -> getFirstName() << " ";
    output << regularCustomers[i] -> getLastName() << " ";
    cout << regularCustomers[i] -> getLastName() << " ";
    output << fixed << setprecision(2) << regularCustomers[i] -> getAmountSpent() << endl;
    cout << fixed << setprecision(2) << regularCustomers[i] -> getAmountSpent() << endl;
  }
  output.close();

  cout << endl;

  ofstream output2;
  output2.open("preferred.dat");
  //Open another new customer file, and write the current state of the preferred customers into that file
  for(int i = 0; i < numberOfPreferredCustomers; i++) {
    output2 << preferredCustomers[i] -> getGuestID() << " ";
    cout << preferredCustomers[i] -> getGuestID() << " ";
    output2 << preferredCustomers[i] -> getFirstName() << " ";
    cout << preferredCustomers[i] -> getFirstName() << " ";
    output2 << preferredCustomers[i] -> getLastName() << " ";
    cout << preferredCustomers[i] -> getLastName() << " ";
    output2 << fixed << setprecision(2) << preferredCustomers[i] -> getAmountSpent() << " ";
    cout << fixed << setprecision(2) << preferredCustomers[i] -> getAmountSpent() << " ";

    if(dynamic_cast<Gold*>(preferredCustomers[i])) {
      //If the current customer is a Gold customer, write their discount percentage to the output file 
      Gold *gold = dynamic_cast<Gold*>(preferredCustomers[i]);
      output2 << fixed << setprecision(2) << static_cast<int>(gold -> getDiscountPercentage() * 100) << "%" << endl;
      cout << fixed << setprecision(2) << static_cast<int>(gold -> getDiscountPercentage() * 100) << "%" << endl;
    }
    else if(dynamic_cast<Platinum*>(preferredCustomers[i])) {
      //If the current customer is a Platinum customer, write their number of bonus bucks to the output file
      Platinum *platinum = dynamic_cast<Platinum*>(preferredCustomers[i]);
      output2 << platinum -> getBonusBucks() << endl;
      cout << platinum -> getBonusBucks() << endl;
    }
  }

  //delete all the regular customer pointers from the regular customer array. This will save memory
  for(int i = 0; i < numberOfRegularCustomers; i++) {
      delete regularCustomers[i];
  }
  //delete all the preferred customer pointers from the prefererd customer array. This will also save memory 
  for(int i = 0; i < numberOfPreferredCustomers; i++) {
      delete preferredCustomers[i];
  }
  //After deleting each value in each array, delete the arrays themselves. This will save even more memory 
  delete[] regularCustomers;
  delete[] preferredCustomers;

  output2.close();

}

Customer** decrementRegularList(Customer **regularCustomers, int& numberOfRegularCustomers, int currentCustomerIndex) {
  //Create a temporary regular customer array that will hold one less customer
  Customer **newRegularCustomers = new Customer*[numberOfRegularCustomers - 1];
  int i = 0, j = 0;
  //add all the regular customers to the temporary array, except for the customer that we are trying to remove from the regular customer array
  while(j < numberOfRegularCustomers) {
    if(j != currentCustomerIndex) {
      newRegularCustomers[i] = regularCustomers[j];
      i++;
    }
    j++;
  }
  //turn the original regular customer array into a new array, containing new Customer objects with the same data member values as the Customers in the temporary array, to avoid memory issues when deleting the temporary array
  regularCustomers = new Customer*[numberOfRegularCustomers - 1];
  for(int i = 0; i < numberOfRegularCustomers - 1; i++) {
    string guestID = newRegularCustomers[i] -> getGuestID();
    string firstName = newRegularCustomers[i] -> getFirstName();
    string lastName = newRegularCustomers[i] -> getLastName();
    double amountSpent = newRegularCustomers[i] -> getAmountSpent();
    regularCustomers[i] = new Customer(firstName, lastName, guestID, amountSpent);
    //delete each Customer from the temporary array to save memory, after copying the data over to the original regular customer array 
    delete newRegularCustomers[i];

  }
  //delete the temporary array itself to save even more memory 
  delete[] newRegularCustomers;
  //decrement the number of regular customers after, to keep track of the number of regular customers so far
  numberOfRegularCustomers--;
  return regularCustomers;

}

Customer** incrementPreferredList(Customer **preferredCustomers, int& numberOfPreferredCustomers, Customer *newPreferredCustomer) {
  //Create a temporary array that will hold all the original array's values, plus the new preferred customer
  Customer **newPreferredCustomers = new Customer*[numberOfPreferredCustomers + 1];
  for(int i = 0; i < numberOfPreferredCustomers; i++) {
    newPreferredCustomers[i] = preferredCustomers[i];
  }
  //Add the customer as a Platinum customer if they are Platinum
  if(dynamic_cast<Platinum*>(newPreferredCustomer)) {
    Platinum *temp = dynamic_cast<Platinum*>(newPreferredCustomer);
    newPreferredCustomers[numberOfPreferredCustomers] = temp;
  }
  //Add the customer as a Gold customer if they are Gold
  else if(dynamic_cast<Gold*>(newPreferredCustomer)) {
    Gold *temp = dynamic_cast<Gold*>(newPreferredCustomer);
    newPreferredCustomers[numberOfPreferredCustomers] = temp;
  }
  //Reset the original array, and copy all the values from the temporary array to the original array
  preferredCustomers = new Customer*[numberOfPreferredCustomers + 1];
  for(int i = 0; i < numberOfPreferredCustomers + 1; i++) {
    //Copy all the data members of each customer into a new customer object, then add that new customer object to the original preferred customer array to avoid memory issues when deleting the temporary array 
    string guestID = newPreferredCustomers[i] -> getGuestID();
    string firstName = newPreferredCustomers[i] -> getFirstName();
    string lastName = newPreferredCustomers[i] -> getLastName();
    double amountSpent = newPreferredCustomers[i] -> getAmountSpent();

    if(dynamic_cast<Gold*>(newPreferredCustomers[i])) {
      //Add a Gold version of the current customer if the customer is Gold
      Gold *temp = dynamic_cast<Gold*>(newPreferredCustomers[i]);
      preferredCustomers[i] = new Gold(firstName, lastName, guestID, amountSpent, temp -> getDiscountPercentage());
    }
    else if(dynamic_cast<Platinum*>(newPreferredCustomers[i])) {
      //Add a Platinum version of the current customer if the customer is Platinum
      Platinum *temp = dynamic_cast<Platinum*>(newPreferredCustomers[i]);
      preferredCustomers[i] = new Platinum(firstName, lastName, guestID, amountSpent, temp -> getBonusBucks());
    }
    //Delete each Customer from the temporary array to save memory, after copying the data over to the original preferred customer array
    delete newPreferredCustomers[i];
  }
  //increment the number of preferred customers accordingly, and then delete the temporary array itself to save even more memory
  numberOfPreferredCustomers++;
  delete[] newPreferredCustomers;
  return preferredCustomers;
}

void destroyLinkedList(Node *head) {
  //Go through each node of the linked list, deleting each one to delete the entire linked list
  if(head == nullptr) {
    return;
  }
  else if(head -> next == nullptr) {
    delete head;
    return;
  }
  //Keep a next pointer so that we can access the next node after deleting the current node
  Node *next = head -> next;
  while(head) {
    delete head;
    head = next;
    if(next != nullptr) {
      next = next -> next;
    }
  }
}

Node* buildPreferredLinkedList(Node* head, string fileName, int& numberOfCustomers) {
  ifstream input;
  input.open(fileName);

  //Make sure that the file opened properly 
  if(!input.is_open()) {
    cout << "Input file failed to open!" << endl;
    return nullptr;
  }
  //Read each customer from the preferred customer file, then build each node to hold the current customer we are looking at 
  Node *traverser = head;

  while(!input.eof()) {
    //Take in the information for each customer from the file
    string guestID = "";
    string firstName = "";
    string lastName = "";
    double amountSpent = 0.0;

    string bonusBucksOrDiscount = "";
    input >> guestID >> firstName >> lastName >> amountSpent >> bonusBucksOrDiscount;


    int bonusBucks = 0;
    double discount = 0.0;

    //Check that we reached the end of the file by seeing if guestID didn't change its value, meaning that there is no real input left. After this, we reached the end of the file and can break the while loop
    if(guestID == "") break;

    if(bonusBucksOrDiscount.at(bonusBucksOrDiscount.size() - 1) == '%') {
      //A percentage sign in the last part of the line of the input means that the value is a discount percentage, meaning that we are currently looking at the information for a Gold customer 
      //Create a new Gold custome object with the information we just read in, and then add it to the linked list
      discount = stod(bonusBucksOrDiscount.substr(0, 2)) / 100.0;
      Gold *goldCustomer = new Gold(firstName, lastName, guestID, amountSpent, discount);
      Node *currentNode = new Node(nullptr, goldCustomer);

      //If the head node is null, that means this is the first customer, so we make the node containing this customer the head node 
      if(head == nullptr) {
        head = currentNode;
        traverser = currentNode;
      }
        //Otherwise, connect the previous node to the current node 
      else {
        traverser -> next = currentNode;
        traverser = traverser -> next;
      }          
    }

    else {
      //If there is no percentage sign, the last part represents a number of bonus bucks, meaning that we are looking at a Platinum customer 
      bonusBucks = static_cast<int>(stoi(bonusBucksOrDiscount));
      //Create a new Platinum customer object with the information we just read in, and then add it to the linked list
      Platinum *platinumCustomer = new Platinum(firstName, lastName, guestID, amountSpent, bonusBucks); 
      Node *currentNode = new Node(nullptr, platinumCustomer);

      //If the head node is null, that means this is the first customer, so we make the node containing this customer the head node 
      if(head == nullptr) {
        head = currentNode;
        traverser = currentNode;
      }
      //Otherwise, add the node containing this customer to the linked list by connecting the previous node to this node
      else {
        traverser -> next = currentNode;
        traverser = traverser -> next;
      }
    }
    //Increment the number of preferred customers by 1 each time we add a new preferred customer to the linked list
    numberOfCustomers++;

  }

  input.close();
  return head;
}

Node* buildLinkedList(Node *head, string fileName, int& numberOfCustomers) {
  ifstream input;
  input.open(fileName);
  Node *traverser = head;
  //Make sure the file opened correctly first before reading from it
  if(!input.is_open()) {
    cout << "Input file failed to open!" << endl;
    return nullptr;
  }

  while(!input.eof()) {
    //Read the information for the current customer we are looking at from the file 
    string guestID = "";
    string firstName = "";
    string lastName = "";
    double amountSpent = 0.0;
    input >> guestID >> firstName >> lastName >> amountSpent;
    //If guestID hasn't changed its value, there is no real input left, meaning that we have reached the end of the file, and so we terminate the file reading 
    if(guestID == "") break;

    //Create a new customer object with the give customer information
    Customer *currentCustomer = new Customer(firstName, lastName, guestID, amountSpent);
    Node *currentNode = new Node(nullptr, currentCustomer);

    //If the head node is null, we are looking at the first customer, so we make the node containing this customer the head node of the linked list 
    if(head == nullptr) {
      head = currentNode;
      traverser = currentNode;
    }
    //Otherwise, add this customer's node to the linked list by connecting the previous node to this customer's node
    else {
      traverser -> next = currentNode;
      traverser = traverser -> next;
    }

    //Increment the number of regular customers by 1 each time we add a new regular customer to the linked list 
    numberOfCustomers++;
  }

  input.close();
  return head;
}

void setGoldDiscount(Gold *goldCustomer) {
  //Increase the Gold customer's discount to 10% if they spent between $99.99 and $149.99
  if(goldCustomer -> getAmountSpent() > 99.99) {
    goldCustomer -> setDiscountPercentage(0.1);
  }
  //Increase the Gold customer's discount to 15% if they've spent at least $150
  if(goldCustomer -> getAmountSpent() > 149.99) {
            goldCustomer -> setDiscountPercentage(0.15);
          }
}

double calculateGoldOrderCost(Gold *goldCustomer, string drinkSize, string drinkType, double squareInchPrice, int quantity) {
  //Calculate the regular cost of the order, and apply the discount to that order 
  return calculateCostOfOrder(drinkSize, drinkType, squareInchPrice, quantity) * (1-goldCustomer -> getDiscountPercentage());
}

double calculatePlatinumTotalCost(Platinum *platinumCustomer, string drinkSize, string drinkType, double squareInchPrice, int quantity) {
  //Calculate the regular cost of the order, then apply the most amount of bonus bucks possible to that order
  double cost = calculateCostOfOrder(drinkSize, drinkType, squareInchPrice, quantity);
  int originalBB = platinumCustomer -> getBonusBucks();
  cout << "originalBB: " << originalBB << " for guest " << platinumCustomer -> getGuestID() << endl;
  cout << "cost: " << cost << "for guest " << platinumCustomer -> getGuestID() << endl;
  //If the Platinum customer has enough bonus bucks to cover the entire cost of the order, use only as bonus bucks as needed to bring the order down to $0.00. Use a bonus buck even if that makes the total value of the order go below $0.00
  if(originalBB >= cost) {
    //Subtract the total number of bonus bucks used from the Platinum customer
    platinumCustomer -> setBonusBucks(static_cast<int>(originalBB - ceil(cost)));
    return 0;
  }
  else {
    cost -= static_cast<double>(originalBB);
    cout << "new cost: " << cost << "for guest " << platinumCustomer -> getGuestID() << endl;
    //Otherwise, if the total cost of the order exceeds the total value of the customer's bonus bucks, use all the bonus bucks. Add more bonus bucks for each multiple of 5 over $200 dollars or more reached in total money spent by the customer, then subtract the total number of bonus bucks used
    int numBB = 0;
    double amntSpent = platinumCustomer -> getAmountSpent();
    for(int i = static_cast<int>(amntSpent + 1); i <= static_cast<int>(amntSpent + cost); i++) {
      if(i % 5 == 0) {
        numBB++;
      }
    }
    platinumCustomer -> setBonusBucks(numBB);
    //platinumCustomer -> setBonusBucks(static_cast<int>(cost) / 5);
    // platinumCustomer -> setBonusBucks(static_cast<int>(platinumCustomer -> getAmountSpent() + cost - 200) / 5 - originalBB);
    cout << "total amount spent: " << platinumCustomer -> getAmountSpent() << endl;
    cout << " new bonusBucks: " << platinumCustomer -> getBonusBucks() << "for guest " << platinumCustomer -> getGuestID() << endl;
    cout << endl;
    //platinumCustomer -> setAmountSpent(platinumCustomer -> getAmountSpent() + cost);
    return cost;
    
  }
}

double calculateCostOfOrder(string drinkSize, string drinkType, double pricePerSquareInch, int quantity) {
  //Calculate the regular cost of each order
  int numberOfOunces = 0;
  double pricePerOunce = 0;
  double surfaceArea = 0;

  //Calculate the surface area and the number of ounces of the drink, according to the size of the drink requested 
  switch(drinkSize.at(0)) {
    case 'S':
      numberOfOunces = 12;
      surfaceArea = M_PI * 4 * 4.5;
      break;
    case 'M':
      numberOfOunces = 20;
      surfaceArea = M_PI * 4.5 * 5.75;
      break;
    case 'L':
      numberOfOunces = 32;
      surfaceArea = M_PI * 5.5 * 7;
  }

  //Calculate the price per ounce of the drink, according to the type of drink requested
  if(drinkType == "soda") {
    pricePerOunce = 0.2;
  }
  else if(drinkType == "tea") {
    pricePerOunce = 0.12;
  }
  else if(drinkType == "punch") {
    pricePerOunce = 0.15;
  }

  //calculate the total cost of the order for the number of cups requested by multiplying the quantity of cups to the price of each cup requested, which includes the Disney graphics requested (calculated by the total surface area of the cup not includign the top or bottom multiplied to the price per square inch of surface area)
  return quantity * (pricePerOunce * numberOfOunces + pricePerSquareInch * surfaceArea);  
}
