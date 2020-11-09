#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <iostream>
#include <cstdlib>
#include <cstdint>

// Linked Lists are made up of nodes connected by pointers
struct Node { int value; Node* prev; Node* next; };

// a Doubly-Linked List is an expandable data type that uses dynamic
// memory allocation and pointers to store an abstract number of values;
// the `Doubly` part of the name refers to the fact that each node in
// the list points to both the previous and next node/element in the list
class DoublyLinkedList {

    // friend function to put the list to an output stream
    friend std::ostream& operator<<(std::ostream&, DoublyLinkedList&);

    // delete some special member functions so the compiler does not
    // create default versions of them
    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;
    DoublyLinkedList(DoublyLinkedList&&) = delete;
    DoublyLinkedList& operator=(DoublyLinkedList&&) = delete;

public:

    // constructor method for a empty list;
    // initializes head and tail node pointers to null, and length to 0
    DoublyLinkedList(void);

    // constructor method to make a copy of a list;
    // must be given a List object, an index number at which to start,and
    // a number of nodes to copy (i.e. length)
    DoublyLinkedList(const DoublyLinkedList& l, intmax_t startIdx, intmax_t len);

    // destructor method frees the memory given to each node in the list
    ~DoublyLinkedList();

    // return the length of the list
    int getLength(void) const { return length; };

    // remove the first element from the List are return its value
    int popFront(void);

    // return a reference to the value stored at the specified index
    int& at(int idx);

    // add the supplied value to the end of the list; increment length by one
    void append(int i);

    // add the supplied value to the beginning of the list; increment length by one
    void prepend(int i);

    // add all of the elements of the supplied List to the end of the calling List;
    // this function creates a copy of each of the nodes of the supplied List
    void concatenate(const DoublyLinkedList&);

    // delete all of the elements from the List and reset the length to 0
    void clear(void);

    // sort the values in the array using bubble sort method
    // see: https://en.wikipedia.org/wiki/Bubble_sort
    void bubbleSort(void);

    // sort the values in the array using merge sort method
    // see: https://en.wikipedia.org/Merge_sort
    void mergeSort(void);

private:

    // swap the values stored in two nodes
    void swapValues(Node* a, Node* b);

    // increment length by 1
    void incrementLength(void);

    // decrement (decrease) length by 1
    void decrementLength(void);

    // a pointer to the first node in the list
    Node* head;

    // a pointer to the last node in the list
    Node* tail;

    // the length of the list in number of values/nodes
    intmax_t length;
};

std::ostream& operator<<(std::ostream& outStream, DoublyLinkedList& linkedList);

#endif