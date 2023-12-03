This projects models a ticket reservation system (using hashmaps - unordered_map in C++)!

The user has to login using a username and password, and can reserve seats in 1 of three auditoriums that they choose from (the seats marked as dots are unreserved). 
  - To reserve a seat, the user goes through the reservation process, and if some seats they want are already reserved, the program tries to find the best alternative available seating arrangement in the auditorium (if there are any)

The user can also view their previous orders, update their previous orders, display their order receipt, and log out.

There is also an admin (must be logged into using "admin" as username and the correct password), who can display the report for all the auditoriums, logout, or quit the program. 

Each auditorium is comprised of a group of linked lists linked together
