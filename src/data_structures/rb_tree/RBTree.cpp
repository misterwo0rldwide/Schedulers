#include "RBTree.h"
#include "../../schedulers/CFS.h"

template <class T>
void deleteRBTree(RBNode<T>* node) {
    if (!node)
        return;
    
    deleteRBTree(node->left);
    deleteRBTree(node->right);
    
    delete node;
}

template <class T, class Compare>
void RBTree<T, Compare>::clear(void) {
    std::lock_guard<std::mutex> lock(mtx);

    deleteRBTree<T>(this->getRoot());
    this->setRoot(nullptr);
    this->setLeftMost(nullptr);
}

template <class T, class Compare>
void RBTree<T, Compare>::leftRotate(RBNode<T>* node) {
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

template <class T, class Compare>
void RBTree<T, Compare>::rightRotate(RBNode<T>* node) {
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

template <class T, class Compare>
void RBTree<T, Compare>::insert_fixup(RBNode<T>* node) {    
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

template <class T, class Compare>
void RBTree<T, Compare>::insert(const T& value) {
    std::lock_guard<std::mutex> lock(mtx);

    RBNode<T>* tmp = nullptr;
    RBNode<T>* node = this->getRoot();

    while (node) {
        tmp = node;
        if (comp(value, node->getValue()))
            node = node->left;
        else
            node = node->right;
    }

    RBNode<T>* newNode = new RBNode<T>(value, nullptr, nullptr, tmp, Color::RED);
    if (!tmp) {
        this->setRoot(newNode);
        this->setLeftMost(newNode);
    }
    else if (comp(value, tmp->getValue())) {
        tmp->left = newNode;
        if (tmp == this->getLeftMost())
            this->setLeftMost(newNode);
    }
    else {
        tmp->right = newNode;
    }
    
    this->total_weight += prioNice::get_nice_weight(node->getValue()->getNice());
    this->insert_fixup(newNode);
}

template <class T, class Compare>
RBNode<T>* RBTree<T, Compare>::search(const T& value) const {
    std::lock_guard<std::mutex> lock(mtx);

    RBNode<T>* tmp = this->getRoot();

    while (tmp) {
        if (comp(tmp->getValue(), value)) 
            tmp = tmp->right;
        else if (comp(value, tmp->getValue()))
            tmp = tmp->left;
        else
            break;
    }

    return tmp;
}

template <class T, class Compare>
void RBTree<T, Compare>::transplant(RBNode<T>* u, RBNode<T>* v) {
    if (u->parent == nullptr)
        this->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    
    if (v)
        v->parent = u->parent;
}

template <class T, class Compare>
RBNode<T>* RBTree<T, Compare>::minimum(RBNode<T>* node) const {
    while (node->left)
        node = node->left;
    
    return node;
}

template <class T, class Compare>
void RBTree<T, Compare>::remove_fixup(RBNode<T>* node) {
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

template <class T, class Compare>
void RBTree<T, Compare>::remove(RBNode<T>* node) {
    std::lock_guard<std::mutex> lock(mtx);

    RBNode<T>* tmp;
    RBNode<T>* minRightNode;
    Color min_orig_color = node->getColor();
    
    size_t node_weight = prioNice::get_nice_weight(node->getValue()->getNice());
    bool removingLeftMost = (node == this->getLeftMost());

    if (node->left == nullptr) {
        tmp = node->right;
        this->transplant(node, node->right);
        
        if (removingLeftMost) {
            if (node->right) {
                this->setLeftMost(this->minimum(node->right));
            } else {
                RBNode<T>* current = node->parent;
                while (current && current->left == nullptr) {
                    current = current->parent;
                }
                this->setLeftMost(current);
            }
        }
    }
    else if (node->right == nullptr) {
        tmp = node->left;
        this->transplant(node, node->left);
        
        if (removingLeftMost) {
            this->setLeftMost(node->left);
        }
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
    
    if (this->root == nullptr) {
        this->setLeftMost(nullptr);
    }
    this->total_weight -= node_weight;

    delete node;
    if (min_orig_color == Color::BLACK)
        this->remove_fixup(tmp);
}

template <class T, class Compare>
bool RBTree<T, Compare>::remove(const T& value) {
    RBNode<T>* node = this->search(value);

    if (!node)
        return false;
    
    this->remove(node);
    return true;
}


template class RBTree<Task*, TimeCmp>;