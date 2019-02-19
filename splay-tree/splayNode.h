#ifndef SPLAY_NODE_H
#define SPLAY_NODE_H

/**
 * @brief Node for the Splay tree
 * @tparam T type of the key
 * @tparam V type of the value pointer
 */

template <typename T, typename V>
class Node {

public:
    
    /**
     * Key according to which is the splay tree sorted
     */
    T key;

    /**
     * Pointer to value of the node
     */
    V * value;

    /**
     * Pointer to left child of the node
     */
    Node * left;

    /**
     * Pointer to right child of the node
     */
    Node * right;

    /**
     * Pointer to parent node
     */
    Node * parent;

    /**
     * Constructor that sets pointers to nullptr
     */
    Node();

    /**
     * Destructor deletes children if not nullptr
     */
    ~Node();
};

template <typename T, typename V>
Node<T, V>::Node() {
    left = nullptr;
    right = nullptr;
    parent = nullptr;
}

template <typename T, typename V>
Node<T, V>::~Node() {
    if(left)
        delete left;
    if(right)
        delete right;
}

#endif
