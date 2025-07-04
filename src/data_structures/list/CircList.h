#pragma once

#include "../../process/task.h"

template <class T>
class Node {
    private:
        T value;
        Node<T>* next;
        Node<T>* prev;
    public:
        Node(T value, Node<T>* next = nullptr, Node<T>* prev = nullptr): value(value), next(next), prev(prev) {}
        ~Node() = default;

        void setValue(T value) noexcept { this->value = value; }
        void setNext(Node<T>* next) noexcept { this->next = next; }
        void setPrev(Node<T>* prev) noexcept { this->prev = prev; }

        T getValue(void) const noexcept { return this->value; }
        Node<T>* getNext(void) const noexcept { return this->next; }
        Node<T>* getPrev(void) const noexcept { return this->prev; }
};

template <class T>
class CircList {
    private:
        Node<T>* head;
        Node<T>* last;
    public:
        CircList(T value);
        ~CircList();

        Node<T>* getHead(void) const noexcept;

        void insert(T value);
        void remove(T value);
        void remove(Node<T>* node);
        Node<T>* search(T value) const;
};

extern template class CircList<Task *>;