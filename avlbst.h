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
		void insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node);
		void removeFix(AVLNode<Key,Value>* node, int diff);
		void rotateRight(AVLNode<Key,Value>* node);
		void rotateLeft(AVLNode<Key,Value>* node);

		//inherited
		AVLNode<Key, Value>* internalFind(const Key &key) const;
    static AVLNode<Key, Value>* predecessor(AVLNode<Key, Value> *current);
};


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* temp=static_cast<AVLNode<Key, Value>*>(this->root_);
		AVLNode<Key, Value>* parent=nullptr;
		const Key& key = new_item.first;
		const Value& val = new_item.second;

    while(temp!=nullptr){ //walk the tree
      if(key == temp->getKey()){
        temp->setValue(val);
        return;
      }

			parent=temp;
			if(key < temp->getKey()){
				temp = temp->getLeft();
			}
			else{
				temp = temp->getRight();
			}
    }

    temp = new AVLNode<Key, Value>(key, val, parent);
		if(parent == nullptr){ //empty tree
      this->root_ = temp;
			return;
    } 
		else if(key < parent->getKey()) {
      parent->setLeft(temp);
    } 
		else{
      parent->setRight(temp);
    }

		//fix balances
		int balance = parent->getBalance();
		if(abs(balance)==1){
			parent->setBalance(0);
		}
		else if(balance==0){
			if(temp==parent->getLeft()){
				parent->setBalance(-1);
			}
			else{
				parent->setBalance(1);
			}
			insertFix(parent,temp);
		}
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node){
	AVLNode<Key, Value>* gparent = parent->getParent();
	if(parent==nullptr || gparent==nullptr){
		return;
	}

	//left child
	if(parent == gparent->getLeft()){
		gparent->updateBalance(-1);
		int balance = gparent->getBalance();
		if(balance==0){ //Case 1
			return;
		}
		else if(balance==-1){ //Case 2
			insertFix(gparent,parent);
		}
		else if(balance==-2){ //Case 3
			//zig zig (left left)
			if(node==parent->getLeft()){
				rotateRight(gparent);
				parent->setBalance(0);
				gparent->setBalance(0);
			}

			//zig zag (left right)
			else{
				rotateLeft(parent);
				rotateRight(gparent);

				int nbalance=node->getBalance();
				if(nbalance==-1){
					parent->setBalance(0);
					gparent->setBalance(1);
				}
				else if(nbalance==0){
					parent->setBalance(0);
					gparent->setBalance(0);
				}
				else if(nbalance==1){
					parent->setBalance(-1);
					gparent->setBalance(0);
				}
				node->setBalance(0);
			}
		}
	}

	//right child
	else if(parent == gparent->getRight()){
		gparent->updateBalance(1);
		int balance = gparent->getBalance();
		if(balance==0){ //Case 1
			return;
		}
		else if(balance==1){ //Case 2
			insertFix(gparent,parent);
		}
		else if(balance==2){ //Case 3
			//zig zig (right right)
			if(node==parent->getRight()){
				rotateLeft(gparent);
				parent->setBalance(0);
				gparent->setBalance(0);
			}

			//zig zag (right left)
			else{
				rotateRight(parent);
				rotateLeft(gparent);

				int nbalance=node->getBalance();
				if(nbalance==1){
					parent->setBalance(0);
					gparent->setBalance(-1);
				}
				else if(nbalance==0){
					parent->setBalance(0);
					gparent->setBalance(0);
				}
				else if(nbalance==-1){
					parent->setBalance(1);
					gparent->setBalance(0);
				}
				node->setBalance(0);
			}
		}
	}
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
		AVLNode<Key, Value>* curr = internalFind(key);
		if(curr==nullptr){ //key not found
			return;
		}
		
		//2 children
		if(curr->getLeft()!=nullptr && curr->getRight()!=nullptr){
			AVLNode<Key, Value>* pred=predecessor(curr);
			nodeSwap(curr,pred);
		}

		AVLNode<Key, Value>* parent = curr->getParent();
		int diff=0;
		
		if(parent!=nullptr){
			if(curr==parent->getLeft()){ //left child
				diff=1;
			}
			else{ //right child
				diff=-1;
			}
		}

		//leaf node
		if(curr->getLeft()==nullptr && curr->getRight()==nullptr){
			//update pointers
			if(parent == nullptr){
				this->root_=nullptr;
			}
			else{
				if(curr==parent->getLeft()){
					parent->setLeft(nullptr);
				}
				else{
					parent->setRight(nullptr);
				}	
			}
			delete curr;
			removeFix(parent,diff);
			return;
		}

		//1 child
		AVLNode<Key, Value>* child = nullptr;
		if(curr->getLeft()!=nullptr){  //left child
			child=curr->getLeft();
		}
		else{ //right child
			child=curr->getRight();
		}

		//update pointers
		if(parent == nullptr){
			this->root_=child;
		}
		else{
			if(parent->getLeft()==curr){
				parent->setLeft(child);
			}
			else{
				parent->setRight(child);
			}
		}
    child->setParent(parent);

		delete curr;
		removeFix(parent,diff);
}


template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* node, int diff){
	if(node==nullptr){
		return;
	}

	AVLNode<Key, Value>* parent = node->getParent();
	int ndiff;

	if(parent!=nullptr){
		if(node==parent->getRight()){ //right child
			ndiff=-1;
		}
		else{ //left child
			ndiff=1;
		}
	}

	int nbalance = node->getBalance();
	if((nbalance+diff) == -2){ //Case 1 (left child)
		AVLNode<Key, Value>* curr = node->getLeft();
		int cbalance = curr->getBalance();
		if(cbalance==-1){ //zig zig
			rotateRight(node);
			node->setBalance(0);
			curr->setBalance(0);
			removeFix(parent, ndiff);
		}
		else if(cbalance==0){ //zig zig
			rotateRight(node);
			node->setBalance(-1);
			curr->setBalance(1);
			return;
		}
		else if(cbalance==1){ //zig zag
			AVLNode<Key, Value>* grand = curr->getRight();
			rotateLeft(curr);
			rotateRight(node);

			int gbalance=grand->getBalance();
			if(gbalance==1){ 
				node->setBalance(0);
				curr->setBalance(-1);
			}
			else if(gbalance==0){
				node->setBalance(0);
				curr->setBalance(0);
			}
			else if(gbalance==-1){
				node->setBalance(1);
				curr->setBalance(0);
			}
			grand->setBalance(0);

			removeFix(parent, ndiff);	
		}
	}

	else if((nbalance+diff) == 2){ //Case 1 (right child)
		AVLNode<Key, Value>* curr = node->getRight();
		int cbalance = curr->getBalance();
		if(cbalance==1){ //zig zig
			rotateLeft(node);
			node->setBalance(0);
			curr->setBalance(0);
			removeFix(parent, ndiff);
		}
		else if(cbalance==0){ //zig zig
			rotateLeft(node);
			node->setBalance(1);
			curr->setBalance(-1);
			return;
		}
		else if(cbalance==-1){ //zig zag
			AVLNode<Key, Value>* grand = curr->getLeft();
			rotateRight(curr);
			rotateLeft(node);

			int gbalance=grand->getBalance();
			if(gbalance==-1){ 
				node->setBalance(0);
				curr->setBalance(1);
			}
			else if(gbalance==0){
				node->setBalance(0);
				curr->setBalance(0);
			}
			else if(gbalance==1){
				node->setBalance(-1);
				curr->setBalance(0);
			}
			grand->setBalance(0);

			removeFix(parent, ndiff);	
		}
	}	

		else if((nbalance+diff) == -1){ //Case 2 (left child)
			node->setBalance(-1);
			return;
		}

		else if((nbalance+diff) == 1){ //Case 2 (right child)
			node->setBalance(1);
			return;
		}

		else if((nbalance+diff) == 0){ //Case 3
			node->setBalance(0);
			removeFix(parent, ndiff);
		}

} //end function


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* node){
	AVLNode<Key, Value>* parent = node->getParent();
	AVLNode<Key, Value>* left = node->getLeft(); //new parent

	//fix parent relationships
	left->setParent(parent);
	if(parent==nullptr){
		this->root_=left;
	}
	else if(node==parent->getLeft()){
		parent->setLeft(left);
	}
	else{
		parent->setRight(left);
	}

	//fix children relationships
	AVLNode<Key, Value>* child = left->getRight();
	left->setRight(node);
	node->setParent(left);
	node->setLeft(child);
	if(child != nullptr){
		child->setParent(node);
	}
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* node){
	AVLNode<Key, Value>* parent = node->getParent();
	AVLNode<Key, Value>* right = node->getRight(); //new parent

	//fix parent relationships
	right->setParent(parent);
	if(parent == nullptr){
		this->root_ = right;
	}
	else if(node==parent->getLeft()){
		parent->setLeft(right);
	}
	else{
		parent->setRight(right);
	}

	//fix children relationships
	AVLNode<Key, Value>* child = right->getLeft();
	right->setLeft(node);
	node->setParent(right);
	node->setRight(child);
	if (child != nullptr){
		child->setParent(node);
	}
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2){
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::internalFind(const Key &key) const{
	AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key)); //change node type
	return node;
}


template<class Key, class Value>
AVLNode<Key, Value>* 
AVLTree<Key, Value>::predecessor(AVLNode<Key, Value>* current){ //static
	AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(current)); //change node type
	return node;
}

#endif
