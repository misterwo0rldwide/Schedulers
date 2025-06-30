#include "RBTree.h"

template <class T>
void deleteRBTree(RBNode<T>* node) {
    if (!node)
        return;
    
    deleteRBTree(node->left);
    deleteRBTree(node->right);
    
    delete node;
}

template <class T>
void RBTree<T>::clear(void) {
    deleteRBTree(this->getRoot());
    this->setRoot(nullptr);
}

template <class T>
void RBTree<T>::leftRotate(RBNode<T>* node) {
    RBNode<T>* right = node->right;
    node->right = right->left;

    if (right->left)
        right->left->parent = node;
    
    right->parent = node->parent;
    if (!node->parent)
        this->root = right;
    else if (node == node->parent->left)
        node->parent->left = right;
    else
        node->parent->right = right;
    
    right->left = node;
    node->parent = right;
}

template <class T>
void RBTree<T>::rightRotate(RBNode<T>* node) {
    RBNode<T>* left = node->left;
    node->left = left->right;

    if (left->right)
        left->right->parent = node;
    
    left->parent = node->parent;
    if (!node->parent)
        this->root = left;
    else if (node == node->parent->right)
        node->parent->right = left;
    else
        node->parent->left = left;
    
    left->right = node;
    node->parent = left;
}

template <class T>
void RBTree<T>::insert_fixup(RBNode<T>* node) {    
    while (node != this->root && node->parent && node->parent->getColor() == Color::RED) {
        if (node->parent == node->parent->parent->left) {
            RBNode<T>* uncle = node->parent->parent->right;

            /* --------- case 1 --------- */
            if (uncle && uncle->getColor() == Color::RED) {
                node->parent->setColor(Color::BLACK);
                uncle->setColor(Color::BLACK);
                node->parent->parent->setColor(Color::RED);
                node = node->parent->parent;
            } else {
                /* --------- case 2 --------- */
                if (node == node->parent->right) {
                    node = node->parent;
                    leftRotate(node);
                }
                /* --------- case 3 --------- */
                node->parent->setColor(Color::BLACK);
                node->parent->parent->setColor(Color::RED);
                rightRotate(node->parent->parent);
            }
        } else {
            RBNode<T>* uncle = node->parent->parent->left;

            /* --------- case 1 -- mirror --------- */
            if (uncle && uncle->getColor() == Color::RED) {
                node->parent->setColor(Color::BLACK);
                uncle->setColor(Color::BLACK);
                node->parent->parent->setColor(Color::RED);
                node = node->parent->parent;
            } else {
                /* --------- case 2 -- mirror --------- */
                if (node == node->parent->left) {
                    node = node->parent;
                    rightRotate(node);
                }
                /* --------- case 3 -- mirror --------- */
                node->parent->setColor(Color::BLACK);
                node->parent->parent->setColor(Color::RED);
                leftRotate(node->parent->parent);
            }
        }
    }

    this->root->setColor(Color::BLACK);
}

template <class T>
void RBTree<T>::insert(const T& value) {
    RBNode<T>* tmp = nullptr;
    RBNode<T>* node = this->getRoot();

    while (node) {
        tmp = node;
        if (value < node->getValue())
            node = node->left;
        else
            node = node->right;
    }

    RBNode<T>* newNode = new RBNode<T>(value, nullptr, nullptr, tmp, Color::RED);
    if (!tmp)
        this->setRoot(newNode);
    else if (value < tmp->getValue())
        tmp->left = newNode;
    else
        tmp->right = newNode;
    
    this->insert_fixup(newNode);
}

template <class T>
RBNode<T>* RBTree<T>::search(const T& value) const {
    RBNode<T>* tmp = this->getRoot();

    // RBTree is also a binary tree so we would search like a normal binary tree
    while (tmp) {
        if (value > tmp->getValue())
            tmp = tmp->right;
        else if (value < tmp->getValue())
            tmp = tmp->left;
        else
            break;
    }

    return tmp;
}

template <class T>
void RBTree<T>::transplant(RBNode<T>* u, RBNode<T>* v) {
    if (u->parent == nullptr)
        this->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    
    // FIX: Check if v is not null before setting parent
    if (v)
        v->parent = u->parent;
}

template <class T>
RBNode<T>* RBTree<T>::minimum(RBNode<T>* node) const {
    while (node->left)
        node = node->left;
    
    return node;
}

template <class T>
void RBTree<T>::remove_fixup(RBNode<T>* node) {
    RBNode<T>* tmp;
    
    while (node != this->root && node && node->getColor() == Color::BLACK) {
        if (node == node->parent->left) {
            tmp = node->parent->right;

            if (tmp && tmp->getColor() == Color::RED) {
                tmp->setColor(Color::BLACK);
                node->parent->setColor(Color::RED);
                this->leftRotate(node->parent);
                tmp = node->parent->right;
            }

            if (tmp && 
                (!tmp->left || tmp->left->getColor() == Color::BLACK) && 
                (!tmp->right || tmp->right->getColor() == Color::BLACK)) {
                tmp->setColor(Color::RED);
                node = node->parent;
            }
            else if (tmp) {
                if (!tmp->right || tmp->right->getColor() == Color::BLACK) {
                    if (tmp->left)
                        tmp->left->setColor(Color::BLACK);
                    tmp->setColor(Color::RED);
                    this->rightRotate(tmp);
                    tmp = node->parent->right;
                }

                if (tmp) {
                    tmp->setColor(node->parent->getColor());
                    node->parent->setColor(Color::BLACK);
                    if (tmp->right)
                        tmp->right->setColor(Color::BLACK);
                    this->leftRotate(node->parent);
                    node = this->root;
                }
            }
        }
        else {
            tmp = node->parent->left;

            if (tmp && tmp->getColor() == Color::RED) {
                tmp->setColor(Color::BLACK);
                node->parent->setColor(Color::RED);
                this->rightRotate(node->parent);
                tmp = node->parent->left;
            }

            if (tmp && 
                (!tmp->right || tmp->right->getColor() == Color::BLACK) && 
                (!tmp->left || tmp->left->getColor() == Color::BLACK)) {
                tmp->setColor(Color::RED);
                node = node->parent;
            }
            else if (tmp) {
                if (!tmp->left || tmp->left->getColor() == Color::BLACK) {
                    if (tmp->right)
                        tmp->right->setColor(Color::BLACK);
                    tmp->setColor(Color::RED);
                    this->leftRotate(tmp);
                    tmp = node->parent->left;
                }

                if (tmp) {
                    tmp->setColor(node->parent->getColor());
                    node->parent->setColor(Color::BLACK);
                    if (tmp->left)
                        tmp->left->setColor(Color::BLACK);
                    this->rightRotate(node->parent);
                    node = this->root;
                }
            }
        }
    }

    if (node)
        node->setColor(Color::BLACK);
}

template <class T>
void RBTree<T>::remove(RBNode<T>* node) {
    RBNode<T>* tmp;
    RBNode<T>* minRightNode;
    Color min_orig_color = Color::RED;

    if (node->left == nullptr) {
        tmp = node->right;
        this->transplant(node, node->right);
    }
    else if (node->right == nullptr) {
        tmp = node->left;
        this->transplant(node, node->left);
    }
    else {
        minRightNode = this->minimum(node->right);
        min_orig_color = minRightNode->getColor();

        tmp = minRightNode->right;
        if (minRightNode->parent == node) {
            if (tmp)
                tmp->parent = minRightNode;
        }
        else {
            this->transplant(minRightNode, minRightNode->right);
            minRightNode->right = node->right;
            minRightNode->right->parent = minRightNode;
        }

        this->transplant(node, minRightNode);
        minRightNode->left = node->left;
        minRightNode->left->parent = minRightNode;
        minRightNode->setColor(node->getColor());
    }
    delete node;

    if (min_orig_color == Color::BLACK)
        this->remove_fixup(tmp);
}

template <class T>
bool RBTree<T>::remove(const T& value) {
    RBNode<T>* node = this->search(value);

    if (!node)
        return false;
    
    this->remove(node);
    return true;
}

typedef int Process;

template class RBTree<Process>;
template void deleteRBTree<Process>(RBNode<Process>*);
template void RBTree<Process>::clear(void);
template void RBTree<Process>::leftRotate(RBNode<Process>*);
template void RBTree<Process>::rightRotate(RBNode<Process>*);
template void RBTree<Process>::insert_fixup(RBNode<Process>*);
template void RBTree<Process>::insert(const Process&);
template RBNode<Process>* RBTree<Process>::search(const Process&) const;
template void RBTree<Process>::transplant(RBNode<Process>* u, RBNode<Process>* v);
template RBNode<Process>* RBTree<Process>::minimum(RBNode<Process>* node) const;
template void RBTree<Process>::remove_fixup(RBNode<Process>* node);
template void RBTree<Process>::remove(RBNode<Process>* node);
template bool RBTree<Process>::remove(const Process& value);