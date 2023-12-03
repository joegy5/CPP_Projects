//Joel Gurivireddy
//jxg220051
#ifndef DVD_H
#define DVD_H

#include <iostream>

class DVD {
  private:
    std::string title = "";
    int available = 0;
    int rented = 0;

  public:
    DVD(std::string, int, int);

    //Accessors to access the DVD's title, number of available copies, and number of rented copies 
    std::string getTitle() const;
    int getAvailable() const;
    int getRented() const;
    //Mutators to change the DVD's title, number of available copies, and number of rented copies 
    void setTitle(std::string);
    void setAvailable(int);
    void setRented(int);

    //Overloaded insertion operator
    friend std::ostream& operator<<(std::ostream&, const DVD*);

    //Overloaded comparison operators
    bool operator>(const DVD&);
    bool operator<(const DVD&);
    bool operator==(const DVD&);
};

#endif
