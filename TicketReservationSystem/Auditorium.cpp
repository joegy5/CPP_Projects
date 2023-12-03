// Joel Gurivireddy
// jxg220051

#include "Auditorium.h"
#include <iostream>
#include <fstream>

Auditorium::Auditorium(std::string fileName) {
    //Open the given file, stop program if file fails to open
    std::ifstream input;
    input.open(fileName);

    if(!input.is_open()) {
        std::cout << "File stream failed to open" << std::endl;
    }
    else {
        unsigned int rowNumber = 1;
        Node* headConnector = nullptr;
        first = nullptr;

        std::string currentRow;
        //get a line of input from the file (representing the current row being processed)
        while(getline(input, currentRow) && input) {
            totalColumns = (int)currentRow.size();
            //create the first seat in the row, start linking other created seats from there
            Seat *firstPayload = new Seat(rowNumber, 'A', currentRow.at(0));

            Node *current_head = new Node(nullptr, nullptr, firstPayload);

            if(first == nullptr) {
                first = current_head;
                //In this case, first hasn't been updated yet, meaning that current_head is the very first node in the auditorium. So we set the first pointer to that node
            }

            if(headConnector != nullptr) {
                headConnector -> setDown(current_head);
                //In this case, if the headConnector node is not null, it was already created before, meaning that we are not on the first row. So we link the the headConnector to the first node of this current row using the down pointer
            }
            //After that, we set headConnector equal to current_head to update it
            headConnector = current_head;
            Node *current = current_head;

            char currentTicketType = *current << current;
            //update the trackers for the total amount of tickets for each type of person, since the auditorium may already have some seats reserved in it
            switch(currentTicketType) {
                case 'A':
                    totalAdultTickets++;
                    break;
                case 'C':
                    totalChildTickets++;
                    break;
                case 'S':
                    totalSeniorTickets++;
                    break;
            }
            //Now the row is created
            unsigned int i;
            for(i = 1; i < currentRow.size(); i++) {
                //Create each new Node in the row with a Seat payload that it contains
                Seat *payload = new Seat(rowNumber, char(i + 'A'), currentRow.at(i));
                Node *newNode = new Node(nullptr, nullptr, payload);
                //After each new node is created, link the previous node to this one using the next pointer
                current -> setNext(newNode);
                current = current -> getNext();
                char currentTicketType = current -> getPayload() -> getTicketType();
                //Once again, keep tracking and updating the total number of tickets for each type of person
                switch(currentTicketType) {
                    case 'A':
                        totalAdultTickets++;
                        break;
                    case 'C':
                        totalChildTickets++;
                        break;
                    case 'S':
                        totalSeniorTickets++;
                        break;
                }   
            } 
            rowNumber++;
            //Move on to the next row 
        }
        totalRows = rowNumber - 1;
        //Keep track of the total number of rows after the auditorium linked list is fully assembled
    }
}
int Auditorium::getOpen() {
  return totalRows * totalColumns - totalAdultTickets - totalChildTickets - totalSeniorTickets;
}
int Auditorium::getReserved() {
  return totalAdultTickets + totalChildTickets + totalSeniorTickets;
}
double Auditorium::getSales() {
  return totalAdultTickets * 10 + totalChildTickets * 5 + totalSeniorTickets * 7.5;
}