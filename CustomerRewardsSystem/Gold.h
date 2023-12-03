// Joel Gurivireddy 
//jxg220051

#ifndef GOLD_H
#define GOLD_H

#include "Customer.h"

//Gold class, derived from Customer class
class Gold : public Customer {
  private: 
    // Extra data member to hold current Gold customer's discount 
    double discountPercentage;

  public:
    Gold() {discountPercentage = 0;}
    Gold(std::string firstName, std::string lastName, std::string guestID, double amountSpent, double discount) : Customer(firstName, lastName, guestID, amountSpent) {this -> discountPercentage = discount;}
    ~Gold() {}

    double getDiscountPercentage() {return discountPercentage;}

    void setDiscountPercentage(double discount) {this -> discountPercentage = discount;}

};

#endif