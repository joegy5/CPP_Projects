// Joel Gurivireddy
// jxg220051

#ifndef AUDITORIUM_H
#define AUDITORIUM_H

#include "Node.h"
#include <iostream>

class Auditorium {
    public:
      // Have a Node pointer that represents the absolute beginning of the list
      Node *first = nullptr;
      Auditorium(std::string fileName);
      //Keep track of total number of rows and total number of columns of auditorium
      int totalRows = 0;
      int totalColumns = 0;
      //Keep track of total tickets for each type of person
      int totalAdultTickets = 0;
      int totalChildTickets = 0;
      int totalSeniorTickets = 0;
      int getOpen();
      int getReserved();
      double getSales();
};

#endif