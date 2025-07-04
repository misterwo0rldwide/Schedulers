#include "CircList.h"

template <class T>
CircList<T>::CircList(T value) {
    if (value == nullptr) {
        this->head = this->last = nullptr;
    } else {
        // Create the first node
        this->head = this->last = new Node<T>(value);
        
        // Make it circular - node points to itself
        this->head->setNext(this->head);
        this->head->setPrev(this->head);
    }
}

template <class T>
CircList<T>::~CircList() {
    if (!this->head) return;
    
    // Break the circle first
    this->last->setNext(nullptr);
    
    // Delete all nodes
    Node<T>* current = this->head;
    while (current) {
        Node<T>* next = current->getNext();
        delete current;
        current = next;
    }
}

template <class T>
Node<T>* CircList<T>::getHead(void) const noexcept { 
    return this->head; 
}

template <class T>
void CircList<T>::insert(T value) { 
    Node<T>* node = new Node<T>(value);
    
    if (!this->head) {
        // First node
        this->head = this->last = node;
        node->setNext(node);
        node->setPrev(node);
    } else {
        // Insert at the end (after last)
        node->setNext(this->head);
        node->setPrev(this->last);
        this->last->setNext(node);
        this->head->setPrev(node);
        this->last = node;
    }
}

template <class T>
Node<T>* CircList<T>::search(T value) const {
    if (!this->head) return nullptr;
    
    Node<T>* tmp = this->head;
    do {
        if (tmp->getValue() == value)
            return tmp;
        tmp = tmp->getNext();
    } while(tmp != this->head);

    return nullptr;
}

template <class T>
void CircList<T>::remove(Node<T>* node) {
    if (!node || !this->head) return;
    
    // Only one node in the list
    if (node->getNext() == node) {
        this->head = this->last = nullptr;
    } else {
        // Remove node from circular structure
        node->getPrev()->setNext(node->getNext());
        node->getNext()->setPrev(node->getPrev());
        
        // Update head/last pointers if necessary
        if (node == this->head) {
            this->head = node->getNext();
        }
        if (node == this->last) {
            this->last = node->getPrev();
        }
    }
    
    delete node;
}

template <class T>
void CircList<T>::remove(T value) {
    Node<T>* node = this->search(value);
    
    if (node)
        this->remove(node);
}

template class CircList<Task *>;