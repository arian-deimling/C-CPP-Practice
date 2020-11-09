#include <iostream>
#include <cstdint>
#include <stdexcept>

#include "DoublyLinkedList.h"

// CONSTRUCTOR
DoublyLinkedList::DoublyLinkedList() : head{nullptr}, tail{nullptr}, length{0} {

    /* constructor has an empty body */

}

// CONSTRUCTOR
DoublyLinkedList::DoublyLinkedList(const DoublyLinkedList& l, intmax_t startIdx, intmax_t len)
        : head{nullptr}, tail{nullptr}, length{0} {

    // current node; start at the head of the list provided
    Node* currNode = l.head;

    // move down the list until the node at index `startIdx` is reached
    for (size_t i = 0; i < startIdx; i++) { currNode = currNode->next; }

    // iterate through `len` nodes (or until the end of the list is
    // reached) beginning at currNode
    for (size_t i = 0; (i < len) && (currNode != nullptr); i++) {

        // add the value of the current node being copied to the new list
        DoublyLinkedList::append(currNode->value);

        // move to the next node to copy
        currNode = currNode->next;
    }
}

// DESTRUCTOR
DoublyLinkedList::~DoublyLinkedList() {

    // if list has no elements, there is no memory to free
    if (length == 0) { 

    // if the list has one element, free memory that holds it
    } else if (length == 1) {
        delete head;

    // if the list has more than one element
    } else {

        // current node; start at the node prior tail of the list
        Node* currNode = head;

        while (currNode != nullptr) {

            // store the address of the next node
            Node* nextNode = currNode->next;

            // set `next` and `prev` pointers to null
            currNode->prev = nullptr;
            currNode->next = nullptr;

            // free the memory associated with the current node
            delete currNode;

            // move to the next node
            currNode = nextNode;
        }
    }

    // set head and tail pointers to null
    head = nullptr;
    tail = nullptr;
}

int DoublyLinkedList::popFront(void) {

    // if there's nothing to pop throw an out of range error
    if (length == 0) {
        throw std::out_of_range{"list index out of range"};
    }

    // value being popped from the list
    int v;

    // if there is only one element
    if (length == 1) {

        // store the value of the only element
        v = head->value;

        // reset the list to empty state
        clear();

        // return the value
        return v;
    }

    // store the location of the head node
    Node* nodeToPop = head;

    // set the second node as the head node
    head = head->next;

    // set the `prev` pointer of the new head to null
    head->prev = nullptr;

    // remove `next` pointer from old head node
    nodeToPop->next = nullptr;

    // retrieve the value from the node and the delete it
    v = nodeToPop->value;
    delete nodeToPop;
    nodeToPop = nullptr;

    // decrement the length by one
    decrementLength();

    return v;
}

int& DoublyLinkedList::at(int idx) {

    // if the specified index does not exist
    if (idx >= length) {

        // throw an out of range error
        throw std::out_of_range{"list index out of range"};
    }

    // start at the head node
    Node* currNode = head;

    // iterate until the desired node is reached
    for (size_t i = 0; i < idx; i++) {
        currNode = currNode->next;
    }

    // return a reference to the current node's value
    return currNode->value;
}

void DoublyLinkedList::append(int i) {
    
    if (length == 0) {
        
        // create the first node of the list
        tail = new Node;
        
        // since this is also the head prev pointer is null
        tail->prev = nullptr;
        
        // for a list of len 1, head and tail are the same
        head = tail;
    
    } else {  // if the list's len > 0
        
        // create a new node after the tail
        tail->next = new Node;
        
        // set new node's prev pointer to the current tail
        tail->next->prev = tail;
        
        // set the new node as the tail of the list
        tail = tail->next;
    }
    
    // set the value of the new node
    tail->value = i;
    
    // by nature, the tail does not have a next node
    tail->next = nullptr;
    
    // increment length
    incrementLength();
}

void DoublyLinkedList::prepend(int i) {

    if (length == 0) {

        // create the first node of the list
        head = new Node;

        // since this node is also the tail, next ptr is null
        head->next = nullptr;

        // for a list of len 1, head and tail are the same
        tail = head;
    
    } else {  // if the list length > 0

        // create a new node before the head
        head->prev = new Node;

        // link the new node to the current head
        head->prev->next = head;

        // set the new node as the head of the list
        head = head->prev;
    }

    // set the value of the new node
    head->value = i;

    // by nature the head does not have a prev node
    head->prev = nullptr;

    // increment length
    incrementLength();
}

void DoublyLinkedList::concatenate(const DoublyLinkedList& rightList) {

    // start at the head of the list supplied in the function call
    Node* currNode = rightList.head;

    // iterate through each node in the supplied List
    while (currNode != nullptr) {

        // create a new node in the calling List with the value of currNode
        // from the supplied List
        append(currNode->value);

        // move to the next mode in the supplied List
        currNode = currNode->next;
    }
}

void DoublyLinkedList::clear(void) {

    // if list has no elements, there is no memory to free
    if (length == 0) { 

    // if the list has one element, free memory that holds it
    } else if (length == 1) {
        delete head;

    // if the list has more than one element
    } else {

        // current node; start at the first node of the list
        Node* currNode = head;

        while (currNode != nullptr) {

            // store the address of the next node
            Node* nextNode = currNode->next;

            // set `next` and `prev` pointers to null
            currNode->prev = nullptr;
            currNode->next = nullptr;

            // free the memory associated with the current node
            delete currNode;

            // move to the next node
            currNode = nextNode;
        }
    }

    // set head and tail pointers to null
    head = nullptr;
    tail = nullptr;

    // reset length to 0
    length = 0;
}

void DoublyLinkedList::bubbleSort(void) {

    // holder for the current working node
    Node* currNode;

    // remains true if the list is sorted; otherwise, changed to false
    bool sorted = false;

    while (!sorted) {

        // if no values are swapped after an iteration through the list, it is sorted
        sorted = true;

        // start at the head of the list
        currNode = head;

        while (currNode->next != nullptr) {

            // if the current and next value are out of order
            if (currNode->value > currNode->next->value) {

                // swap the values of the current and next node
                swapValues(currNode, currNode->next);

                // set sorted to false
                sorted = false;
            }

            // move to the next node
            currNode = currNode->next;
        }
    }
}

void DoublyLinkedList::mergeSort(void) {

    // if there is nothing to sort, do nothing
    if (length < 2) {
        return;
    }
    
    // copy the List into a right and left half
    DoublyLinkedList right{*this, 0, length / 2};
    DoublyLinkedList left{*this, length / 2, length - (length / 2)};

    // clear all of the values from the calling List
    clear();

    // call merge sort on each half of the list
    right.mergeSort();
    left.mergeSort();

    // while both the right and left list have elements
    while (right.getLength() && left.getLength()) {

        // pops values from right and left into the calling List
        // in sorted order
        if (right.at(0) <= left.at(0)) {
            append(right.popFront());
        } else {
            append(left.popFront());
        }
    }

    // when one of the right or left list has been emptied, the
    // other can just be concatenated to the end of the calling List
    if (right.getLength()) {
        concatenate(right);
    } else {
        concatenate(left);
    }
}

void DoublyLinkedList::swapValues(Node* a, Node* b) {

    // temporarily hold the value of a node
    int temp = a->value;

    // put the value of b into a
    a->value = b->value;

    // put the old value of a into b
    b->value = temp;
}

void DoublyLinkedList::incrementLength(void) {

    // if the value of length will overflow upon being incremented
    if (length == UINTMAX_MAX) {

        // throw an error
        throw std::overflow_error{"DoublyLinkedList length exceeded "
                                  "max"};

    } else {

        // increment length
        length++;
    }
}

void DoublyLinkedList::decrementLength(void) {

    // if the list already has no elements
    if (length == 0) {

        // throw and error
        throw std::underflow_error{"DoublyLinkedList length cannot be"
                                   "shortened below zero"};
    
    } else {

        // decrement length
        length--;
    }

}

std::ostream& operator<<(std::ostream& outStream, DoublyLinkedList& linkedList) {

    // return the outStream with no values if the list length is equal to 0
    if (linkedList.length == 0) { return outStream; }
    
    Node* currNode = linkedList.head;

    // iterate through each value in the list except the last one
    for (int i = 0; i < linkedList.length - 1; i++) {
        
        // send the value to the output stream with a trailing comma
        outStream << currNode->value << ", " << std::flush;
        
        // move to the next node in the list
        currNode = currNode->next;

        // print 25 values per line
        if (((i + 1) % 25) == 0) {
            outStream << std::endl;
        }
    }

    // send the final value to the output stream without a trailing comma
    outStream << currNode->value;
    
    return outStream;
}