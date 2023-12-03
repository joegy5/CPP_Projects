// Joel Gurivireddy
// jxg220051

#ifndef SEAT_H
#define SEAT_H
// Seat class, with information about the Seat's row, ticket type (either unreserved, reserved by a senior, child, or adult), and seat letter (representing the column the seat is in)

class Seat {
    private:
        int row = 0;
        char seat = '<';
        char ticketType = '.';

    public:
        Seat();
        Seat(int, char, char);

        //Get access to information about the seat (ticket type, row, or seat letter)
        int getRow();
        char getSeat();
        char getTicketType();

        //change information about the seat
        void setRow(int);
        void setSeat(char);
        void setTicketType(char);

        char operator<<(Seat*);
};

#endif
