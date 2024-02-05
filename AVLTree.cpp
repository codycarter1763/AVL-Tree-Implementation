/**
 * Models an AVL tree.
 * @param <E> data type of elements of the tree
 * @author William Duncan, Cody Carter
 * @see AVLTree
 * <pre>
 * Date: 10/18/2023
 * CSC 3102 Programming Project # 2
 * Instructor: Dr. Duncan 
 *
 * DO NOT REMOVE THIS NOTICE (GNU GPL V2):
 * Contact Information: duncanw@lsu.edu
 * Copyright (c) 2023 William E. Duncan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 * </pre>
 */
#include "AVLTree.h"
#include <cstdlib>
#include <iostream>
#include <queue>
#include <cmath>
#include <sstream>

using namespace std;

/* Nested Node class definitions */

template <typename E>
AVLTree<E>::Node::Node(E s)
{
   data = s;
   left = NULL;
   right = NULL;
   bal = EH;
}

/* Outer AVLTree class definitions */

template <typename E>
AVLTree<E>::AVLTree()
{
   root = NULL;
   count = 0;
   cmp = [](E a, E b) -> int{return a < b? -1 : (a == b? 0 : 1);};
}

template <typename E>
AVLTree<E>::AVLTree(std::function<int(E,E)> fn)
{
    root = NULL;
    count = 0;
    if (cmp == nullptr) 
        cmp = fn;
}

template <typename E>
AVLTree<E>::~AVLTree()
{
   recDestroy(root);
}


template <typename E>
bool AVLTree<E>::isEmpty() const
{
   return root == NULL;
}

template<typename E>
void AVLTree<E>::insert(const E& obj)
{
   bool forTaller;
   Node* newNode = new Node(obj);
   /* If it is the first node in the tree */
   if (!inTree(obj))
      count++;
   root = insert(root, newNode, forTaller);
}

template<typename E>
bool AVLTree<E>::inTree(const E& item) const
{
   Node *tmp;
   if (isEmpty())
      return false;
   /*find where it is */
   tmp = root;
   while (1)
   {
      if (cmp(tmp->data,item) == 0)
         return true;
      if (cmp(tmp->data, item) > 0)
      {
         if (!(tmp->left))
            return false;
         /* continue searching */
         tmp = tmp->left;
      }
      else
      {
         if (!(tmp->right))
            return false;
         /* continue searching for insertion pt. */
         tmp = tmp->right;
      }
   }
}

template<typename E>
void AVLTree<E>::remove(const E& item)
{
   bool shorter;
   bool success;
   Node* newRoot;
   if (!inTree(item))
      return;
   newRoot = remove(root, item, shorter, success);
   if (success)
   {
      root = newRoot;
      count--;
   }
}

template<typename E>
const E& AVLTree<E>::retrieve(const E& key) const
{
   Node* tmp;
   if (isEmpty())
      throw AVLTreeException("AVL Tree Exception: tree empty on retrieve()");
   tmp = root;
   while(true)
   {
      if (cmp(tmp->data,key) == 0)
         return tmp->data;
      if (cmp(tmp->data, key) > 0)
      {
         if (tmp->left == NULL)
            throw new AVLTreeException("AVL Tree Exception: key not in tree call to retrieve()");
         tmp = tmp->left;
      }
      else
      {
         if (tmp->right ==  NULL)
            throw new AVLTreeException("AVL Tree Exception: key not in tree call to retrieve()");
         tmp = tmp->right;
      }
   }
   //return tmp->data;
}

template<typename E>
void AVLTree<E>::traverse(FuncType func)
{
   traverse(root, func); //In-order
}

template<typename E>
int AVLTree<E>::size() const
{
   return count;
}

/* BEGIN: Augmented Public Functions */
template<typename E>
void AVLTree<E>::preorderTraverse(FuncType func)
{
   preorderTraverse(root, func);
}

template<typename E>
void AVLTree<E>::postorderTraverse(FuncType func)
{
   postorderTraverse(root, func);
}

template<typename E>
vector<E*> AVLTree<E>::getChildren(E entry) const
{
    Node* parent = root;
    std::vector<E*> children;

    while (parent) 
    {
        if (cmp(parent->data, entry) == 0) {
            if (parent->left) {
                children.push_back(&(parent->left->data));
            } else {
                children.push_back(nullptr);
            }

            if (parent->right) {
                children.push_back(&(parent->right->data));
            } else {
                children.push_back(nullptr);
            }

            return children;
        } 
        else if (cmp(parent->data, entry) > 0) 
        {
            parent = parent->left;
        } 
        else 
        {
            parent = parent->right;
        }
    }

    // If entry is not found, throw an exception or handle it as needed.
    throw AVLTreeException("AVLTreeException: Entry not found in the tree");
}

   
template <typename E>
const E* AVLTree<E>::getParent(E entry) const      
{
    Node* currentNode = root;
    Node* parentNode = nullptr;
    
    while (currentNode != nullptr)
    {
        if (cmp(currentNode->data, entry) == 0)
        {
            // Found the node, return its parent
            return (parentNode != nullptr) ? &(parentNode->data) : nullptr;
        }
        else if (cmp(currentNode->data, entry) > 0)
        {
            // Traverse left
            parentNode = currentNode;
            currentNode = currentNode->left;
        }
        else
        {
            // Traverse right
            parentNode = currentNode;
            currentNode = currentNode->right;
        }
    }

    // Node with given entry not found
    return nullptr;
}   
   

template <typename E>
int AVLTree<E>::ancestors(E entry) const
{
    if (!inTree(entry)) {
        throw AVLTreeException("Entry is not in the tree");
    }

    int numberAncestors = 0;
    Node* currentNode = root;

    while (currentNode) {
        if (cmp(currentNode->data, entry) == 0) 
        {
            // Found the node with the specified entry
            int heightOfNode = height(currentNode);
            return height(root) - heightOfNode;
        } 
        else if (cmp(currentNode->data, entry) > 0) 
        {
            currentNode = currentNode->left;
        } 
        else 
        {
            currentNode = currentNode->right;
        }
    }

    // If entry is not found, throw an exception or handle it as needed.
    throw AVLTreeException("AVLTreeException: Entry not found in the tree");
}

template <typename E>
int AVLTree<E>::descendants(E entry) const
{
    if (!inTree(entry)) 
    {
        throw AVLTreeException("Entry is not in the tree");
    }

    Node* currentNode = root;

    while (currentNode) {
        if (cmp(currentNode->data, entry) == 0) {
            // Found the node with the specified entry
            return countDesc(currentNode);
        } else if (cmp(currentNode->data, entry) > 0) {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }
    }

    // If entry is not found, throw an exception or handle it as needed.
    throw AVLTreeException("AVLTreeException: Entry not found in the tree");
}


template <typename E>
bool AVLTree<E>::isFibonacci() const
{
   int fib = fibonacci(height(root) + 3) - 1;

   if (fib == size())
   {
      return true;
   }

   return false;    
}

template<typename E>
int AVLTree<E>::height() const
{
    return height(root);
}

template<typename E>
int AVLTree<E>::diameter() const
{

    if (root == nullptr)
    {
        return 0;
    }
    else 
   return height(root->left) + height(root->right) + 3;
}

template <typename E>
int AVLTree<E>::fibonacci(int n)
{
   if (n == 0)
   {
      return 0;
   }

   else if (n == 1)
   {
      return 1;
   }

   else
   {
      return fibonacci(n - 1) + fibonacci(n - 2);
   }
}

template <typename E>
bool AVLTree<E>::isComplete() const
{
    if (root == nullptr) 
        return true;
    return isComplete(root,0);
}
/* END: Augmented Public Functions */


/* Private functions */

template<typename E>
void AVLTree<E>::recDestroy(Node* root)
{
   if (root)
   {
      if (root->left) recDestroy(root->left);
      if (root->right) recDestroy(root->right);
      delete root;
   }
   return;
}

template<typename E>
typename AVLTree<E>::Node* AVLTree<E>::insert(Node* curRoot, Node* newNode, bool& taller)
{
   if (curRoot == NULL)
   {
      curRoot = newNode;
      taller = true;
      return curRoot;
   }
   if (cmp(newNode->data,curRoot->data) < 0)
   {
      curRoot->left = insert(curRoot->left,newNode, taller);
      if (taller)
         switch(curRoot->bal)
         {
            case LH: // was left-high -- rotate
               curRoot = leftBalance(curRoot, taller);
               break;
            case EH: //was balanced -- now LH
               curRoot->bal = LH;
               break;  
            case RH: //was right-high -- now EH
               curRoot->bal = EH;
               taller = false;
               break;
         }
      return curRoot;
   }
   if (cmp(newNode->data,curRoot->data) > 0)
   {
      curRoot->right = insert(curRoot->right,newNode,taller);
      if (taller)
         switch(curRoot->bal)
         {
            case LH: // was left-high -- now EH
               curRoot->bal = EH;
               taller=false;
               break;
           case EH: // was balance -- now RH
              curRoot->bal = RH;
              break;
           case RH: //was right high -- rotate
              curRoot = rightBalance(curRoot,taller);
              break;
         }
      return curRoot;
   }
   else
   {
      curRoot->data = newNode->data;
      delete newNode;
      taller = false;
      return curRoot;
   }
}

template<typename E>
typename AVLTree<E>::Node* AVLTree<E>::leftBalance(Node* curRoot, bool& taller)
{
   Node* rightTree;
   Node* leftTree;   
   leftTree = curRoot->left;
   switch(leftTree->bal)
   {
      case LH: //left-high -- rotate right
         curRoot->bal = EH;
         leftTree->bal = EH;
         // Rotate right
         curRoot = rotateRight(curRoot);
         taller = false;
         break;
      case EH: // This is an error
         cerr<<"AVL Tree Error: error in balance tree in call to leftBalance()"<<endl;
         exit(-5);
         break;   
      case RH: // right-high - requires double rotation: first left, then right
         rightTree = leftTree->right;
         switch(rightTree->bal)
         {
            case LH:
               curRoot->bal = RH;
               leftTree->bal = EH;
               break;   
            case EH:
               curRoot->bal = EH;
               leftTree->bal = EH;   
               break;
            case RH:
               curRoot->bal = EH;
               leftTree->bal = LH;
               break;
         }
         rightTree->bal = EH;
         // rotate left
         curRoot->left = rotateLeft(leftTree);
         //rotate right
         curRoot = rotateRight(curRoot);
         taller= false;
   }
   return curRoot;
}

template<typename E>
typename AVLTree<E>::Node* AVLTree<E>::rightBalance(Node* curRoot, bool& taller)
{
   Node* rightTree;
   Node* leftTree;
   rightTree = curRoot->right;
   switch(rightTree->bal)
   {
      case RH: //right-high -- rotate left   
         curRoot->bal = EH;
         rightTree->bal = EH;
         // Rotate left
         curRoot = rotateLeft(curRoot);
         taller = false;
         break;
      case EH: // This is an error
         cerr<<"AVL Tree Error: error in balance tree in call to rightBalance()"<<endl;
         exit(1);
         break;
      case LH: // left-high - requires double rotation: first right, then left
         leftTree = rightTree->left;
         switch(leftTree->bal)
         {
            case RH:
               curRoot->bal = LH;
               rightTree->bal = EH;
               break;
            case EH:   
               curRoot->bal = EH;
               rightTree->bal = EH;    
               break;
            case LH:
               curRoot->bal = EH;
               rightTree->bal = RH;
               break;
         }
         leftTree->bal = EH;
         // rotate right
         curRoot->right = rotateRight(rightTree);
         //rotate left
         curRoot = rotateLeft(curRoot);
         taller = false;
   }
   return curRoot;
}

template<typename E>
typename AVLTree<E>::Node* AVLTree<E>::rotateLeft(Node* node)
{
   Node* tmp;
   tmp = node->right; 
   node->right = tmp->left;
   tmp->left = node;
   return tmp;
}

template<typename E>
typename AVLTree<E>::Node* AVLTree<E>::rotateRight(Node* node)
{
   Node* tmp;
   tmp = node->left;
   node->left = tmp->right;
   tmp->right = node;
   return tmp;
}   


template<typename E>
void AVLTree<E>::traverse(Node* node, FuncType func)
{
   if (node)
   {
      traverse(node->left,func);
      func(node->data);
      traverse(node->right,func);
   }
}


template<typename E>
typename AVLTree<E>::Node* AVLTree<E>::remove(Node* node,const E& key, bool& shorter, bool& success)
{
   Node* delPtr;   
   Node* exchPtr;
   Node* newRoot;
   if (node == NULL) 
   {
      shorter = false;
      success = false;
      return NULL;
   }
   if (cmp(key,node->data) < 0)
   {
      node->left = remove(node->left,key,shorter,success);
      if (shorter)
         node = deleteRightBalance(node,shorter);
   }
   else if (cmp(key,node->data) > 0)
   {
      node->right = remove(node->right,key,shorter,success);
      if (shorter)
         node = deleteLeftBalance(node,shorter);
   }
   else
   {
      delPtr = node;
      if (node->right == NULL)
      {
         newRoot = node->left;
         success = true;
         shorter = true;
         delete delPtr;
         return newRoot;
      }
      if(node->left == NULL)
      {
         newRoot = node->right;
         success = true;
         shorter = true;
         delete delPtr;
         return newRoot;
      }
      else
      {
         exchPtr = node->left;
         while(exchPtr->right != NULL)
            exchPtr = exchPtr->right;
         node->data = exchPtr->data;
         node->left = remove(node->left,exchPtr->data,shorter,success);
         if (shorter)
            node = deleteRightBalance(node,shorter);
      }
   }
   return node;
}


template<typename E>
typename AVLTree<E>::Node* AVLTree<E>::deleteRightBalance(Node* node,bool& shorter)
{
   Node* rightTree;
   Node* leftTree;
   switch(node->bal)
   {
      case LH: //deleted left -- now balanced  
         node->bal = EH;
         break;
         case EH: //now right high
         node->bal = RH;
         shorter = false;
         break;
      case RH: // right high -- rotate left
         rightTree = node->right;
         if (rightTree->bal == LH)
         {
            leftTree = rightTree->left;
            switch(leftTree->bal)
            {
               case LH:
                  rightTree->bal = RH;
                  node->bal = EH;
                  break;
               case EH:
                  node->bal = EH;
                  rightTree->bal = EH;
                  break;
               case RH:
                  node->bal = LH;
                  rightTree->bal = EH;
                  break;
            }
            leftTree->bal = EH;
            //rotate right, then left
            node->right = rotateRight(rightTree);
            node = rotateLeft(node);
         }
         else
         {
            switch(rightTree->bal)
            {
               case LH: 
               case RH:
                  node->bal = EH;
                  rightTree->bal = EH;
                  break;
               case EH:
                  node->bal = RH;
                  rightTree->bal = LH;
                  shorter = false;
                  break;
            }
            node = rotateLeft(node);
         }
      }
   return node;
}

template<typename E>
typename AVLTree<E>::Node* AVLTree<E>::deleteLeftBalance(Node* node,bool& shorter)
{
   Node* rightTree;
   Node* leftTree;
   switch(node->bal)
   {
      case RH: //deleted right -- now balanced  
         node->bal = EH;
         break;
      case EH: //now left high
         node->bal = LH;
         shorter = false;
         break;
      case LH: // left high -- rotate left
         leftTree = node->left;
         if (leftTree->bal == RH)
         {
            rightTree = leftTree->right;
            switch(rightTree->bal)
            {
               case RH:
                  leftTree->bal = LH;
                  node->bal = EH;
                  break;
               case EH:
                  node->bal = EH;
                  leftTree->bal = EH;
                  break;
               case LH:
                  node->bal = RH;
                  leftTree->bal = EH;
                  break;
            }
            rightTree->bal = EH;
            //rotate left, then right
            node->left = rotateLeft(leftTree);
            node = rotateRight(node);
         }
         else
         {
            switch(leftTree->bal)
            {
               case RH: 
               case LH:
                  node->bal = EH;
                  leftTree->bal = EH;
                  break;
               case EH:
                  node->bal = LH;
                  leftTree->bal = RH;
                  shorter = false;
                  break;
            }
            node = rotateRight(node);
         }
      }
   return node;
}
/* BEGIN: Augmented Private Auxiliary Functions */

template<typename E>
int AVLTree<E>::height(Node* node) const
{
   if(node == nullptr)
   {
       return -1;
   }
   int leftHeight = height(node->left);
   int rightHeight = height(node->right);
   return max(leftHeight, rightHeight) + 1;    
}

template <typename E>
void AVLTree<E>::preorderTraverse (Node* node, FuncType func)
{
    if (node)
    {
        func(node->data);
        preorderTraverse(node->left, func);
        preorderTraverse(node->right, func);
    }
}
template <typename E>
void AVLTree<E>::postorderTraverse (Node* node, FuncType func)
{
    if (node)
    {
        postorderTraverse(node->left, func);
        postorderTraverse(node->right, func);
        func(node->data);
    }
}

template<typename E>
int AVLTree<E>::countDesc(Node* node) const
{
   if (node == nullptr) 
    {
        return 0; // Base case: empty subtree has no descendants
    }
    int leftDescendants = countDesc(node->left);
    int rightDescendants = countDesc(node->right);
    
    int totalDescendants = leftDescendants + rightDescendants;
    
    if (node->left != nullptr && node->right != nullptr)
    {
        totalDescendants += 2;
    }
    
    else if (node->left != nullptr || node->right != nullptr)
    {
        totalDescendants += 1;
    }
    return totalDescendants;
}

template<typename E>  
bool AVLTree<E>::isComplete(Node* node, int index) const
{
    //Implement this function
    if (node == nullptr) return true;
    if (index >= size()) return false;
    return isComplete(node->left,2*index+1) && isComplete(node->right,2*index+2);
}
/* END: Augmented Private Auxiliary Functions */ 

