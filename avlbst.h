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
    AVLTree() : BinarySearchTree<Key, Value>() {}
    virtual ~AVLTree() = default;
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    // Add helper functions here
    void rotateLeft(AVLNode<Key, Value>* z);
    void rotateRight(AVLNode<Key, Value>* z);
    void insert_fix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void remove_fix(AVLNode<Key, Value>* n,int8_t diff);
};
template<class Key, class Value>
void AVLTree<Key, Value>::remove_fix(AVLNode<Key, Value>* n,int8_t diff)
{
  if(n==nullptr) return;
  int8_t ndiff = 0;
  AVLNode<Key, Value>* p = n->getParent();
  if(p!=nullptr)
  {
    if(n==p->getLeft())
    ndiff = 1;
    if(n==p->getRight())
    ndiff = -1;
  }
  if(diff == -1)
  {
    if(n->getBalance()+diff == -2)
    {
      AVLNode<Key, Value>* c = n->getLeft();
      if(c->getBalance()==-1)
      {
        rotateRight(n);
        n->setBalance(0);
        c->setBalance(0);
        remove_fix(p,ndiff);
      }
      else if(c->getBalance()==0)
      {
        rotateRight(n);
        n->setBalance(-1);
        c->setBalance(1);
      }
      else if(c->getBalance()==1)
      {
        AVLNode<Key, Value>* g = c->getRight();
        rotateLeft(c);
        rotateRight(n);
        if(g->getBalance()==1)
        {
          n->setBalance(0);
          c->setBalance(-1);
          g->setBalance(0);
        }
        else if(g->getBalance()==0)
        {
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        }
        else if(g->getBalance()==-1)
        {
          n->setBalance(1);
          c->setBalance(0);
          g->setBalance(0);
        }
        remove_fix(p,ndiff);
      }
    }
    else if(n->getBalance()+diff == -1)
    {
      n->setBalance(-1);
    }
    else if(n->getBalance()+diff == 0)
    {
      n->setBalance(0);
      remove_fix(p,ndiff);
    }
  }
  else if(diff == 1)
  {
    if(n->getBalance()+diff == 2)
    {
      AVLNode<Key, Value>* c = n->getRight();
      if(c->getBalance()==1)
      {
        rotateLeft(n);
        n->setBalance(0);
        c->setBalance(0);
        remove_fix(p,ndiff);
      }
      else if(c->getBalance()==0)
      {
        rotateLeft(n);
        n->setBalance(1);
        c->setBalance(-1);
      }
      else if(c->getBalance()==-1)
      {
        AVLNode<Key, Value>* g = c->getLeft();
        rotateRight(c);
        rotateLeft(n);
        if(g->getBalance()==-1)
        {
          n->setBalance(0);
          c->setBalance(1);
          g->setBalance(0);
        }
        else if(g->getBalance()==0)
        {
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        }
        else if(g->getBalance()==1)
        {
          n->setBalance(-1);
          c->setBalance(0);
          g->setBalance(0);
        }
        remove_fix(p,ndiff);
      }
    }
    else if(n->getBalance()+diff == 1)
    {
      n->setBalance(1);
      return;
    }
    else if(n->getBalance()+diff == 0)
    {
      n->setBalance(0);
      remove_fix(p,ndiff);
    }
  }
}
template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key, Value>* parent,AVLNode<Key, Value>* node)
{
  if(parent==nullptr||parent->getParent()==nullptr) return;
  AVLNode<Key, Value>* grandpa = parent->getParent();
  if(grandpa->getLeft()==parent)
  {
    grandpa->updateBalance(-1);
    if(grandpa->getBalance()==0) return;
    else if(grandpa->getBalance()==-1) return insert_fix(grandpa,parent);
    else if(grandpa->getBalance()==-2) 
    {
      if(node==parent->getLeft())
      {
        rotateRight(grandpa);
        grandpa->setBalance(0);
        parent->setBalance(0);
      }
      else
      {
        rotateLeft(parent);
        rotateRight(grandpa);
        if(node->getBalance()==-1)
        {
          parent->setBalance(0);
          grandpa->setBalance(1);
          node->setBalance(0);
        }
        else if(node->getBalance()==0)
        {
          parent->setBalance(0);
          grandpa->setBalance(0);
          node->setBalance(0);
        }
        else if(node->getBalance()==1)
        {
          parent->setBalance(-1);
          grandpa->setBalance(0);
          node->setBalance(0);
        }
      }
    }
  }
  else
  {
    grandpa->updateBalance(1);
    if(grandpa->getBalance()==0) return;
    else if(grandpa->getBalance()==1) return insert_fix(grandpa,parent);
    else if(grandpa->getBalance()==2) 
    {
      if(parent->getRight()==node)
      {
        rotateLeft(grandpa);
        grandpa->setBalance(0);
        parent->setBalance(0);
      }
      else
      {
        rotateRight(parent);
        rotateLeft(grandpa);
        if(node->getBalance()==1)
        {
          parent->setBalance(0);
          grandpa->setBalance(-1);
          node->setBalance(0);
        }
        else if(node->getBalance()==0)
        {
          parent->setBalance(0);
          grandpa->setBalance(0);
          node->setBalance(0);
        }
        else if(node->getBalance()==-1)
        {
          parent->setBalance(1);
          grandpa->setBalance(0);
          node->setBalance(0);
        }
      }
    }
  }
}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* z)
{
  if(z == nullptr || z->getRight() == nullptr) {
    return; 
  }
  AVLNode<Key, Value>* newY = z->getRight();
  AVLNode<Key, Value>* parent = z->getParent();
  AVLNode<Key, Value>* newYLeft = newY->getLeft();
  // Update z's right child
  z->setRight(newYLeft);
  if (newYLeft != nullptr) {
    newYLeft->setParent(z);
  }
  // Update newY
  newY->setLeft(z);
  z->setParent(newY);
  newY->setParent(parent);
  // Update parent's child pointer
  if (parent != nullptr) {
    if (parent->getLeft() == z) {
      parent->setLeft(newY);
    } else {
      parent->setRight(newY);
    }
  } else {
    this->root_ = newY;
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* z)
{
  if(z == nullptr || z->getLeft() == nullptr) {
    return; 
  }
  AVLNode<Key, Value>* newY = z->getLeft();
  AVLNode<Key, Value>* parent = z->getParent();
  AVLNode<Key, Value>* newYRight = newY->getRight();
  // Update z
  z->setLeft(newYRight);
  if (newYRight != nullptr) {
    newYRight->setParent(z);
  }
  // Update newY's left child and parent
  newY->setRight(z);
  z->setParent(newY);
  newY->setParent(parent);
  // Update parent's child pointer
  if (parent != nullptr) {
    if (parent->getRight() == z) {
      parent->setRight(newY);
    } else {
      parent->setLeft(newY);
    }
  } else {
    this->root_ = newY;
  }
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if (this->root_==nullptr) //empty
    {
      AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
      this->root_=new_node;
      new_node->setBalance(0);
      return;
    }
    if(this->internalFind(new_item.first)!=nullptr)
    {
      this->internalFind(new_item.first)->setValue(new_item.second);
      return;
    }
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;
    while(curr!=nullptr)
    {
      if(new_item.first<curr->getKey())
      {
        parent = curr;
        curr = curr->getLeft();
      }
      else
      {
        parent = curr;
        curr = curr->getRight();
      }
    }
    AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    curr = new_node;
    new_node->setBalance(0);
    if (new_item.first < parent->getKey()) 
    {
        parent->setLeft(new_node);
    } 
    else 
    {
        parent->setRight(new_node);
    }
    if(parent->getBalance()==-1)
    {
      parent->setBalance(0);
    }
    else if(parent->getBalance()==1)
    {
      parent->setBalance(0);
    }
    else if(parent->getBalance()==0)
    {
      if(new_node->getKey() < parent->getKey())
      {
        parent->setBalance(-1);
      }
      else
      {
        parent->setBalance(1);
      }
      insert_fix(parent,new_node);
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
  int8_t diff = 0;
  AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
  if (node == nullptr) return;
  if (node->getLeft() != nullptr && node->getRight() != nullptr)
  {//find the predecessor
    AVLTree<Key, Value>::nodeSwap(static_cast<AVLNode<Key,Value>*>(BinarySearchTree<Key,Value>::predecessor(node)),node);
  }
  AVLNode<Key, Value>* parentNode = static_cast<AVLNode<Key,Value>*>(node->getParent());
  if (parentNode!=nullptr)
  {
      if(node == parentNode->getLeft())
      {
        diff = 1;
          //parentNode->updateBalance(diff);
      }
      else
      {
        diff = -1;
          //parentNode->updateBalance(diff);
      }
      Node<Key, Value>* child = nullptr;
      if (node->getLeft() != nullptr&&node->getRight()==nullptr) //if has left child
        child = node->getLeft();
      else if (node->getRight() != nullptr&&node->getLeft()==nullptr) //if has right child
        child = node->getRight();
      if (node == parentNode->getLeft()) //if it's a left child
      {
        parentNode->setLeft(child);
        if(child!=nullptr)
        {
          child->setParent(node->getParent());
        }
      }
      else //if it's a right child
      {
        parentNode->setRight(child);
        if(child!=nullptr)
        {
          child->setParent(node->getParent());
        }
      }
      delete node; //delete the node
      remove_fix(parentNode,diff);
   }
  else // if 2 children
  {
    Node<Key, Value>* child = nullptr;
      if (node->getLeft() != nullptr&&node->getRight()==nullptr) //if has left child
        child = node->getLeft();
      if (node->getRight() != nullptr&&node->getLeft()==nullptr) //if has right child
        child = node->getRight();
      this->root_ = child;
      if(child!=nullptr)
      child->setParent(nullptr);
      delete node; //delete the node
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


