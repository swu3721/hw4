#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void rotateLeft(AVLNode<Key, Value>* node); //Int to represent zig-zig (0) or zig-zag (1)
    void rotateRight(AVLNode<Key, Value>* node); //Int to represent zig-zig (0) or zig-zag (1)
    AVLNode<Key, Value>* getRoot() const;
    void removeFix(AVLNode<Key, Value>* node, int diff);

};

template<class Key, class Value>
AVLNode<Key, Value> *AVLTree<Key, Value>::getRoot() const
{
    return static_cast<AVLNode<Key, Value>*>(this->root_);
}

//Rotate left from grandparent
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* grandparent)
{
    AVLNode<Key, Value>* parent = grandparent->getRight(); //Get parent

    grandparent->setRight(parent->getLeft()); //Set grandparent's right to parent's left

    //Update parent's child to grandparent
    if (parent->getLeft() != nullptr) {
        parent->getLeft()->setParent(grandparent);
    }
    parent->setParent(grandparent->getParent()); //set parent to grandparent old position
    
    //If parent is root, update root, else update great grandparent
    if (parent->getParent() == nullptr) {
        this->root_ = parent;
    } else if (grandparent == grandparent->getParent()->getLeft()){
        grandparent->getParent()->setLeft(parent);
    } else if (grandparent == grandparent->getParent()->getRight()) {
        grandparent->getParent()->setRight(parent);
    }

    //Update parent and grandparent relation
    parent->setLeft(grandparent);
    grandparent->setParent(parent);

    
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* grandparent) 
{
    AVLNode<Key, Value>* parent = grandparent->getLeft(); //Get parent

    grandparent->setLeft(parent->getRight()); //Set grandparent's right to parent's left
    
    //Update parent's child to grandparent
    if (parent->getRight() != nullptr) {
        parent->getRight()->setParent(grandparent);
    }
    parent->setParent(grandparent->getParent()); //set parent to grandparent old position

    //If parent is root, update root, else update great grandparent
    if (parent->getParent() == nullptr) {
        this->root_ = parent;
    } else if (grandparent == grandparent->getParent()->getLeft()){
        grandparent->getParent()->setLeft(parent);
    } else if (grandparent == grandparent->getParent()->getRight()) {
        grandparent->getParent()->setRight(parent);
    }

    //Update parent and grandparent relation
    parent->setRight(grandparent);
    grandparent->setParent(parent);
}


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    // If the tree is empty, set the new node as the root
    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        this->root_->setLeft(nullptr);
        this->root_->setRight(nullptr);
        return;
    }

    // Otherwise, insert the new node into the tree
    AVLNode<Key, Value>* parent = nullptr;
    AVLNode<Key, Value>* currentNode = this->getRoot();
    
    while (currentNode != nullptr) {
        if (new_item.first < currentNode->getKey()) {
            parent = currentNode;
            currentNode = currentNode->getLeft();
        } else if (new_item.first > currentNode->getKey()) {
            parent = currentNode;
            currentNode = currentNode->getRight();
        } else {
            // If the key already exists, update the value and return
            currentNode->setValue(new_item.second);
            return;
        }
    }

    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    newNode->setLeft(nullptr);
    newNode->setRight(nullptr);
    // Attach the new node to its parent
    if (newNode->getKey() < parent->getKey()) {
        parent->setLeft(newNode);
    } else {
        parent->setRight(newNode);
    }

    // Set balance to 0
    newNode->setBalance(0);

    if (parent->getBalance() == -1 || parent->getBalance() == 1) {
        parent->setBalance(0);
    } else if (parent->getBalance() == 0) {
        if (parent->getLeft() == newNode) {
            parent->setBalance(-1);
        } else {
            parent->setBalance(1);
        }
        insertFix(parent, newNode);
    }

}


//Helper insertFix
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* grandparent = parent->getParent();
    if (grandparent == nullptr || parent == nullptr) {
        return; // No further action needed
    }
        
    //Updating grandparent balance
    if (grandparent->getLeft() == parent) {
        grandparent->setBalance(grandparent->getBalance() - 1);
    } else {
        grandparent->setBalance(grandparent->getBalance() + 1);
    }

    int8_t grandparentBal = grandparent->getBalance();
    if (grandparentBal == 0) {
        return;
    }

    if (grandparentBal == -1 || grandparentBal == 1) {
        insertFix(grandparent, parent);
    } else if (grandparentBal == -2) {
        if (grandparent->getLeft() == parent && parent->getLeft() == node) { //Zig-zig (right rotation)
            rotateRight(grandparent);
            parent->setBalance(0);
            grandparent->setBalance(0);
        } else { //Zig-zag
            rotateLeft(parent);
            rotateRight(grandparent);
            if (node->getBalance() == -1) {
                parent->setBalance(0);
                grandparent->setBalance(1);
            } else if (node->getBalance() == 0) {
                parent->setBalance(0);
                grandparent->setBalance(0);
            } else {
                parent->setBalance(-1);
                grandparent->setBalance(0);
            }
            node->setBalance(0);
        }
        return;
    } else if (grandparentBal == 2) {
        if (grandparent->getRight() == parent && parent->getRight() == node) { //Zig-zig (left rotation)
            rotateLeft(grandparent);
            parent->setBalance(0);
            grandparent->setBalance(0);
        } else { //Zig-zag
            rotateRight(parent);
            rotateLeft(grandparent);
            if (node->getBalance() == 1) {
                parent->setBalance(0);
                grandparent->setBalance(-1);
            } else if (node->getBalance() == 0) {
                parent->setBalance(0);
                grandparent->setBalance(0);
            } else {
                parent->setBalance(1);
                grandparent->setBalance(0);
            }
            node->setBalance(0);
        }
        return;
    }
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* removeNode = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
    if (removeNode == nullptr) {
        return;
    }

    if (removeNode->getLeft() != nullptr && removeNode->getRight() != nullptr) {
        AVLNode<Key, Value>* predNode = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(removeNode)); //Find predecessor
        nodeSwap(predNode, removeNode); //Swap predecessor
    }

    AVLNode<Key, Value>* parent = removeNode->getParent();
    int8_t diff = 0;

    if (parent != nullptr) {
        if (parent->getLeft() == removeNode) {
            diff = 1;
        } else {
            diff = -1;
        }
    }


    if (removeNode->getLeft() == nullptr && removeNode->getRight() == nullptr) {
        if (removeNode == getRoot()) {  //If its the root, delete
            delete removeNode;
            this->root_ = nullptr;
        } else { //Remove node and update parent
            if (parent->getLeft() == removeNode) { //Check if left child or right child
                parent->setLeft(nullptr);
            } else {
                parent->setRight(nullptr);
            }
            delete removeNode;
        }
    } else if (removeNode->getRight() == nullptr) { //If there is left child at the remove node
        AVLNode<Key, Value>* child = removeNode->getLeft(); //the child node
        if (removeNode == getRoot()) { //If remove is root, make child root and update
            this->root_ = child;
            child->setParent(nullptr);
        } else {
            if (parent->getLeft() == removeNode) { //If removeNode is left, make the child of remove child of parent
                parent->setLeft(child);
            } else {
                parent->setRight(child);
            }
            child->setParent(parent);
        }
        delete removeNode;
    } else if (removeNode->getLeft() == nullptr) { //If there is right node
        AVLNode<Key, Value>* child = removeNode->getRight(); //the child node
        if (removeNode == getRoot()) { //If remove is root make child root
            this->root_ = child;
            child->setParent(nullptr);
        } else { // Otherwise
            if (parent->getLeft() == removeNode) { //If removeNode is left, make the child of remove child of parent
                parent->setLeft(child);
            } else {
                parent->setRight(child);
            }
            child->setParent(parent);
        }
        delete removeNode;
    }

    removeFix(parent, diff);

}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int diff) 
{
    if (node == nullptr) {
        return;
    }
    
    AVLNode<Key, Value>* parent = node->getParent();
    int8_t newDiff = 0;

    if (parent != nullptr) {
        if (node->getParent()->getLeft() == node) {
            newDiff = 1;
        } else {
            newDiff = -1;
        }
    }
    if (node->getBalance() + diff == -2) { //Case 1
        AVLNode<Key, Value>* child = node->getLeft();
        if (child->getBalance() == -1) { //Case 1a
            rotateRight(node);
            child->setBalance(0);
            node->setBalance(0);
            removeFix(parent, newDiff);
        } else if (child->getBalance() == 0) { //Case 1b
            rotateRight(node);
            child->setBalance(1);
            node->setBalance(-1);
            return;
        } else if (child->getBalance() == 1) { //Case 1c
            AVLNode<Key, Value>* grandChild = child->getRight();
            rotateLeft(child);
            rotateRight(node);
            if (grandChild->getBalance() == 1) {
                node->setBalance(0);
                child->setBalance(-1);
                grandChild->setBalance(0);
            } else if (grandChild->getBalance() == 0) {
                node->setBalance(0);
                child->setBalance(0);
                grandChild->setBalance(0);
            } else if (grandChild->getBalance() == -1) {
                node->setBalance(1);
                child->setBalance(0);
                grandChild->setBalance(0);
            }
        removeFix(parent, newDiff);
        }
    } else if (node->getBalance() + diff == -1) {
        node->setBalance(-1);
        return;
    } else if (node->getBalance() + diff == 1) {
        node->setBalance(1);
        return;
    } else if (node->getBalance() + diff == 0) {
        node->setBalance(0);
        removeFix(parent, newDiff);
    } else if (node->getBalance() + diff == 2) {
        AVLNode<Key, Value>* child = node->getRight();
        if (child->getBalance() == 1) {
            rotateLeft(node);
            node->setBalance(0);
            child->setBalance(0);
            removeFix(parent, newDiff);
        } else if (child->getBalance() == 0) {
            rotateLeft(node);
            node->setBalance(1);
            child->setBalance(-1);
            return;
        } else if (child->getBalance() == -1) {
            AVLNode<Key, Value>* grandChild = child->getLeft();
            rotateRight(child);
            rotateLeft(node);
            if (grandChild->getBalance() == -1) {
                node->setBalance(0);
                child->setBalance(1);
                grandChild->setBalance(0);
            } else if (grandChild->getBalance() == 0) {
                node->setBalance(0);
                child->setBalance(0);
                grandChild->setBalance(0);
            } else if (grandChild->getBalance() == 1) {
                node->setBalance(-1);
                child->setBalance(0);
                grandChild->setBalance(0);
            }
            removeFix(parent, newDiff);
        }
    } 
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
