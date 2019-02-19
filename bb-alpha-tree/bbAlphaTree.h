#ifndef BB_ALPHA_TREE_H
#define BB_ALPHA_TREE_H

#include <iostream>

using namespace std;


template <class K>
class BBAlphaNode {
public:

    /** 
     * Number of all descendant nodes
     */
    int count;

    /** 
     * Comparable key of the node
     */ 
    K key;

    /** 
     * Left child
     */
    BBAlphaNode<K> * left;

    /** 
     * Right child
     */
    BBAlphaNode<K> * right;

    
    inline BBAlphaNode(const K & key) : count(0), key(key), left(nullptr), right(nullptr) {}
};

template <class K>
class BBAlphaTree {

    /**
     * Factor of the tree balance - between 0.5 and 1
     */
    double alpha;

    /**
     * Root of the tree
     */
    BBAlphaNode<K> * root;

public:
    
    /**
     * @param alpha factor of the tree balance
     */
    inline BBAlphaTree(double alpha = 0.52) : alpha(alpha), root(nullptr) {}

    inline ~BBAlphaTree() {
        destroyTree(root);
    }

    /**
     * Recursively destroys tree
     * @param node root of the subtree to destroy
     */
    inline void destroyTree(BBAlphaNode<K> * node) {
        if(!node)
            return;
        destroyTree(node -> right);
        destroyTree(node -> left);
        delete node;
    }

    /**
     * Rebuilds subtree by creating an ordered array and building new balanced tree
     * @param node root of the subtree
     */
    inline BBAlphaNode<K> * rebuildTree(BBAlphaNode<K> * node) {
        if(node -> count <= 0)
            return node;

        BBAlphaNode<K> ** array = new BBAlphaNode<K>*[node -> count + 1];
        makeArray(node, array);

        BBAlphaNode<K> * res = buildTree(array, node -> count + 1);
        delete [] array;
        return res;
    }

    /**
     * Makes ordered array out of subtree
     * @param node root of the subtree
     * @param array 
     */
    inline void makeArray(BBAlphaNode<K> * node, BBAlphaNode<K> ** array) {
        if(!node)
            return;

        long int position = 0;
        if(node -> left)
            position = node -> left -> count + 1;

        array[position] = node;

        makeArray(node -> left, array);
        makeArray(node -> right, array + position + 1);
    }

    /**
     * Builds new balanced subtree from the ordered array
     * @param array ordered array
     * @param size current size of array/subarray
     */
    inline BBAlphaNode<K> * buildTree(BBAlphaNode<K> ** array, long int size) {
        if(size <= 0)
            return nullptr;

        long int index = size / 2;

        array[index] -> count = size - 1;
        array[index] -> left = buildTree(array, index);
        array[index] -> right = buildTree(array + index + 1, size - index - 1);
        return array[index];
    }

    /**
     * Insert new key to the tree (iterative)
     * @param key
     */
    inline void insert(const K & key) {
        BBAlphaNode<K> * node = new BBAlphaNode<K>(key);

        if(!root) {
            root = node;
            return;
        }

        BBAlphaNode<K> * tmp = root;
        BBAlphaNode<K> * prev = nullptr;
        BBAlphaNode<K> * prevRebuild = nullptr;
        BBAlphaNode<K> * toRebuild = nullptr;

        if(root)
            root -> count++;

        while(tmp) {
            if(node -> key < tmp -> key) {
                if(tmp -> left) {
                    tmp -> left -> count ++;
                    if(!subtreeConditionValid(tmp) && !toRebuild) {
                        toRebuild = tmp;
                        prevRebuild = prev;
                    }
                    prev = tmp;
                    tmp = tmp -> left;
                }
                else {
                    tmp -> left = node;
                    break;
                }
            }
            else {
                if(tmp -> right) {
                    tmp -> right -> count ++;
                    if(!subtreeConditionValid(tmp) && !toRebuild) {
                        toRebuild = tmp;
                        prevRebuild = prev;
                    }
                    prev = tmp;
                    tmp = tmp -> right;
                }
                else {
                    tmp -> right = node;
                    break;
                }
            }
        }

        if(toRebuild) {
            if(prevRebuild) {
                if(prevRebuild -> left == toRebuild)
                    prevRebuild -> left = rebuildTree(toRebuild);
                else prevRebuild -> right = rebuildTree(toRebuild);
            } else root = rebuildTree(toRebuild);
        }
    }

private:

    /**
     * Verifies if condition of balance is valid for the given subtree
     * @param node root of the subtree
     */
    inline bool subtreeConditionValid(BBAlphaNode<K> * node) {
        return ((node -> count + 1) * alpha) >= subTreeCount(node -> left)
               && ((node -> count + 1) * alpha) >= subTreeCount(node -> right);
    }

    /**
     * Returns number of nodes in the subtree (including root)
     * @param node root of the subtree
     */
    inline long int subTreeCount(BBAlphaNode<K> * node) {
        return node? (node -> count + 1) : 0;
    }
};

#endif
