#include "../headers/avl_tree.h"
#include <algorithm>
#include <cmath>
using namespace std;

template<typename K, typename V>
AVLTree<K, V>::AVLTree() : BST<K, V>() {
}

template<typename K, typename V>
AVLTree<K, V>::AVLTree(function<bool(const K&, const K&)> comp) : BST<K, V>(comp) {
}
//---------------------------------------------------------------------------------
template<typename K, typename V>
bool AVLTree<K, V>::insert(const K& key, const V& value) 
{
    shared_ptr<BSTNode> temp = nullptr;
    temp = this->findHelper(this->root, key);
    if(temp)
    {
        return false;
    }

    this->root = this->insertAVL(this->root, key, value);

    temp = this->findHelper(this->root, key);
    if(temp)
    {
        this->nodeCount++;
        return true;
    }
    else
    {
        return false;
    }
}

template<typename K, typename V>
shared_ptr<typename AVLTree<K, V>::BSTNode> AVLTree<K, V>::insertAVL(shared_ptr<BSTNode> node, const K& key, const V& value) {
    if (node == nullptr)
    {
        return make_shared<BSTNode>(key, value);
    }
    else if (node->key == key)
    {
        return node;
    }
    else if (this->comparator(key, node->key))
    {
        node->left = this->insertAVL(node->left, key, value);
        if(node->left)
        {
            node->left->parent = node;
        }
    }
    else
    {
        node->right = this->insertAVL(node->right, key, value);
        if(node->right)
        {
            node->right->parent = node;
        }
    }

    this->updateHeight(node);
    node = this->rebalance(node);
    return node;
}

template<typename K, typename V>
bool AVLTree<K, V>::remove(const K& key) {
    shared_ptr<BSTNode> temp = nullptr;
    temp = this->findHelper(this->root, key);
    if(!temp)
    {
        return false;
    }

    this->root = this->removeAVL(this->root, key);
    /* this additional part is to confirm whether the node is removed or not. */
    
    temp = this->findHelper(this->root, key);
    if(temp)
    {
        return false;
    }
    else
    {
        this->nodeCount--;
        return true;
    }
}

template<typename K, typename V>
shared_ptr<typename AVLTree<K, V>::BSTNode> AVLTree<K, V>::removeAVL(shared_ptr<BSTNode> node, const K& key) {
    if (node == nullptr)
    {
        return node;
    }
    else if(node->key == key)
    {
        if (node->left && node->right)
        {
            shared_ptr<BSTNode> succ = this->findMinHelper(node->right); // searching for the smallest immediate successor
            node->key = succ->key;
            node->value = succ->value;
            node->right = this->removeAVL(node->right, succ->key);
        }
        else if (node->left)
        {
            shared_ptr<BSTNode> temp = node->left;
            node.reset();
            node = temp;
        }
        else if (node->right)
        {
            shared_ptr<BSTNode> temp = node->right;
            node.reset();
            node = temp;
        }
        else
        {
            node.reset();
            return nullptr;
        }
    }
    else if (this->comparator(key, node->key))
    {
        node->left = this->removeAVL(node->left, key);
        if(node->left)
        {
            node->left->parent = node;
        }
    }
    else
    {
        node->right = this->removeAVL(node->right, key);
        if(node->right)
        {
            node->right->parent = node;
        }
    }

    this->updateHeight(node);
    return this->rebalance(node);
}

template<typename K, typename V>
shared_ptr<typename AVLTree<K, V>::BSTNode> AVLTree<K, V>::rotateLeft(shared_ptr<BSTNode> node) {
    if(node == nullptr)
    {
        return node;
    }
    shared_ptr<BSTNode> node_r = node->right;
    shared_ptr<BSTNode> node_r_l = node_r->left;
    node_r->left = node;
    node->right = node_r_l;
    this->updateHeight(node);
    this->updateHeight(node_r);
    return node_r;
}

template<typename K, typename V>
shared_ptr<typename AVLTree<K, V>::BSTNode> AVLTree<K, V>::rotateRight(shared_ptr<BSTNode> node) {
    if(node == nullptr)
    {
        return node;
    }
    shared_ptr<BSTNode> node_l = node->left;
    shared_ptr<BSTNode> node_l_r = node_l->right;
    node_l->right = node;
    node->left = node_l_r;
    this->updateHeight(node);
    this->updateHeight(node_l);
    return node_l;
}

template<typename K, typename V>
shared_ptr<typename AVLTree<K, V>::BSTNode> AVLTree<K, V>::rotateLeftRight(shared_ptr<BSTNode> node) {
    if(node == nullptr)
    {
        return node;
    }
    node->left = this->rotateLeft(node->left);
    node = rotateRight(node);
    return node;
}

template<typename K, typename V>
shared_ptr<typename AVLTree<K, V>::BSTNode> AVLTree<K, V>::rotateRightLeft(shared_ptr<BSTNode> node) {
    if(node == nullptr)
    {
        return node;
    }
    node->right = this->rotateRight(node->right);
    node = rotateLeft(node);
    return node;
}

template<typename K, typename V>
int AVLTree<K, V>::getBalanceFactor(shared_ptr<BSTNode> node) const {
    if(node == nullptr)
    {
        return 0;
    }
    int right = this->getHeight(node->right);
    int left = this->getHeight(node->left);
    return left - right;
}

template<typename K, typename V>
shared_ptr<typename AVLTree<K, V>::BSTNode> AVLTree<K, V>::rebalance(shared_ptr<BSTNode> node) {
    int bf = this->getBalanceFactor(node);
    if(bf > 1)
    {
        int bf_l = this->getBalanceFactor(node->left);
        if(bf_l >= 0)
        {
            return this->rotateRight(node);
        }
        else
        {
            return this->rotateLeftRight(node);
        }
    }
    else if(bf < -1)
    {
        int bf_r = this->getBalanceFactor(node->right);
        if(bf_r <= 0)
        {
            return this->rotateLeft(node);
        }
        else
        {
            return this->rotateRightLeft(node);
        }
    }
    return node;
}

template<typename K, typename V>
bool AVLTree<K, V>::isBalanced() const {
    int bf = this->getBalanceFactor(this->root);
    if(bf <= 1 && bf >= -1)
    {
        return true;
    }
    else 
    {
        return false;
    }
}

template<typename K, typename V>
bool AVLTree<K, V>::isValidAVLHelper(shared_ptr<BSTNode> node) const {
    return false;
}

template<typename K, typename V>
int AVLTree<K, V>::getMaxDepth() const {
    // shared_ptr<BSTNode> temp = this->root;
    // int height = 0;
    // while(temp)
    // {
    //     if(temp->left);
    // }
    return 0;
}

template<typename K, typename V>
double AVLTree<K, V>::getAverageDepth() const {
    return 0;
}

template<typename K, typename V>
void AVLTree<K, V>::calculateDepthStats(shared_ptr<BSTNode> node, int depth, int& totalDepth, int& nodeCount, int& maxDepth) const {
}

template<typename K, typename V>
bool AVLTree<K, V>::isValidAVL() const {
    return false;
}

template class AVLTree<int, string>;
template class AVLTree<string, string>;
template class AVLTree<int, int>;
template class AVLTree<string, void*>;