#include "../headers/category_tree.h"
#include "../headers/post.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <climits>
#include <functional>
using namespace std;

CategoryNode::CategoryNode(const string &name)
    : categoryName(name), totalPostCount(0)
{
}

void CategoryNode::addChild(shared_ptr<CategoryNode> child)
{
    if (!child)
    {
        return;
    }
    child->parent = shared_from_this();
    children.push_back(child);
}

bool CategoryNode::removeChild(const string &childName)
{
    for (int i = 0; i < this->children.size(); i++)
    {
        if (this->children[i]->categoryName == childName)
        {
            for (int j = i; j < this->children.size() - 1; j++)
            {
                shared_ptr<CategoryNode> temp = this->children[j];
                this->children[j] = this->children[j + 1];
                temp->parent.reset();
            }
            this->children.pop_back();
            return true;
        }
    }
    return false;
}

shared_ptr<CategoryNode> CategoryNode::findChild(const string &childName) const
{
    for (auto child : children)
    {
        if (child->categoryName == childName)
        {
            return child;
        }
    }
    return nullptr;
}

void CategoryNode::addPost(Post *post)
{
    if (!post)
    {
        return;
    }
    this->posts.push_back(post);
    this->totalPostCount++;
    if (this->parent.lock())
    {
        shared_ptr<CategoryNode> temp = this->parent.lock();
        while (temp)
        {
            temp->updatePostCounts();
            if (temp->parent.lock())
            {
                temp = temp->parent.lock();
            }
        }
    }
}

bool CategoryNode::removePost(Post *post)
{
    for (int i = 0; i < this->posts.size(); i++)
    {
        if(this->posts[i]->postID == post->postID)
        {
            this->posts.erase(this->posts.begin() + i);
            return true;
        }
    }
    return false;
}

void CategoryNode::updatePostCounts()
{
    int tot = 0;
    for (auto child : this->children)
    {
        tot += child->totalPostCount;
    }
    this->totalPostCount += tot;
}

CategoryTree::CategoryTree()
{
    root = make_shared<CategoryNode>("root");
}

bool CategoryTree::addCategory(const string &categoryPath)
{
    return false;
}

bool CategoryTree::removeCategory(const string &categoryPath)
{
    return false;
}

bool CategoryTree::moveCategory(const string &fromPath, const string &toPath)
{
    return false;
}

shared_ptr<CategoryNode> CategoryTree::findCategory(const string &categoryPath) const
{
    return nullptr;
}

void CategoryTree::addPost(Post *post)
{
}

bool CategoryTree::removePost(Post *post)
{
    return false;
}

vector<Post *> CategoryTree::getPostsInCategory(const string &categoryPath, bool includeSubcategories) const
{
    vector<Post *> p;
    return p;
}

void CategoryTree::displayTree() const
{
}

CategoryTree::PreOrderIterator::PreOrderIterator(shared_ptr<CategoryNode> root)
{
}

CategoryTree::PreOrderIterator::PreOrderIterator() : current(nullptr)
{
}

shared_ptr<CategoryNode> CategoryTree::PreOrderIterator::operator*() const
{
    return nullptr;
}

CategoryTree::PreOrderIterator &CategoryTree::PreOrderIterator::operator++()
{
    CategoryTree::PreOrderIterator a;
}

bool CategoryTree::PreOrderIterator::operator!=(const PreOrderIterator &other) const
{
    return false;
}

bool CategoryTree::PreOrderIterator::operator==(const PreOrderIterator &other) const
{
    return false;
}

CategoryTree::PostOrderIterator::PostOrderIterator(shared_ptr<CategoryNode> root)
    : current(nullptr), lastVisited(nullptr)
{
}

CategoryTree::PostOrderIterator::PostOrderIterator()
    : current(nullptr), lastVisited(nullptr)
{
}

shared_ptr<CategoryNode> CategoryTree::PostOrderIterator::operator*() const
{
    return nullptr;
}

CategoryTree::PostOrderIterator &CategoryTree::PostOrderIterator::operator++()
{
    CategoryTree::PostOrderIterator a;
    return a;
}

bool CategoryTree::PostOrderIterator::operator!=(const PostOrderIterator &other) const
{
    return false;
}

bool CategoryTree::PostOrderIterator::operator==(const PostOrderIterator &other) const
{
    return false;
}

CategoryTree::BreadthFirstIterator::BreadthFirstIterator(shared_ptr<CategoryNode> root)
    : currentIndex(0)
{
}

CategoryTree::BreadthFirstIterator::BreadthFirstIterator() : currentIndex(SIZE_MAX)
{
}

shared_ptr<CategoryNode> CategoryTree::BreadthFirstIterator::operator*() const
{
    return nullptr;
}

CategoryTree::BreadthFirstIterator &CategoryTree::BreadthFirstIterator::operator++()
{
    CategoryTree::BreadthFirstIterator a;
    return a;
}

bool CategoryTree::BreadthFirstIterator::operator!=(const BreadthFirstIterator &other) const
{
    return false;
}

bool CategoryTree::BreadthFirstIterator::operator==(const BreadthFirstIterator &other) const
{
    return false;
}

CategoryTree::PreOrderIterator CategoryTree::preOrderBegin() const
{
    CategoryTree::PreOrderIterator a;
    return a;
}

CategoryTree::PreOrderIterator CategoryTree::preOrderEnd() const
{
    CategoryTree::PreOrderIterator a;
    return a;
}

CategoryTree::PostOrderIterator CategoryTree::postOrderBegin() const
{
    CategoryTree::PostOrderIterator a;
    return a;
}

CategoryTree::PostOrderIterator CategoryTree::postOrderEnd() const
{
    CategoryTree::PostOrderIterator a;
    return a;
}

CategoryTree::BreadthFirstIterator CategoryTree::breadthFirstBegin() const
{
    CategoryTree::BreadthFirstIterator a;
    return a;
}

CategoryTree::BreadthFirstIterator CategoryTree::breadthFirstEnd() const
{
    CategoryTree::BreadthFirstIterator a;
    return a;
}