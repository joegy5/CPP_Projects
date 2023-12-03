#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <climits>
#include <cmath>
#include "Auditorium.h"
using namespace std;

class RowNumberNode { //RowNumberNode class used to keep track of the row number corresponding to each section in a customer order
  private:
    // RowNumberNode class attributes include the row number, starting seat letter, and the next row number (since this is a node in a linked list) of the section this node corresponds to. We include the starting seat letter as well to avoid attributing two different sections that happen to have the same row number to the same RowNumberNode
    int rowNumber = -1;
    char startingSeatLetter = '<';
    RowNumberNode* next = nullptr;
  public:
    RowNumberNode(int rowNumber, char startingSeatLetter) {this -> rowNumber = rowNumber; this -> startingSeatLetter = startingSeatLetter;}
    int getRowNumber() {return rowNumber;}
    char getStartingSeatLetter() {return startingSeatLetter;}
    RowNumberNode* getNext() {return next;}
    void setNext(RowNumberNode* next) {this -> next = next;}
};

class Section { //Section class used to keep track of each section in an order
  private:
    //Keep track of the section's starting seat letter, total tickets in the section, and total adult, child, and senior tickets
    char startingSeatLetter = 'a';
    int totalSectionTickets = 0;
    int adultTickets = 0;
    int childTickets = 0;
    int seniorTickets = 0;
    //Include an array characters with length 26
    char* seats;

  public:
    Section(char startingSeatLetter, int totalSectionTickets) {
      this -> startingSeatLetter = startingSeatLetter;
      this -> totalSectionTickets = totalSectionTickets; 
      seats = new char[totalSectionTickets];
      for(int i = 0; i < 26; i++) {
        seats[i] = '<';
      }
      for(int i = 0; i < totalSectionTickets; i++) {
        //Whichever seats are in the section are marked with their corresponding seat column letter in the seats array
        //Seats array also ensures the seats are kept in alphabetical order, as we start from the beginning of the array
        seats[startingSeatLetter + i - 'A'] = char(startingSeatLetter + i);
      }
    }

    char* getSeats() {return seats;}
    int getAdultTickets() {return adultTickets;}
    int getChildTickets() {return childTickets;}
    int getSeniorTickets() {return seniorTickets;}
    char getStartingSeatLetter() {return startingSeatLetter;}
    int getTotalSectionTickets() {return totalSectionTickets;}
    void setTotalSectionTickets(int totalSectionTickets) {this -> totalSectionTickets = totalSectionTickets;}

    void remove(char seat) {
      //TO remove a seat from a section, uninitialize its position in the seats array and decrement the total amount of seats in the section. The individual adult, child, and senior ticket amounts being decremented are taken care of later on in the code
      if(seats[seat - 'A'] != '<') {
        totalSectionTickets--;
      }
      seats[seat - 'A'] = '<';
    }
    bool isEmpty() {
      //If there are no tickets in the section, then the section is empty.
      return totalSectionTickets == 0;
    }
};

class Order {
  private:
    //In each order, keep track of the number of the auditorium the order was made in, the total adult, child, and senior tickets in htat order.
    int auditoriumNumber = 0;
    int adultTotalTickets = 0;
    int childTotalTickets = 0;
    int seniorTotalTickets = 0;

    //Use the first RowNumberNode and the last RowNumberNode as the head and tail (respectively) of a linked list of RowNumberNodes that are keys to the sectionMap map used to find sections the customer has reserved in the order
    RowNumberNode* firstRowNumber = nullptr;
    RowNumberNode* lastRowNumber = nullptr;
    unordered_map<RowNumberNode*, Section*> sectionMap;

    //The Order class itself will also be a linked list orders that the customer makes
    Order* next = nullptr;

  public:
    ~Order() {//To delete an order, delete every RowNumberNode pointer and the pointer pointing the section associated with that node in the RowNumberNode linked list.
      RowNumberNode* traverser = firstRowNumber;
      while(traverser != nullptr) {
        RowNumberNode* next = traverser -> getNext();
        delete sectionMap[traverser];
        delete traverser;
        traverser = next;
      } 
    }
    Order(int auditoriumNumber, int adultTotalTickets, int childTotalTickets, int seniorTotalTickets, int rowNumber, char startingSeatLetter) {
      this -> auditoriumNumber = auditoriumNumber;
      this -> adultTotalTickets = adultTotalTickets;
      this -> childTotalTickets = childTotalTickets;
      this -> seniorTotalTickets = seniorTotalTickets;
      this -> firstRowNumber = new RowNumberNode(rowNumber, startingSeatLetter);
      this -> lastRowNumber = firstRowNumber;

      Section* section = new Section(startingSeatLetter, adultTotalTickets + childTotalTickets + seniorTotalTickets);
      sectionMap[firstRowNumber] = section;
    }
    //Functions to retrieve or change the next order after the current one being looked at
    Order* getNext() {return next;}
    void setNext(Order* order) {next = order;}

    //Functions to retrieve the other attributes of the Order class
    unordered_map<RowNumberNode*, Section*> getSectionMap() {return sectionMap;}
    int getTotalTickets() {return adultTotalTickets + childTotalTickets + seniorTotalTickets;}
    int getAuditoriumNumber() {return auditoriumNumber;}
    int getAdultTotalTickets() {return adultTotalTickets;}
    int getChildTotalTickets() {return childTotalTickets;}
    int getSeniorTotalTickets() {return seniorTotalTickets;}
    void setAdultTotalTickets(int adultTotalTickets) {this -> adultTotalTickets = adultTotalTickets;}
    void setChildTotalTickets(int childTotalTickets) {this -> childTotalTickets = childTotalTickets;}
    void setSeniorTotalTickets(int seniorTotalTickets) {this -> seniorTotalTickets = seniorTotalTickets;}
    RowNumberNode* getFirstRowNumber() {return firstRowNumber;}
    //Function to retrieve a RowNumberNode* using its position in the RowNumberNode linked list
    RowNumberNode* getRowNumber(int rowNumber) {
      //Traverser the linked list a total of rowNumber times, then return the RowNumberNode we land on 
      RowNumberNode* currentRowNumber = firstRowNumber;
      while(currentRowNumber != nullptr && currentRowNumber -> getRowNumber() != rowNumber) {
        currentRowNumber = currentRowNumber -> getNext();
      }
      return currentRowNumber;
    }

    //Method to add a reserved section to the order
    void addSection(int rowNumber, char startingSeatLetter, int totalSectionTickets, int adultTickets, int childTickets, int seniorTickets) {
      //First go through the entire RowNumberNode linked list to check if the newly reserved section is in the same row as another section already in the order
      bool rowAlreadyExists = false;
      RowNumberNode* curr = firstRowNumber;
      while(curr != nullptr) {
        if(curr -> getRowNumber() == rowNumber) {
          rowAlreadyExists = true;
          break;
        }
        curr = curr -> getNext();
      }
      if(!rowAlreadyExists) {
        //If the section was in fact reserved in a new row, create a new section, and place it in the sectionMap, which can be accessed by using the new RowNumberNode created to contain that new row number as a key to the map
        Section* section = new Section(startingSeatLetter, totalSectionTickets);
        //Add the new RowNumberNode to the RowNumberNode linked list 
        lastRowNumber -> setNext(new RowNumberNode(rowNumber, startingSeatLetter));
        lastRowNumber = lastRowNumber -> getNext();
        sectionMap[lastRowNumber] = section;
      }
      else {
        //If the new section was reserved in a row that contains an already reserved section, combine this new section with that old section by updating the seats array of that old section to include all the new seats reserved in this new section
        for(int i = 0; i < totalSectionTickets; i++) {
          sectionMap.at(curr) -> getSeats()[i + startingSeatLetter - 'A'] = char(i + startingSeatLetter);
        }
        sectionMap.at(curr) -> setTotalSectionTickets(sectionMap.at(curr) -> getTotalSectionTickets() + totalSectionTickets);
      }
      //Update the total amount of adult, child, and senior tickets in the order
      adultTotalTickets += adultTickets;
      childTotalTickets += childTickets;
      seniorTotalTickets += seniorTickets;
    }
    //Method to remove a seat from the order
    bool removeSeat(int row, char seat) {
      //Try to find the seat
      bool seatFound = false;
      //Traverse the RowNumberNode linked list
      RowNumberNode* traverser = firstRowNumber;
      while(traverser != nullptr) {
        //First check if the current RowNumberNode we are looking at has the same row number as the row containing the seat we want to remove
        if(traverser -> getRowNumber() == row) {
          //If it does, check the seats array of the section that the curent RowNumberNode maps to (using the sectionMap)
          for(int i = 0; i < 26; i++) {
            //If the letter of the seat we are trying to remove is already initialized in the seats array, then we have found the RowNumberNode mapping to the section containing the seat we want to remove 
            if(sectionMap.at(traverser) -> getSeats()[i] == seat) {
              seatFound = true;
              break;
            }
          }
        }
        if(seatFound) {
          break;
        }
        traverser = traverser -> getNext();
      }
      if(seatFound) {
        //If we found the seat, remove it from that section mapped to by the RowNumberNode we found
        sectionMap.at(traverser) -> remove(seat);
        if(sectionMap.at(traverser) -> isEmpty()) {
          //If after removing the seat, there are no more initialized seats in the seats array of this section, erase the section and the corresponding RowNumberNode from the sectionMap
          sectionMap.erase(traverser);
        }
        return true;
      }
      return false;
    }
    bool isEmpty() {
      //Call the C++ unordered_map empty() method to check if the sectionMap is empty (meaning that the Order is empty)
      return sectionMap.empty();
    }
};

class Customer {
  private:
    //For each customer, keep track of the customer's username password, first order and last order (to form a linked list of orders), and the total number of orders made
    string username = "";
    string password = "";
    Order* firstOrder = nullptr;
    Order* lastOrder = nullptr;
    int totalNumberOfOrders = 0;

  public:
    ~Customer() {
      //To delete a customer, delete all the orders the customer has made (automatically calling each order's destructor)
      Order* traverser = firstOrder;
      while(traverser != nullptr) {
        Order* next = traverser -> getNext();
        delete traverser;
        traverser = next;
      }
    }
    Customer(string username, string password) {
      this -> username = username;
      this -> password = password;
    }
    
    //Methods to access the various attributes of each customer 
    int gettotalNumberOfOrders() {return totalNumberOfOrders;}
    string getUsername() {return username;}
    string getPassword() {return password;}
    Order* getFirstOrder() {return firstOrder;}
    Order* getLastOrder() {return lastOrder;}

    //Method to add an order to the customer's order list
    void addOrder(Order* order) {
      //First increment the total number of orders the customer has made
      totalNumberOfOrders++;
      //If the customer hasn't made any orders yet, point the first and last order to this new order, marking the beginning of the linked list of orders
      if(firstOrder == nullptr) {
        firstOrder = order;
        lastOrder = order;
        return;
      }
      //Otherwise, make this new order come after the last order, then make the last order point to this new order, thus adding the new order to the linked list
      lastOrder -> setNext(order);
      lastOrder = lastOrder -> getNext();
    }

    //Method to get an order at a certain position in the customer's linked list of orders
    Order* getOrder(int orderNumber) {
      //First make sure that the given order number is within the bounds of the total number of orders the customer has made. If not, just return null, as there is no order at the given position
      if(orderNumber <= 0 || orderNumber > totalNumberOfOrders) {
        return nullptr;
      }
      //Otherwise, traverse the linked list of orders a total of orderNumber times to access the Order at the desired position, and return that Order
      Order* order = firstOrder;
      int i = 1;
      while(i < orderNumber && order != nullptr) {
        order = order -> getNext();
        i++;
      }
      return order;
    }

    //Method to remove an order from the customer's linked list of orders
    void removeOrder(Order* order) {
      //First check if the first order is the one we are trying to delete
      if(firstOrder == order) {
        //If so, check that this order is the only one left. If it is, set both the first and last orders equal to null, meaning that there are no more orders for this current customer
        if(totalNumberOfOrders == 1) {
          firstOrder = nullptr;
          lastOrder = nullptr;
        }
        //If not, move the first order to be equal to the order coming after it in the linked list, then remove this order from the linked list
        else {
          Order* orderToBeDeleted = firstOrder;
          firstOrder = firstOrder -> getNext();
          orderToBeDeleted -> setNext(nullptr);
          orderToBeDeleted = nullptr;
        }
        totalNumberOfOrders--;
        return;
      }
      //Otherwise, remove the order from the linked list of orders by linking the previous order to the order after the one we want to delete, and breaking the link of the order we want to delete
      Order* currentOrder = firstOrder;
      while(currentOrder != nullptr && currentOrder -> getNext() != nullptr && currentOrder -> getNext() != order) {
        currentOrder = currentOrder -> getNext();
      }
      if(currentOrder -> getNext() == order) {
        currentOrder -> setNext(order -> getNext());
        order -> setNext(nullptr);
        order = nullptr;
      }
      //Decrement the total number of orders by 1 after removing the order
      totalNumberOfOrders--;
      //If there were originally only 2 orders left and we deleted the second one, set the last order equal to the first order to ensure that the last order is still linked to this linked list of orders
      if(totalNumberOfOrders == 1) {
        lastOrder = firstOrder;
      }
    }
};

void displayAuditorium(Node*, int);
bool gatherInput(bool, Order*, Auditorium&, Customer*, int);
void viewOrders(Customer*, bool);
void updateOrder(Customer*, Auditorium&, Auditorium&, Auditorium&);
void displayReceipt(Customer*);
void printReport(Auditorium&, Auditorium&, Auditorium&);
void printReportHelper(Auditorium&, int);
void writeToFile(Auditorium&, Auditorium&, Auditorium&, unordered_map<string, Customer*>&);
void writeToFileHelper(Auditorium&, int);
bool inputIsValid(string);
bool reserveSeats(bool, Order*, Auditorium&, int, Customer*, int, char, int, int, int);
bool bestAvailable(Auditorium&, Customer*, int, int, int, int, int, int);
void removeSeatFromAuditorium(int, char, Auditorium&, Order*);

int main() {
  //Build each of the 3 auditoriums by calling their constructors to read the auditorium files
  Auditorium Auditorium1("A1.txt");
  Auditorium Auditorium2("A2.txt");
  Auditorium Auditorium3("A3.txt");

  //Handle the case of bad input files for any of the three auditoriums
  if(Auditorium1.totalRows == 0 || Auditorium2.totalRows == 0 || Auditorium3.totalRows == 0) {
    cout << "Auditorium files failed to be read" << endl;
    return 0;
  }
  //create a map that maps customer usernames to customers
  unordered_map<string, Customer*> customerMap;

  //Read in the customer information (username and password for each customer)
  ifstream input("userdb.dat");
  string currentLine = "";
  while(getline(input, currentLine)) {
    stringstream ss(currentLine);
    //Read in the username of the customer
    string username = "";
    ss >> username;
    //Read in the password of the customer 
    string password = ""; 
    ss >> password;
    //Create a new customer with the given information, and add it to the customerMap
    Customer* customer = new Customer(username, password);
    customerMap[username] = customer;
  }

  bool exit = false;
  while(true) {
    //If the exit boolean variable is true, it means that the user was the admin, and decided to quit the entire program, so break out of the while loop
    if(exit) {break;}
    //Let the user login to their customer/admin account
    string username = "";
    //Read in the given username
    cout << "Enter the username: ";
    cin >> username;
    //Keep asking for the username until a username that actually maps to a customer in the database is given
    while(customerMap.find(username) == customerMap.end()) {
      cout << "This username was not found in our database. Please try again: ";
      cin >> username;
    }
    //Get the password
    string password = "";
    //Give the user 3 tries to enter the password
    int numberOfLoginTries = 0;
    while(true) {
      //If the user enters the wrong password 3 times, take them back to the login page and ask for a username again
      if(numberOfLoginTries == 3) {
        cout << "Too many login attempts. Going back to login page" << endl;
        break;
      }
      //Read in the password
      cout << "Enter the password: ";
      cin >> password;
      //If the user enters the correct password, break out of the while loop and go the next part of the program
      if(password == customerMap[username] -> getPassword()) {
        break;
      }
      else {
        //If the user enters the wrong password, get them to try again 
        cout << "Invalid password" << endl;
        numberOfLoginTries++;
      }
    }

    if(numberOfLoginTries == 3) {continue;}
    //If the current user is the admin, display the option to print the report of all the auditoriums, logout, or exit the entire program
    if((username) == "admin") {
      while(true) {
        cout << "Welcome to the admin page! Please select one of the following options:" << endl;
        cout << "1. Print Report" << endl;
        cout << "2. Logout" << endl;
        cout << "3. Exit" << endl;

        //Ask the admin for their choice, and keep asking until they enter either 1, 2, or 3 to represent what they want to do
        string choiceTemp = "";
        while(true) {
          cin >> choiceTemp;
          if(choiceTemp.size() == 1 && isdigit(choiceTemp.at(0))) {
            break;
          }
          else {
            cout << "Please enter a valid input" << endl;
            continue;
          }
        }

        int choice = stoi(choiceTemp);

        //If the admin wants a report of all the auditoriums, print the report by calling the printReport() function
        if(choice == 1) {
          printReport(Auditorium1, Auditorium2, Auditorium3);
        }
        //If the admin wants to logout, break from the while loop to bring the user back to the login page
        else if(choice == 2) {
          break;
        }
        //If the admin wants to end the program, finish off by writing all the auditoriums to new output files, then set the "exit" boolean variable to true to break out of all while loops and end the program
        else if(choice == 3) {
          writeToFile(Auditorium1, Auditorium2, Auditorium3, customerMap);
          exit = true;
          break;
        }
      }
    }
    else {
      //Find the Customer corresponding to the username the user entered
      Customer* currentCustomer = customerMap.at(username);
      while(true) {
        //Display the 5 options to the customer
        cout << "Welcome to the ticket reservation system! What would you like to do?" << endl;
        cout << "1. Reserve Seats" << endl;
        cout << "2. View Orders" << endl;
        cout << "3. Update Order" << endl;
        cout << "4. Display Receipt" << endl;
        cout << "5. Log Out" << endl;

        //Keep prompting until the customer gives valid input
        string choiceTemp = "";
        while(true) {
          cin >> choiceTemp;
          if(choiceTemp.size() == 1 && isdigit(choiceTemp.at(0))) {
            break;
          }
          else {
            cout << "Please enter a valid input" << endl;
            continue;
          }
        }
        int choice = stoi(choiceTemp);
        
        //If the customer wants to reserve seats, take them through the reservation process
        if(choice == 1) {
          //First ask them which auditorium they want to reserve seats in
          cout << "Which auditorium would you like to choose from?" << endl;
          cout << "1. Auditorium 1" << endl;
          cout << "2. Auditorium 2" << endl;
          cout << "3. Auditorium 3" << endl;

          //Keep prompting until the customer gives a valid auditorium number
          string auditoriumChoiceTemp = "";
          while(true) {
            cin >> auditoriumChoiceTemp;
            if(auditoriumChoiceTemp.size() == 1 && isdigit(auditoriumChoiceTemp.at(0))) {
              break;
            }
            else {
              cout << "Please enter a valid input" << endl;
              continue;
            }
          }
          int auditoriumChoice = stoi(auditoriumChoiceTemp);

          //Take the customer through the reservation process in the auditorium they want to reserve seats in
          if(auditoriumChoice == 1) {
            displayAuditorium(Auditorium1.first, Auditorium1.totalColumns);
            gatherInput(false, nullptr, Auditorium1, currentCustomer, 1);
          }

          else if(auditoriumChoice == 2) {
            displayAuditorium(Auditorium2.first, Auditorium2.totalColumns);
            gatherInput(false, nullptr, Auditorium2, currentCustomer, 2);
          }

          else if(auditoriumChoice == 3) {
            displayAuditorium(Auditorium3.first, Auditorium3.totalColumns);
            gatherInput(false, nullptr, Auditorium3, currentCustomer, 3);
          }
        }

        //If the customer wants to view all their orders, call the viewOrders() function to let them do so (unless the customer has no orders, calling for just printing "no orders")
        else if(choice == 2) {
          if(currentCustomer -> getFirstOrder() == nullptr) {
            cout << "No orders" << endl;
            continue;
          }
          viewOrders(currentCustomer, false);
        }

        //If the customer wants to update an order, call the updateOrder() function to let them do so (unless the customer has no orders, calling for just printing "no orders")
        else if(choice == 3) {
          if(currentCustomer -> getFirstOrder() == nullptr) {
            cout << "No orders" << endl;
            continue;
          }
          updateOrder(currentCustomer, Auditorium1, Auditorium2, Auditorium3);
        }

        //If the customer wants to see their receipt, call the displayReceipt() function to let them do so
        else if(choice == 4) {
          displayReceipt(currentCustomer);
        }

        //If the customer wants to logout, log them out by taking them back to the user page
        else if(choice == 5) {
          cout << "Returning to the login page..." << endl;
          break;
        }

        //Otherwise, ask the customer again for a valid choice number
        else {
          cout << "Please enter a valid choice number" << endl;
          continue;
        }
      }
    }
  }
}

//Method to delete an auditorium after the program finishes
void deleteAuditorium(Auditorium& auditorium) {
  //go through every node representing the seat in the auditorium and delete it
  Node* rowTraverser = auditorium.first;
  Node* columnTraverser = rowTraverser;
  while(rowTraverser != nullptr) {
    //Go through each row 
    Node* nextDown = rowTraverser -> getDown();
    while(columnTraverser != nullptr) {
      //Delete every node and its payload in the current row 
      Node* next = columnTraverser -> getNext();
      delete columnTraverser -> getPayload();
      delete columnTraverser;
      columnTraverser = next;
    }
    //Move on to the next row
    rowTraverser = nextDown;
  }
}

//Method to write each auditorium to its own file after the program finishes
void writeToFile(Auditorium& Auditorium1, Auditorium& Auditorium2, Auditorium& Auditorium3, unordered_map<string, Customer*>& customerMap) {
  //Write each auditorium to its own file
  writeToFileHelper(Auditorium1, 1);
  writeToFileHelper(Auditorium2, 2);
  writeToFileHelper(Auditorium3, 3);
  //Delete each auditorium after
  deleteAuditorium(Auditorium1);
  deleteAuditorium(Auditorium2);
  deleteAuditorium(Auditorium3);
  //Delete every Customer in the map last 
  for(auto mapping : customerMap) {
    Customer* curr = mapping.second;
    delete curr;
    customerMap.erase(mapping.first);
  }
}

//Method for writing a given auditorium to an output file
void writeToFileHelper(Auditorium& auditorium, int auditoriumNumber) {
  //Create an output file for the auditorium
  ofstream output("Auditorium" + to_string(auditoriumNumber) + "Final.txt");
  Node* rowTraverser = auditorium.first;
  Node* columnTraverser = auditorium.first;

  //Traverse each row of the auditorium
  while(rowTraverser != nullptr) {
    while(columnTraverser != nullptr) {
      //For each seat in the current row, write the seat's ticket type to the output file, then move on to the next seat in the row
      output << columnTraverser -> getPayload() -> getTicketType();
      columnTraverser = columnTraverser -> getNext();
    }
    output << endl;
    //After processing the current row, move on to the next 
    rowTraverser = rowTraverser -> getDown();
  }
}

//Method to print the report of all the auditoriums
void printReport(Auditorium& Auditorium1, Auditorium& Auditorium2, Auditorium& Auditorium3) {
  //print the report of each auditorium 
  printReportHelper(Auditorium1, 1);
  printReportHelper(Auditorium2, 2);
  printReportHelper(Auditorium3, 3);

  //Print the total number of open seats, reserved seats, adult, child, and senior tickets, and revenue for all the auditoriums combined
  cout << "Total";
  cout << setw(9) << right << Auditorium1.getOpen() + Auditorium2.getOpen() + Auditorium3.getOpen();
  cout << setw(5) << right << Auditorium1.getReserved() + Auditorium2.getReserved() + Auditorium3.getReserved();
  cout << setw(5) << right << Auditorium1.totalAdultTickets + Auditorium2.totalAdultTickets + Auditorium3.totalAdultTickets;
  cout << setw(5) << right << Auditorium1.totalChildTickets + Auditorium2.totalChildTickets + Auditorium3.totalChildTickets;
  cout << setw(5) << right << Auditorium1.totalSeniorTickets + Auditorium2.totalSeniorTickets + Auditorium3.totalSeniorTickets;
  cout << "   ";
  cout << setw(5) << right << "$" << fixed << setprecision(2) << (Auditorium1.getSales() + Auditorium2.getSales() + Auditorium3.getSales());
  cout << endl;
}

//Method to print the report of a given auditorium
void printReportHelper(Auditorium& auditorium, int auditoriumNumber) {
  //For each auditorium, print the number of open seats, reserved seats, adult, child, and senior tickets, and total sales for that auditorium
  cout << "Auditorium " << auditoriumNumber;
  cout << setw(5) << right << auditorium.getOpen();
  cout << setw(5) << right << auditorium.getReserved();
  cout << setw(5) << right << auditorium.totalAdultTickets;
  cout << setw(5) << right << auditorium.totalChildTickets;
  cout << setw(5) << right << auditorium.totalSeniorTickets;
  cout << setw(5) << right << "$" << fixed << setprecision(2) << auditorium.getSales();
  cout << endl;
}

void displayReceipt(Customer* customer) {
  //To display the receipt, call the viewOrders() function with the information that the receipt should be printed, so that the viewOrders() function can act accordingly
  viewOrders(customer, true);
}

//Method to update the customer's order
void updateOrder(Customer* customer, Auditorium& Auditorium1, Auditorium& Auditorium2, Auditorium& Auditorium3) {
  //First call the viewOrders() so that the customer can see which order they want to update 
  viewOrders(customer, false);
  Order* currentOrder = nullptr;
  while(true) {
    //Ask the customer for the number of the order they want to update, and keep looping until they enter a valid order number
    cout << "Enter the number of the order you would like to update: " << endl;
    string orderNumberTemp = "";
    while(true) {
      cin >> orderNumberTemp;
      if(inputIsValid(orderNumberTemp)) {
        break;
      }
      else {
        cout << "Please enter valid input" << endl;
      }
    }
    int orderNumber = stoi(orderNumberTemp);
    //Find the order that the customer wants
    currentOrder = customer -> getOrder(orderNumber);
    if(currentOrder != nullptr) {
      break;
    }
    else {
      cout << "Please enter a valid order number" << endl;
    }
  }
  while(true) {
    //Give the customer the option to ethier add tickets to, delete tickets from, or cancel the order
    cout << "1. Add tickets to order" << endl;
    cout << "2. Delete tickets from order" << endl;
    cout << "3. Cancel Order" << endl;
    
    //Ask the customer for the number of their choice, and keep looping until they give a valid choice number
    string choiceTemp = "";
    while(true) {
      cin >> choiceTemp;
      if(choiceTemp.size() == 1 && isdigit(choiceTemp.at(0))) {
        break;
      }
      else {
        cout << "Please enter a valid input" << endl;
        continue;
      }
    }

    int choice = stoi(choiceTemp);

    //if the customer wants to add tickets to the order, take them through the reservation process of the auditorium which they originally made the order in again 
    if(choice == 1) {
      if(currentOrder -> getAuditoriumNumber() == 1) {
        if(!gatherInput(true, currentOrder, Auditorium1, customer, 1)) {continue;}
        else {break;}
      }
      else if(currentOrder -> getAuditoriumNumber() == 2) {
        if(!gatherInput(true, currentOrder, Auditorium2, customer, 2)) {continue;}
        else {break;}
      }
      else if(currentOrder -> getAuditoriumNumber() == 3) {
        if(!gatherInput(true, currentOrder, Auditorium3, customer, 3)) {continue;}
        else {break;}
      }
      else {
        cout << "Invalid auditorium number" << endl;
      }
    }
    //If the customer wants to delete tickets from the order, ask them for the row number and seat letter of the seat that they want to delete
    else if(choice == 2) {
      //Keep looping until valid input is given for the row number
      string chosenRow;
      cout << "Enter the row that you would like: ";
      while(true) {
        cin >> chosenRow;
        if(inputIsValid(chosenRow)) {
            break;
        }
        else {
            cout << "Please enter valid input" << endl;
        }
      }
      cout << endl;
      //Ask the user for the desired column, keep asking until they give valid input
      cout << "Enter the seat that you would like: ";
      char chosenSeat;
      while(true) {
        cin >> chosenSeat;
        if(chosenSeat >= 'A' && chosenSeat <= 'Z') {
          break;
        }
        else {
          cout << "Please enter a valid input." << endl; 
        }
      }
      //If the seat removal was successful, first check if the order is empty
      if(currentOrder -> removeSeat(stoi(chosenRow), chosenSeat)) {
        if(currentOrder -> isEmpty()) {
          //If the order is empty, remove it from the list of orders the customer has made
          customer -> removeOrder(currentOrder);
        }
        //Unreserve the seat from the appropriate auditorium 
        switch(currentOrder -> getAuditoriumNumber()) {
          case 1:
            removeSeatFromAuditorium(stoi(chosenRow), chosenSeat, Auditorium1, currentOrder);
            break;
          case 2:
            removeSeatFromAuditorium(stoi(chosenRow), chosenSeat, Auditorium2, currentOrder);
            break;
          case 3:
            removeSeatFromAuditorium(stoi(chosenRow), chosenSeat, Auditorium3, currentOrder);
            break;
          default:
            break;
        }
        cout << "Ticket deleted!" << endl;
        break;
      }
      else {
        //If the seat wasn't removed succesfully, it means that the user did not actually reserve the seat at the position they gave. So take them through the update order process again 
        cout << "You have not reserved the seat that you chose. Taking you back to the update order menu" << endl;
        continue;
      }
    }
    else if(choice == 3) {
      //If the user wants to cancel the order:
      RowNumberNode* currRow = currentOrder -> getFirstRowNumber();
      //Go through each RowNumberNode in the order, and remove all the seats in each section that each RowNumberNode maps to using the order's sectionMap
      while(currRow != nullptr) {
        Section* section = currentOrder -> getSectionMap().at(currRow);
        for(int i = 0; i < 26; i++) {
          if(section -> getSeats()[i] != '<') {
            //Unreserve each seat from the appropriate auditorium 
            switch(currentOrder -> getAuditoriumNumber()) {
              case 1:
                removeSeatFromAuditorium(currRow -> getRowNumber(), section -> getSeats()[i], Auditorium1, currentOrder);
                break;
              case 2:
                removeSeatFromAuditorium(currRow -> getRowNumber(), section -> getSeats()[i], Auditorium2, currentOrder);
                break;
              case 3:
                removeSeatFromAuditorium(currRow -> getRowNumber(), section -> getSeats()[i], Auditorium3, currentOrder);
                break;
              default:
                break;
            }
          }
        }
        currRow = currRow -> getNext();
      }
      //After removing all the seats from the order, remove the order from the customer's list of orders
      customer -> removeOrder(currentOrder);
      cout << "Order cancelled!" << endl;
      break;
    }
    else {
      //If the user did not enter a valid choice number, prompt them again to do so. 
      cout << "Please enter a valid choice number (1, 2, or 3)" << endl;
    }
  }
}

//Method to unreserve a seat from the appropriate auditorium 
void removeSeatFromAuditorium(int chosenRow, char chosenSeat, Auditorium &auditorium, Order* currentOrder) {
  Node* node = auditorium.first;
  for(int i = 1; i < chosenRow && node != nullptr; i++) {
    //Go down each row until reaching the right row where the seat is at
    node = node -> getDown();
  }
  for(int i = 0; i < chosenSeat - 'A' && node != nullptr; i++) {
    //Go right across the columns of the current row until we find the seat we want to delete
    node = node -> getNext();
  }
  //If the seat was for an adult, decrement the order's and the auditorium's adult ticket count
  if(node -> getPayload() -> getTicketType() == 'A') {
    auditorium.totalAdultTickets -= 1;
    currentOrder -> setAdultTotalTickets(currentOrder -> getAdultTotalTickets() - 1);
  }
  //If the seat was for a child, decrement the order's and the auditorium's child ticket count
  else if(node -> getPayload() -> getTicketType() == 'C') {
    auditorium.totalChildTickets -= 1;
    currentOrder -> setChildTotalTickets(currentOrder -> getChildTotalTickets() - 1);
  }
  //If the seat was for a senior, decrement the order's and the auditorium's senior ticket count
  else if(node -> getPayload() -> getTicketType() == 'S') {
    auditorium.totalSeniorTickets -= 1;
    currentOrder -> setSeniorTotalTickets(currentOrder -> getSeniorTotalTickets() - 1);
  }
  //Finally, change the actual status of the seat itself to unreserve it 
  node -> getPayload() -> setTicketType('.');
}

//Method to view the customer's orders
void viewOrders(Customer* customer, bool displayingReceipt) {
  //If there are no orders, print that information to the console and print the customer total to be zero dollars
  if(customer -> getFirstOrder() == nullptr) {
    cout << "No orders" << endl;
    cout << "Customer Total: $0.00" << endl;
  }
  int j = 1;
  Order* currentOrder = customer -> getFirstOrder();
  //Keep track of the total adult, child, and senior tickets of all the orders combined, in order to print that information if the receipt is to be displayed
  int totalAdultTickets = 0;
  int totalChildTickets = 0;
  int totalSeniorTickets = 0;

  //Traverse all the orders
  while(currentOrder != nullptr) {
    j++;
    //Print the auditorium number of the current order first
    cout << "Auditorium " << currentOrder -> getAuditoriumNumber() << ", ";
    RowNumberNode* currentRow = currentOrder -> getFirstRowNumber();
    //Go through each RowNumberNode of the current order
    while(currentRow != nullptr) {
      int printedCounter = 0;
      //Access the seating section the RowNumberNode maps to 
      Section* currentSection = currentOrder -> getSectionMap().at(currentRow);
      for(int i = 0; i < 26; i++) {
        //For each element in the seats array of the current seating section, if that element is initialized to an alphabet character, the seat is part of the section. So print that seat
        if('A' <= currentSection -> getSeats()[i] && currentSection -> getSeats()[i] <= 'Z') {
          cout << currentRow -> getRowNumber();
          cout << currentSection -> getSeats()[i];
          printedCounter++;
          if(currentRow -> getNext() != nullptr || printedCounter < currentSection -> getTotalSectionTickets()) {
             cout << ",";
          }
        }
      }
      currentRow = currentRow -> getNext();
    }
    //Print the number of adult, child, and senior tickets for each order
    cout << endl << currentOrder -> getAdultTotalTickets() << " adult, ";
    cout << currentOrder -> getChildTotalTickets() << " child, ";
    cout << currentOrder -> getSeniorTotalTickets() << " senior" << endl;
    if(displayingReceipt) {
      //If the receipt is to be displayed, also print the total cost for paying for all those tickets in that order
      cout << "Order Total: $";
      cout << fixed << setprecision(2) << (currentOrder -> getAdultTotalTickets() * 10) + (currentOrder -> getChildTotalTickets() * 5) + (currentOrder -> getSeniorTotalTickets() * 7.5) << endl;
    }
    
    cout << endl;
    //Add the current order's tickets of each type to the total number of tickets for each type 
    totalAdultTickets += currentOrder -> getAdultTotalTickets();
    totalChildTickets += currentOrder -> getChildTotalTickets();
    totalSeniorTickets += currentOrder -> getSeniorTotalTickets();

    currentOrder = currentOrder -> getNext();
  }
  if(displayingReceipt) {
    //If the receipt is to be displayed, print the cost of all the tickets for all the orders combined
    cout << "Customer Total: $";
    cout << fixed << setprecision(2) << (totalAdultTickets * 10) + (totalChildTickets * 5) + (totalSeniorTickets * 7.5) << endl;
    cout << endl;
  }
}

//Method to gather user input to reserve seats in an auditorium 
bool gatherInput(bool updatingOrder, Order* order, Auditorium& auditorium, Customer* currentCustomer, int auditoriumNumber) {
  //Prompt for rowm, keep prompting until valid input given
  string chosenRow;
  cout << "Enter the row that you would like: ";
  while(true) {
      cin >> chosenRow;
      if(inputIsValid(chosenRow)) {
          break;
      }
      else {
          cout << "Please enter valid input" << endl;
      }
  }
  cout << endl;
  //Ask the user for the desired column, keep asking until they give valid input
  cout << "Enter the seat that you would like: ";
  string chosenSeat;
  while(true) {
      cin >> chosenSeat;
      if(chosenSeat.size() == 1 && chosenSeat.at(0) >= 'A' && chosenSeat.at(0) <= 'Z') {
          break;
      }
      else {
          cout << "Please enter a valid input." << endl;
      }
  }
  cout << endl;
  //Keep asking the user for the number of adult tickets until they give a valid input
  cout << "Enter the number of adult tickets: ";
  string adultTickets;
  while(true) {
      cin >> adultTickets;
      if(inputIsValid(adultTickets)) {
          break;
      }
      else {
          cout << "Please enter a valid input." << endl;
      }
  }
  cout << endl;
  //Keep asking the user for the number of child tickets until they give a valid input
  cout << "Enter the number of child tickets: ";
  string childTickets;
  while(true) {
      cin >> childTickets;
      if(inputIsValid(childTickets)) {
          break;
      }
      else {
          cout << "Please enter a valid input." << endl;
      }
  }
  cout << endl;
  //Keep asking the user for the number of senior tickets until they give a valid input
  cout << "Enter the number of senior tickets: ";
  string seniorTickets;
  while(true) {
      cin >> seniorTickets;
      if(inputIsValid(seniorTickets)) {
          break;
      }
      else {
          cout << "Please enter a valid input." << endl;
      }
  }
  if(stoi(seniorTickets) == 0 && stoi(childTickets) == 0 && stoi(adultTickets) == 0) {
    //If the user didn't actually want to buy any tickets, don't count this as an order at all 
    return false;
  }
  cout << endl;

  if(reserveSeats(updatingOrder, order, auditorium, auditoriumNumber, currentCustomer, stoi(chosenRow),  chosenSeat.at(0), stoi(adultTickets), stoi(childTickets), stoi(seniorTickets))) {
      //If we were able to reserve seats for the user or find an alternate section, update the total ticket counters for the auditorium for each type of person
      auditorium.totalAdultTickets += stoi(adultTickets);
      auditorium.totalChildTickets += stoi(childTickets);
      auditorium.totalSeniorTickets += stoi(seniorTickets);
      return true;
  }
  return false;
}

bool inputIsValid(string input) {//Method to check that the user input is an actual number
    for(int i = 0; i < (int)input.length(); i++) {
        if(!isdigit(input.at(i))) {
            return false;
        }
    }
    return true;
}
//Method to reserve seats in an auditorium 
bool reserveSeats(bool updatingOrder, Order* order, Auditorium &auditorium, int auditoriumNumber, Customer* currentCustomer, int chosenRow, char chosenSeat, int adultTickets, int childTickets, int seniorTickets) {
    int totalTickets = adultTickets + childTickets + seniorTickets;
    Node* first = auditorium.first;

    Node* headTraverser = first;
    int rowCounter = 1;
    //go down to the row that the user wants to reserve seats in
    while(headTraverser != nullptr && rowCounter < chosenRow) {
        headTraverser = headTraverser -> getDown();
        rowCounter++;
    }

    Node* current = headTraverser;
    //Find the column that the user wants the starting seat to be in as a number
    int chosenColumn = chosenSeat - 'A';
    //go to the starting seat that the user wants to reserve
    while(current != nullptr && chosenColumn > 0) {
        current = current -> getNext();
        chosenColumn--;
    }
    //Scout the seats to the right of the starting seats, keep scouting until either we reach the end of the row, we found enough unreserved seats, or we hit a reserved seat
    Node* scoutNode = current;
    while(scoutNode != nullptr && scoutNode -> getPayload() -> getTicketType() == '.' && totalTickets > 0) {
        scoutNode = scoutNode -> getNext();
        totalTickets--;
    }
    //If we hit the end of the row or we hit a reserved seats, we cannot reserve the seats that the user wants. So we call the bestAvailable() function to try to find an alternate section that the user can reserve
    if(totalTickets > 0) {
      if(!updatingOrder) {
        return bestAvailable(auditorium, currentCustomer, auditoriumNumber, auditorium.totalRows, auditorium.totalColumns, adultTickets, childTickets, seniorTickets);
      }
      else  {
        cout << "no seats available" << endl;
        return false;
      }
    }
    else {
        //If the desired section the user wants is available, reserve the seats for them

        //Reserve the adult seats first
        int tempAdults = adultTickets;
        while(tempAdults > 0) {
            current -> getPayload() -> setTicketType('A');
            tempAdults--;
            current = current -> getNext();

        }

        //Reserve the child seats next
        int tempChilds = childTickets;
        while(tempChilds > 0) {
            current -> getPayload() -> setTicketType('C');
            tempChilds--;
            current = current -> getNext();
        }

        //Reserve the senior seats last
        int tempSeniors = seniorTickets;
        while(tempSeniors > 0) {
            current -> getPayload() -> setTicketType('S');
            tempSeniors--;
            current = current -> getNext();
        }
        if(!updatingOrder) {
          //If the customer isn't trying to add tickets to a previous order, count this reservation as a new order and add it to the customer's list of orders
          Order* newOrder = new Order(auditoriumNumber, adultTickets, childTickets, seniorTickets, chosenRow, chosenSeat);
          currentCustomer -> addOrder(newOrder);
        }
        else {
          //Otherwise, create a new section and add this section to the current order that the user is trying to add the tickets to 
          order -> addSection(chosenRow, chosenSeat, adultTickets + childTickets + seniorTickets, adultTickets, childTickets, seniorTickets);
        }
        cout << "Seats successfully reserved!" << endl;
        return true;
    }
}

double calculateDistance(double x1, double y1, double x2, double y2) {
    //Calculate the distance between using two points using the pythagorean theorem 
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

bool bestAvailable(Auditorium& auditorium, Customer* currentCustomer, int auditoriumNumber, int totalRows, int totalColumns, int adultTickets, int childTickets, int seniorTickets) {
    Node* first = auditorium.first;
    //Calcualte the middle of the rows of the auditorium
    double middleOfRows = 0;
    if(totalRows % 2 == 0) {
        middleOfRows = totalRows / 2 + 0.5;
    }
    else {
        middleOfRows = totalRows / 2 + 1;
    }
    //Calcualte the middle of the columns of the auditorium
    double middleOfColumns = 0;
    if(totalColumns % 2 == 0) {
        middleOfColumns = totalColumns / 2 + 0.5;
    }
    else {
        middleOfColumns = totalColumns / 2 + 1;
    }
    //Now we have the x and y coordinates of the exact middle of the auditorium
    //Keep track of the least distance between an available section and the middle of the auditorium. The section with that least distance will be given as an option to the user
    double leastDistance = INT_MAX;
    double smallestRowDistance = INT_MAX;
    int headTraverserTracker = 0;
    int traverserTracker = 0;
    bool foundAtLeastOneSection = false;

    Node* headTraverser = first;
    Node* reserver = headTraverser;
    Node* scoutTracker = first;
    int totalTickets = adultTickets + childTickets + seniorTickets;
    //start searching the available sections
    while(headTraverser != nullptr) {
        Node *traverser = headTraverser;
        //T4raverser through each row
        while(traverser != nullptr) {
            //Keep traversing until we find a starting seat that is not already reserved
            while(traverser -> getNext() != nullptr && traverser -> getPayload() -> getTicketType() != '.') {
                traverser = traverser -> getNext();
            }

            Node *scout = traverser;
            int tempTotal = totalTickets - 1;

            while(scout -> getNext() != nullptr && scout -> getPayload() -> getTicketType() == '.' && tempTotal > 0) {
                //Send a scout node that keeps moving to the right, trying to find an available section with enough seats for the user and without any already reserved seats
                tempTotal--;
                scout = scout -> getNext();
            }

            if(tempTotal == 0 && scout -> getPayload() -> getTicketType() == '.') {

                //We have found an available section
                foundAtLeastOneSection = true;    
                //Update foundAtLeastOneSection to true so that we know that we found at least one section

                //Find the middle column position of the available section

                double columnMiddlePosition = static_cast<double>((traverser -> getPayload() -> getSeat() - 'A' + 1) + (scout -> getPayload() -> getSeat() - 'A' + 1)) / 2.0;


                //The available section's row is just the same as the row that we are looking at
                double rowPosition = traverser -> getPayload() -> getRow();
                //Calculate the distance between the middle of the available section and the middle of the auditorium
                double distance = calculateDistance(rowPosition, columnMiddlePosition, middleOfRows, middleOfColumns);
                //If the current distance between the middle of the available section we are looking at and the middle of the auditorium is the same as the least distance between the middle of any available section and middle of auditorium, 
                //we check which available section's row is closest to the middle row of the auditorium. If the row of the available section we are looking at is closer, then set the smallest row distance equal to the row distance between the row of the middle of the 
                //current available section and the middle of the auditorium.
                //We also keep track of the starting seat of this available section so that if this ends up having the closest middle to the middle of the auditorium, we reserve seats starting from this available seats
                if(distance == leastDistance) {
                    double currentRowDistance = abs(traverser -> getPayload() -> getRow() - middleOfRows);
                    if(currentRowDistance < smallestRowDistance) {
                        smallestRowDistance = currentRowDistance; 
                        headTraverserTracker = headTraverser -> getPayload() -> getRow();
                        traverserTracker = traverser -> getPayload() -> getSeat();
                        reserver = traverser;
                        //Keep track of where the scout node of the closes available section is, and use its seat letter to tell the user the final seat of the available section that they can reserve
                        scoutTracker = scout;
                    }
                }
                //If this distance is less than the least distance between the middle of an available section and the auditorium that we have found so far, we set the least distance tracker to be eqal to this distance
                //We also keep track of the starting seat of this available section so that if this ends up having the closest middle to the middle of the auditorium, we reserve seats starting from this available seats
                else if(distance < leastDistance) {
                    smallestRowDistance = abs(traverser -> getPayload() -> getRow() - middleOfRows);
                    leastDistance = distance;
                    headTraverserTracker = headTraverser -> getPayload() -> getRow();
                    traverserTracker = traverser -> getPayload() -> getSeat();
                    reserver = traverser;
                    //Keep track of where the scout node of the closes available section is, and use its seat letter to tell the user the final seat of the available section that they can reserve
                    scoutTracker = scout;
                }
            }
            traverser = traverser -> getNext();
            //Move the traverser to the right by one to start the process again of checking for an available section
        }
        headTraverser = headTraverser -> getDown();
        //One we are done with one row, move to the row below and starting looking for available sections on that row
    }

    if(!foundAtLeastOneSection) {
        //We did not find any available sections because foundAtLeastOneSection was never set to true
        cout << "no seats available" << endl;
        return false;
    }
    else {
        //We found an available section
        //Output where the available section is
        cout << "Could not reserve your desired seats, but found an alternate section at ";
        cout << headTraverserTracker << char(traverserTracker) << "-" << headTraverserTracker << scoutTracker -> getPayload() -> getSeat() << endl;
        //Ask the user if they want to reserve this available section, keep asking until they enter valid input
        cout << "Would you like to reserve these seats? (type Y/N): " << endl;
        string choice;
        while(true) {
            cin >> choice;
            if(choice == "N" || choice == "n") {
                //User does not want to reserve the available section, so we don't
                cout << "Ok! no problem." << endl;
                break;
            }
            else if(choice == "Y" || choice == "y") {
                //User wants to reserve the available section
                int tempAdults = adultTickets;
                //Update the total number of adult tickets in the auditorium since the user may want to reserve some
                //auditorium.totalAdultTickets += tempAdults;
                //Reserve all the adult tickets in the avaiable section first 
                while(reserver != nullptr && tempAdults > 0) {
                    reserver -> getPayload() -> setTicketType('A');
                    tempAdults--;
                    reserver = reserver -> getNext();
                }

                int tempChilds = childTickets;
                //Update the total number of child tickets in the auditorium since the user may want to reserve some
                //auditorium.totalChildTickets += tempChilds;
                //Reserve all the child tickets in the available section next
                while(reserver != nullptr && tempChilds > 0) {
                    reserver -> getPayload() -> setTicketType('C');
                    tempChilds--;
                    reserver = reserver -> getNext();
                }

                int tempSeniors = seniorTickets;
                //Update the total number of senior tickets in the auditorium since the user may want to reserve some 
                //auditorium.totalSeniorTickets += seniorTickets;
                //Reserve all the senior tickets in the available section last
                while(reserver != nullptr && tempSeniors > 0) {
                    reserver -> getPayload() -> setTicketType('S');
                    tempSeniors--;
                    reserver = reserver -> getNext();
                }
                
                Order* newOrder = new Order(auditoriumNumber, adultTickets, childTickets, seniorTickets, headTraverserTracker, char(traverserTracker));
                currentCustomer -> addOrder(newOrder);
                cout << "reached here" << endl;
                cout << "Seat(s) successfully reserved!" << endl;
                return true;
            }
            else {
                //Ask for valid input again if the user does not give it
                cout << "Please enter a valid choice (either Y/N)" << endl;
            }
        }
    }
    return false;
}

void displayAuditorium(Node* first, int totalColumns) {
    //Print the contents of the auditorium, showing all the reserved seats as '#'
    int i;
    cout << "   ";
    //First print the letters of the columns of the auditorium, so that the user can see them and select the one they want
    for(i = 0; i < totalColumns; i++) {
        cout << char(i + 'A');
    }
    cout << endl;

    Node *headTraverser = first;
    while(headTraverser != nullptr) {
      cout << headTraverser -> getPayload() -> getRow() << ". ";
      Node *traverser = headTraverser;
      while(traverser != nullptr) {
          //For each row, print the ticket type of each seat in that row ('#' if the seat is reserved, '.' if it's not)
          char ticket_type = traverser -> getPayload() -> getTicketType();
          if(ticket_type != '.') {
            cout << '#';
          }
          else {
            cout << '.';
          }
          traverser = traverser -> getNext();
      }
      cout << endl;
      //Move on the to next row
      headTraverser = headTraverser -> getDown();
    }
}
