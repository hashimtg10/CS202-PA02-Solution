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

template<typename K, typename V>
bool AVLTree<K, V>::insert(const K& key, const V& value) {
    return false;
}

template<typename K, typename V>
shared_ptr<typename AVLTree<K, V>::BSTNode> AVLTree<K, V>::insertAVL(shared_ptr<BSTNode> node, const K& key, const V& value) {
    return nullptr;
}

template<typename K, typename V>
bool AVLTree<K, V>::remove(const K& key) {
    return false;
}

template<typename K, typename V>
shared_ptr<typename AVLTree<K, V>::BSTNode> AVLTree<K, V>::removeAVL(shared_ptr<BSTNode> node, const K& key) {
    return nullptr;
}

template<typename K, typename V>
shared_ptr<typename AVLTree<K, V>::BSTNode> AVLTree<K, V>::rotateLeft(shared_ptr<BSTNode> node) {
    return nullptr;
}

template<typename K, typename V>
shared_ptr<typename AVLTree<K, V>::BSTNode> AVLTree<K, V>::rotateRight(shared_ptr<BSTNode> node) {
    return nullptr;
}

template<typename K, typename V>
shared_ptr<typename AVLTree<K, V>::BSTNode> AVLTree<K, V>::rotateLeftRight(shared_ptr<BSTNode> node) {
    return nullptr;
}

template<typename K, typename V>
shared_ptr<typename AVLTree<K, V>::BSTNode> AVLTree<K, V>::rotateRightLeft(shared_ptr<BSTNode> node) {
    return nullptr;
}

template<typename K, typename V>
int AVLTree<K, V>::getBalanceFactor(shared_ptr<BSTNode> node) const {
    return 0;
}

template<typename K, typename V>
shared_ptr<typename AVLTree<K, V>::BSTNode> AVLTree<K, V>::rebalance(shared_ptr<BSTNode> node) {
    return nullptr;
}

template<typename K, typename V>
bool AVLTree<K, V>::isBalanced() const {
    return false;
}

template<typename K, typename V>
bool AVLTree<K, V>::isValidAVLHelper(shared_ptr<BSTNode> node) const {
    return false;
}

template<typename K, typename V>
int AVLTree<K, V>::getMaxDepth() const {
    
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