// Joel Gurivireddy 
//jxg220051

#ifndef PLATINUM_H
#define PLATINUM_H

#include "Customer.h"

//Platinum class, derived from Customer class
class Platinum : public Customer {
  private:
    // Extra data member bonusBucks for platinum customers
    int bonusBucks;

  public:
    Platinum() {bonusBucks = 0;}
    Platinum(std::string firstName, std::string lastName, std::string guestID, double amountSpent, int bonusBucks) : Customer(firstName, lastName, guestID, amountSpent) {this -> bonusBucks = bonusBucks;}
    ~Platinum() {}

    int getBonusBucks() {return bonusBucks;}

    void setBonusBucks(int bonusBucks) {this -> bonusBucks = bonusBucks;}

};

#endif