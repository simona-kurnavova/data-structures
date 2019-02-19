#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

#include <iostream>
#include "splayNode.h"

using namespace std;

/**
 * @brief Splay tree implementation
 * @details According to parameter "simple" in constructor, uses simple (zig, zag) or complex rotations (zig-zag, zig-zig, ...)
 *
 * @tparam T type of the key
 * @tparam V type of the pointer to value
 */

template <typename T, typename V>
class SplayTree {

    /**
     * Root of the tree
     */
    Node<T,V> * root;
    
    /**
     * Attribute for deciding which rotations to use
     */
    bool simple;

public:
    
    /**
     * Constructor of SplayTree
     * @param simple if true, tree uses simple rotations, otherwise uses complex two-step ones, default: false
     */
    SplayTree(bool simple = false);
    
    /**
     * Recursive destructor
     */
    ~SplayTree();
    
    /**
     * Iterative insert of pair - key and value, the node is splayed afterward
     * @param key the element according to which nodes will be sorted
     * @param value stored value, can be null
     */
    void insert(const T & key, V * value);
    
    /**
     * Iterative find of the given key, node with the found key is splayed afterward
     * @param key to find
     * @return the value of the found node, or null if not found
     */
    V * find(const T & key);

private:
    
    /**
     * Chooses proper splay method (either smartSplay or simpleSplay), according to "simple" attribute
     * @param node
     * @param parent
     */
    void splay(Node<T,V> * node, Node<T,V> * parent);
    
    /**
     * Simple rotations
     * @param node the future root
     * @param parent the parent of future root
     */
    void simpleSplay(Node<T,V> * node, Node<T,V> * parent);
    
    /**
     * Simple rotation of the node
     */
    Node<T, V> * simpleRotation(Node<T,V> * node, Node<T,V> * parent);
    
    /**
     * Called after simple rotation to set new parents for both node and parent
     */
    Node<T, V> * setParents(Node<T,V> * node, Node<T,V> * top);
    
    /**
     * Zig rotation of the node
     */
    Node<T, V> * zig(Node<T, V> * node, Node<T, V> * top);
    
    /**
     * Zag rotation of the node
     */
    Node<T, V> * zag(Node<T, V> * node, Node<T, V> * top);
    
    /**
     * Complex rotations
     * @param node the future root
     * @param parent the parent of future root
     */
    void smartSplay(Node<T, V> * node, Node<T, V> * parent);
    
    /**
     * Zig-zag rotation of the node
     */
    Node<T, V> * zigZag(Node<T, V> * node, Node<T,V> * parent, Node<T,V> * grandparent);
    
    /**
     * Zig-zig rotation of the node
     */
    Node<T, V> * zigZig(Node<T, V> * node, Node<T,V> * parent, Node<T,V> * grandparent);
    
    /**
     * Zag-zag rotation of the node
     */
    Node<T, V> * zagZag(Node<T, V> * node, Node<T,V> * parent, Node<T,V> * grandparent);
    
    /**
     * Zag-zig rotation of the node
     */
    Node<T, V> * zagZig(Node<T, V> * node, Node<T,V> * parent, Node<T,V> * grandparent);
};

template <typename T, typename V>
void SplayTree<T,V>::splay(Node<T,V> * node, Node<T,V> * parent){
    if(simple)
        simpleSplay(node, parent);
    else smartSplay(node, parent);
}

template <typename T, typename V>
Node<T,V> * SplayTree<T,V>::zigZig(Node<T,V> * node, Node<T,V> * parent, Node<T,V> * grandparent) {
    return zig(node, zig(parent, grandparent));
}

template <typename T, typename V>
Node<T,V> * SplayTree<T,V>::zagZig(Node<T,V> * node, Node<T,V> * parent, Node<T,V> * grandparent) {
    return zig(zag(node, parent), grandparent);
}

template <typename T, typename V>
Node<T,V> * SplayTree<T,V>::zagZag(Node<T,V> * node, Node<T,V> * parent, Node<T,V> * grandparent) {
    return zag(node, zag(parent, grandparent));
}

template <typename T, typename V>
Node<T,V> * SplayTree<T,V>::zigZag(Node<T,V> * node, Node<T,V> * parent, Node<T,V> * grandparent) {
    return zag(zig(node, parent), grandparent);
}

template <typename T, typename V>
void SplayTree<T,V>::smartSplay(Node<T,V> * node, Node<T,V> * parent){
    Node<T,V> * grandparent = parent -> parent;
    while(parent) {
        if(grandparent) {
            if(grandparent -> left == parent)
                if(parent -> left == node)
                    node = zigZig(node, parent, grandparent);
                else
                    node = zagZig(node, parent, grandparent);
            else if(parent -> left == node)
                node = zigZag(node, parent, grandparent);
            else
                node = zagZag(node, parent, grandparent);

            if(node -> parent)
                grandparent = node -> parent -> parent;
            else grandparent = nullptr;
        }
        else node = simpleRotation(node, parent);
        parent = node -> parent;
    }
    root = node;
}

template <typename T, typename V>
void SplayTree<T,V>::simpleSplay(Node<T,V> * node, Node<T,V> * parent) {
    while(parent) {
        node = simpleRotation(node, parent);
        parent = node -> parent;
    }
    root = node;
}

template <typename T, typename V>
Node<T,V> * SplayTree<T,V>::simpleRotation(Node<T,V> * node, Node<T,V> * parent) {
    if (parent -> left == node)
        return zig(node, parent);
    return zag(node, parent);
}

template <typename T, typename V>
Node<T,V> * SplayTree<T,V>::zig(Node<T,V> * node, Node<T,V> * top){
    top -> left = node -> right;
    if(top -> left) top -> left -> parent = top;
    node -> right = top;
    return setParents(node, top);
}

template <typename T, typename V>
Node<T,V> * SplayTree<T,V>::zag(Node<T,V> * node, Node<T,V> * top){
    top -> right = node -> left;
    if(top -> right) top -> right -> parent = top;
    node -> left = top;
    return setParents(node, top);
}

template <typename T, typename V>
Node<T,V> * SplayTree<T,V>::setParents(Node<T,V> * node, Node<T,V> * top){
    node -> parent = top -> parent;
    top -> parent = node;
    if(node -> parent) {
        if(node -> parent -> left == top)
            node -> parent -> left = node;
        else node -> parent -> right = node;
    }
    return node;
}

template <typename T, typename V>
SplayTree<T,V>::SplayTree(bool simple) {
    this -> simple = simple;
    root = nullptr;
}

template <typename T, typename V>
SplayTree<T,V>::~SplayTree() {
    if(root)
        delete root;
}

template <typename T, typename V>
void SplayTree<T,V>::insert(const T & key, V * value){
    Node<T,V> * node = new Node<T,V>();
    node -> key = key;
    node -> value = value;

    if(!root) {
        root = node;
        return;
    }

    Node<T,V> * tmp = root;
    while(tmp) {
        if(tmp -> key >= key)
            if(tmp -> left) tmp = tmp -> left;
            else {
                node -> parent = tmp;
                tmp -> left = node;
                break;
            }
        else if(tmp -> right) tmp = tmp -> right;
        else {
            node -> parent = tmp;
            tmp -> right = node;
            break;
        }
    }
    splay(node, tmp);
}

template <typename T, typename V>
V * SplayTree<T,V>::find(const T & key) {
    Node<T,V> * tmp = root;
    int count = 0;
    while(tmp) {
        if(tmp -> key == key) {
            if(tmp -> parent)
                splay(tmp, tmp -> parent);
            return tmp -> value;
        }
        if(tmp -> key > key)
            tmp = tmp -> left;
        else tmp = tmp -> right;
        count++;
    }
    return nullptr;
}
#endif
