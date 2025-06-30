#pragma once

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
        T getValue(void) const { return value; }
        Color getColor(void) const { return color; }

        // Setters
        void setValue(T value) { this->value = value; }
        void setColor(Color color) { this->color = color; }
};

template <class T>
class RBTree {
private:
    RBNode<T>* root;

public:
    RBTree() : root(nullptr) {}
    ~RBTree() { this->clear(); }

    RBNode<T>* getRoot(void) const { return this->root; }
    void setRoot(RBNode<T>* root) { this->root = root; }

    void clear(void);
    bool remove(const T& value);
    void remove(RBNode<T>* node);
    void insert(const T& value);
    RBNode<T>* search(const T& value) const;
private:
    void leftRotate(RBNode<T>* node);
    void rightRotate(RBNode<T>* node);
    void insert_fixup(RBNode<T>* node);
    void transplant(RBNode<T>* u, RBNode<T>* v);
    RBNode<T>* minimum(RBNode<T>* node) const;
    void remove_fixup(RBNode<T>* node);
};