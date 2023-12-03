// Joel Gurivireddy 
//jxg220051

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <iostream>

//Regular Customers
class Customer {
  protected: 
    std::string firstName;
    std::string lastName;
    std::string guestID;
    double amountSpent;

  public:
    Customer() { 
      firstName = "no-name"; 
      lastName = "no-name"; 
      guestID = "no-ID"; 
      amountSpent = 0;
    }

    Customer(std::string firstName, std::string lastName, std::string guestID, double amountSpent) {
      this -> firstName = firstName;
      this -> lastName = lastName;
      this -> guestID = guestID;
      this -> amountSpent = amountSpent;
    }

    virtual ~Customer() {}

    //Accessors for accessing data members
    std::string getFirstName() {return firstName;}
    std::string getLastName() {return lastName;}
    std::string getGuestID() {return guestID;}
    virtual double getAmountSpent() {return amountSpent;}

    //Mutators for changing data members 
    void setFirstName(std::string firstName) {this -> firstName = firstName;}
    void setLastName(std::string lastName) {this -> lastName = lastName;}
    void setGuestID(std::string guestID) {this -> guestID = guestID;}
    void setAmountSpent(double amountSpent) {this -> amountSpent = amountSpent;}

};


#endif