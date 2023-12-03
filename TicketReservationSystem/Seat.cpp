// Joel Gurivireddy
// jxg220051

#include "Seat.h"
#include <iostream>
//If no information given to create a new Seat object, some default values are given
Seat::Seat() {
    row = 1;
    seat = 'A';
    ticketType = '.';
}

//Create nerw seat object with given information 
Seat::Seat(int newRow, char newSeat, char newTicketType) {
    row = newRow;
    seat = newSeat;
    ticketType = newTicketType;
}
//get information about the seat
int Seat::getRow() {
    return row;
}
char Seat::getSeat() {
    return seat;
}
char Seat::getTicketType() {
    return ticketType;
}

//change information about the seat
void Seat::setRow(int newRow) {
    row = newRow;
}
void Seat::setSeat(char newSeat) {
    seat = newSeat;
}
void Seat::setTicketType(char newTicketType) {
    ticketType = newTicketType;
}

//print out the ticket type of the current seat
char Seat::operator<<(Seat* currentSeat) {
    return currentSeat -> ticketType;
}
