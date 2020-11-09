#include <iostream>
#include <cstdlib>
#include <ctime>

#include "DoublyLinkedList.h"

int main() {
        
    srand(time(NULL));

    DoublyLinkedList myList{};
    
    for (int i = 0; i < 1'000; i++) {
        myList.append(rand() % 10);
    }

    std::cout << "Value of idx 999 is "
              << myList.at(999) << "." << std::endl;

    myList.at(999) = 100;

    std::cout << "Value of idx 999 is now "
              << myList.at(999) << "." << std::endl;

    myList.at(5'000) = 10;

    return 0;
}