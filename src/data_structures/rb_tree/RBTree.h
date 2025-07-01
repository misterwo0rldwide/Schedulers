#pragma once

#include <functional>
#include <mutex>

enum class Color: char {RED = 0, BLACK = 1};

template <class T>
class RBNode {
private:
    T value;
    Color color;
public:
    RBNode<T>* left;
    RBNode<T>* right;
    RBNode<T>* parent;

    RBNode(T value, RBNode<T>* left, RBNode<T>* right, RBNode<T>* parent, Color color): 
        value(value), left(left), right(right), parent(parent), color(color) {}
    ~RBNode() = default;

    // Getters
    T getValue(void) const noexcept { return value; }
    Color getColor(void) const noexcept { return color; }

    // Setters
    void setValue(T value) { this->value = value; }
    void setColor(Color color) { this->color = color; }
};

template <class T, class Compare = std::less<T>>
class RBTree {
private:
    RBNode<T>* root;
    RBNode<T>* leftMost;
    size_t total_weight;
    Compare comp;

    mutable std::mutex mtx;

public:
    RBTree() : root(nullptr), leftMost(nullptr), total_weight(0) {}
    RBTree(const Compare& c) : root(nullptr), leftMost(nullptr), total_weight(0), comp(c) {}
    ~RBTree() { this->clear(); }

    RBNode<T>* getLeftMost(void) const noexcept { 
        std::lock_guard<std::mutex> lock(mtx);
        return this->leftMost; 
    }
    size_t getTotalWeight(void) const noexcept { 
        std::lock_guard<std::mutex> lock(mtx);
        return this->total_weight; 
    }

    void clear(void);
    bool remove(const T& value);
    void remove(RBNode<T>* node);
    void insert(const T& value);
    RBNode<T>* search(const T& value) const;

private:
    RBNode<T>* getRoot(void) const noexcept { return this->root; }
    void setRoot(RBNode<T>* root) { this->root = root; }
    void setLeftMost(RBNode<T>* leftMost) { this->leftMost = leftMost; }

    void leftRotate(RBNode<T>* node);
    void rightRotate(RBNode<T>* node);
    void insert_fixup(RBNode<T>* node);
    void transplant(RBNode<T>* u, RBNode<T>* v);
    RBNode<T>* minimum(RBNode<T>* node) const;
    void remove_fixup(RBNode<T>* node);
};
