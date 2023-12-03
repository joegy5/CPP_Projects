//Joel Gurivireddy
//jxg220051
#include "DVD.h"
#include <iomanip>
#include <fstream>

DVD::DVD(std::string title, int available, int rented) {//Overloaded constructor to set the DVD's title, number of available copies, and number of copies rented
  this -> title = title;
  this -> available = available;
  this -> rented = rented;
}
//Accessors to acces information about the DVD object 
std::string DVD::getTitle() const {
  return title;
}
int DVD::getAvailable() const {
  return available;
}
int DVD::getRented() const {
  return rented;
}
//Mutators to change information about the DVD object
void DVD::setTitle(std::string title) {
  this -> title = title;
}
void DVD::setAvailable(int available) {
  this -> available = available;
}
void DVD::setRented(int rented) {
  this -> rented = rented;
}
//Overloaded insertion operator to output the DVD's contents in an organized manner
std::ostream& operator<<(std::ostream &output, const DVD* dvd) {
  //Set the max width to 30, then output the left-aligned version of the title
  output << std::setw(30) << std::left << dvd -> getTitle();
  //set the next max width to 2, then output the left-aligned version of the number of available copies
  output << std::setw(2) << std::left << dvd -> getAvailable();
  //Set the next max width to 10, the output the right-aligned version of the number of copies rented
  output << std::setw(10) << std::right << dvd -> getRented() << std::endl;
  return output;
}

bool DVD::operator<(const DVD& dvd) {
  //Overloaded comparison operator to check if this DVD's title is alphabetically smaller than the the title of the DVD we are comparing to 
  return this -> getTitle() < dvd.getTitle();
}

bool DVD::operator>(const DVD& dvd) {
  //Overloaded comparison operator to check if this DVD's title is alphabetically greater than the the title of the DVD we are comparing to 
  return this -> getTitle() > dvd.getTitle();
}

bool DVD::operator==(const DVD& dvd) {
  //Overloaded comparison operator to check if this DVD's title is equal to the title of the DVD we are comparing to 
  return this -> getTitle() == dvd.getTitle();
}



