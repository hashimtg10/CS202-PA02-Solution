#include "../headers/category_tree.h"
#include "../headers/post.h"
#include <iostream>
#include <queue>
#include <stack>
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
    for (size_t i = 0; i < this->children.size(); i++)
    {
        if (this->children[i]->categoryName == childName)
        {
            weak_ptr<CategoryNode> temp = this->children[i]->parent.lock();
            this->children.erase(this->children.begin() + i);
            while (temp.lock())
            {
                temp.lock()->updatePostCounts();
                temp = temp.lock()->parent;
            }
            return true;
        }
    }
    return false;
}

shared_ptr<CategoryNode> CategoryNode::findChild(const string &childName) const
{
    for (size_t i = 0; i < this->children.size(); i++)
    {
        if (this->children[i]->categoryName == childName)
        {
            return this->children[i];
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
        weak_ptr<CategoryNode> temp = this->parent.lock();
        while (temp.lock())
        {
            temp.lock()->updatePostCounts();
            temp = temp.lock()->parent.lock();
        }
    }
}

bool CategoryNode::removePost(Post *post)
{
    for (size_t i = 0; i < this->posts.size(); i++)
    {
        if (this->posts[i]->postID == post->postID)
        {
            this->posts.erase(this->posts.begin() + i);
            this->totalPostCount--;
            if (this->parent.lock())
            {
                shared_ptr<CategoryNode> temp = this->parent.lock();
                while (temp)
                {
                    temp->updatePostCounts();
                    temp = temp->parent.lock();
                }
            }
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
    tot += this->posts.size();
    this->totalPostCount = tot;
}

CategoryTree::CategoryTree()
{
    root = make_shared<CategoryNode>("root");
}

bool CategoryTree::addCategory(const string &categoryPath)
{
    vector<string> categories = this->parseCategoryPath(categoryPath);
    if (categories.empty())
    {
        return false;
    }
    vector<string> added_categories;
    size_t created = 0;
    shared_ptr<CategoryNode> temp = this->root;
    for (auto categ : categories)
    {
        if (temp->findChild(categ))
        {
            temp = temp->findChild(categ);
        }
        else
        {
            temp->addChild(make_shared<CategoryNode>(categ));
            temp = temp->findChild(categ);
            created++;
        }
    }
    if (created == categories.size())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CategoryTree::removeCategory(const string &categoryPath)
{
    vector<string> categories = this->parseCategoryPath(categoryPath);
    if (categories.empty())
    {
        return false;
    }
    shared_ptr<CategoryNode> temp = this->root;
    for (auto categ : categories)
    {
        if (temp->findChild(categ))
        {
            if (categ == categories.back())
            {
                temp->removeChild(categ);
                return true;
            }
            else
            {
                temp = temp->findChild(categ);
            }
        }
        else
        {
            break;
        }
    }
    return false;
}

bool CategoryTree::moveCategory(const string &fromPath, const string &toPath)
{
    vector<string> from_categories = this->parseCategoryPath(fromPath);
    vector<string> to_categories = this->parseCategoryPath(toPath);
    shared_ptr<CategoryNode> from = nullptr;
    if (from_categories.size() <= 0)
    {
        return false;
    }
    else
    {
        from = this->findCategory(fromPath);
    }
    if (from)
    {
        shared_ptr<CategoryNode> to  = nullptr;
        if(to_categories.size() <= 0)
        {
            to = this->root;
        }
        else
        {
            to = this->findCategory(toPath);
        }
        if (!to)
        {
            this->addCategory(toPath);
            to = this->findCategory(toPath);
        }
        weak_ptr<CategoryNode> updater = from->parent.lock();
        if (this->removeCategory(fromPath))
        {
            to->children.push_back(from);
            to->updatePostCounts();
            from->parent = to;
            while (updater.lock())
            {
                updater.lock()->updatePostCounts();
                updater = updater.lock()->parent;
            }
            updater = to->parent.lock();
            while (updater.lock())
            {
                updater.lock()->updatePostCounts();
                updater = updater.lock()->parent;
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

shared_ptr<CategoryNode> CategoryTree::findCategory(const string &categoryPath) const
{
    vector<string> categories = this->parseCategoryPath(categoryPath);
    if (categories.empty())
    {
        return nullptr;
    }
    shared_ptr<CategoryNode> temp = this->root;
    for (auto categ : categories)
    {
        if (temp->findChild(categ))
        {
            temp = temp->findChild(categ);
        }
        else
        {
            return nullptr;
        }
    }
    return temp;
}

void CategoryTree::addPost(Post *post)
{
    if (!post)
    {
        return;
    }
    else
    {
        string category = post->category;
        shared_ptr<CategoryNode> categ = this->findCategory(category);
        if (categ)
        {
            categ->addPost(post);
        }
        else
        {
            this->addCategory(category);
            categ = this->findCategory(category);
            if (categ)
            {
                categ->addPost(post);
            }
        }
    }
}

bool CategoryTree::removePost(Post *post)
{
    if (!post)
    {
        return false;
    }
    else
    {
        string category = post->category;
        shared_ptr<CategoryNode> categ = this->findCategory(category);
        if (categ)
        {
            return categ->removePost(post);
        }
        return false;
    }
}

vector<Post *> CategoryTree::getPostsInCategory(const string &categoryPath, bool includeSubcategories) const
{
    // Upon forgetting the code for level order traversal given lack of revision due to mid week, I took help from geeks from geeks in recalling the algorithm
    // Only the macro algorithm for the level order traversal was seen though. Everything else was done by my self.
    vector<string> categories = this->parseCategoryPath(categoryPath);
    if (categories.empty())
    {
        return {};
    }
    else
    {
        shared_ptr<CategoryNode> temp = this->findCategory(categoryPath);
        if (!temp)
        {
            return {};
        }
        vector<Post *> posts;
        if (!temp)
        {
            return {};
        }
        if (includeSubcategories)
        {
            queue<shared_ptr<CategoryNode>> q;
            q.push(temp);
            vector<vector<Post *>> postings;

            while (!q.empty())
            {
                size_t len = q.size();
                for (size_t i = 0; i < len; i++)
                {
                    shared_ptr<CategoryNode> temp = q.front();
                    q.pop();

                    postings.push_back(temp->posts);
                    if (!temp->children.empty())
                    {
                        for (size_t i = 0; i < temp->children.size(); i++)
                        {
                            q.push(temp->children[i]);
                        }
                    }
                }
            }
            for (size_t i = 0; i < postings.size(); i++)
            {
                for (size_t j = 0; j < postings[i].size(); j++)
                {
                    posts.push_back(postings[i][j]);
                }
            }
        }
        else
        {
            for (size_t i = 0; i < temp->posts.size(); i++)
            {
                posts.push_back(temp->posts[i]);
            }
        }
        return posts;
    }
}

void CategoryTree::displayTree() const
{
    PostOrderIterator it(this->root);
    while (*it)
    {
        std::cout << (*it)->categoryName << std::endl;
        ++it;
    }
}
/*Following is the query I passed to Grok for the iterators portion:
"to implement an iterator, let me know what are the function of these. donot tell even a single line of code"
With this query, i passed the definitions of the operator*, operator++, operator!=, operator== as well
The response I got was extremely generic, but what it clarified was how to keep track of the current iterator.
*/
CategoryTree::PreOrderIterator::PreOrderIterator(shared_ptr<CategoryNode> root)
{
    // I had understood and memorized this code for the midterm from a youtube channel called strivers. There was two questions from this topic (inorder iterative traversal) that appeared in the exam which I blindly got right
    std::stack<shared_ptr<CategoryNode>> st;
    st.push(root);
    while (!st.empty())
    {
        shared_ptr<CategoryNode> temp = st.top();
        st.pop();
        if (!temp->children.empty())
        {
            for (int i = temp->children.size() - 1; i >= 0; i--)
            {
                st.push(temp->children[i]);
            }
        }
        this->stack.push_back(temp);
    }
    if (this->stack.size() >= 1)
    {
        this->current = this->stack[0];
    }
    else
    {
        this->current = nullptr;
    }
}

CategoryTree::PreOrderIterator::PreOrderIterator() : current(nullptr)
{
}

shared_ptr<CategoryNode> CategoryTree::PreOrderIterator::operator*() const
{
    return this->current;
}

CategoryTree::PreOrderIterator &CategoryTree::PreOrderIterator::operator++()
{
    for (size_t i = 0; i < this->stack.size(); i++)
    {
        if (this->stack[i]->categoryName == this->current->categoryName)
        {
            if (i < this->stack.size() - 1)
            {
                this->current = this->stack[i + 1];
                return *this;
            }
            else
            {
                this->current = nullptr;
                return *this;
            }
        }
    }
    return *this;
}

bool CategoryTree::PreOrderIterator::operator!=(const PreOrderIterator &other) const
{
    if (this->current == other.current)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool CategoryTree::PreOrderIterator::operator==(const PreOrderIterator &other) const
{
    if (this->current == other.current)
    {
        return true;
    }
    else
    {
        return false;
    }
}

CategoryTree::PostOrderIterator::PostOrderIterator(shared_ptr<CategoryNode> root)
    : current(nullptr), lastVisited(nullptr)
{
    // I had understood and memorized this code for the midterm from a youtube channel called strivers. There was two questions from this topic (inorder iterative traversal) that appeared in the exam which I blindly got right
    std::stack<shared_ptr<CategoryNode>> st1;
    std::stack<shared_ptr<CategoryNode>> st2;
    st1.push(root);
    while (!st1.empty())
    {
        shared_ptr<CategoryNode> temp = st1.top();
        st1.pop();
        if (!temp->children.empty())
        {
            for (size_t i = 0; i < temp->children.size(); i++)
            {
                st1.push(temp->children[i]);
            }
        }
        st2.push(temp);
    }
    while (!st2.empty())
    {
        this->stack.push_back(st2.top());
        st2.pop();
    }
    if (this->stack.size() >= 1)
    {
        this->current = this->stack[0];
        this->lastVisited = nullptr;
    }
    else
    {
        this->current = nullptr;
        this->lastVisited = nullptr;
    }
}

CategoryTree::PostOrderIterator::PostOrderIterator()
    : current(nullptr), lastVisited(nullptr)
{
}

shared_ptr<CategoryNode> CategoryTree::PostOrderIterator::operator*() const
{
    return this->current;
}

CategoryTree::PostOrderIterator &CategoryTree::PostOrderIterator::operator++()
{
    if (this->current == nullptr)
    {
        return *this;
    }
    for (size_t i = 0; i < this->stack.size(); i++)
    {
        if (this->stack[i]->categoryName == this->current->categoryName)
        {
            if (i < this->stack.size() - 1)
            {
                this->current = this->stack[i + 1];
                this->lastVisited = this->stack[i];
                return *this;
            }
            else
            {
                this->current = nullptr;
                this->lastVisited = this->stack[i];
                return *this;
            }
        }
    }
    return *this;
}

bool CategoryTree::PostOrderIterator::operator!=(const PostOrderIterator &other) const
{
    if (this->current == other.current)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool CategoryTree::PostOrderIterator::operator==(const PostOrderIterator &other) const
{
    if (this->current == other.current)
    {
        return true;
    }
    else
    {
        return false;
    }
}

CategoryTree::BreadthFirstIterator::BreadthFirstIterator(shared_ptr<CategoryNode> root)
    : currentIndex(0)
{
    std::queue<shared_ptr<CategoryNode>> q;
    q.push(root);
    while (!q.empty())
    {
        int len = q.size();
        for (int i = 0; i < len; i++)
        {
            shared_ptr<CategoryNode> temp = q.front();
            q.pop();
            if(!temp->children.empty())
            {
                for (size_t i = 0; i < temp->children.size(); i++)
                {
                    q.push(temp->children[i]);
                }
                
            }
            this->queue.push_back(temp);
        }
    }
}

CategoryTree::BreadthFirstIterator::BreadthFirstIterator() : currentIndex(SIZE_MAX)
{
}

shared_ptr<CategoryNode> CategoryTree::BreadthFirstIterator::operator*() const
{
    if (this->currentIndex >= 0 && this->currentIndex < this->queue.size())
    {
        return this->queue[this->currentIndex];
    }
    else
    {
        return nullptr;
    }
}

CategoryTree::BreadthFirstIterator &CategoryTree::BreadthFirstIterator::operator++()
{
    if (this->currentIndex < 0 || this->currentIndex >= this->queue.size())
    {
        return *this;
    }
    else if (this->currentIndex < this->queue.size() - 1)
    {
        this->currentIndex++;
        return *this;
    }
    else
    {
        this->currentIndex = -1;
        return *this;
    }
}

bool CategoryTree::BreadthFirstIterator::operator!=(const BreadthFirstIterator &other) const
{
    if (this->currentIndex == other.currentIndex)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool CategoryTree::BreadthFirstIterator::operator==(const BreadthFirstIterator &other) const
{
    if (this->currentIndex == other.currentIndex)
    {
        return true;
    }
    else
    {
        return false;
    }
}

CategoryTree::PreOrderIterator CategoryTree::preOrderBegin() const
{
    PreOrderIterator it(this->root);
    return it;
}

CategoryTree::PreOrderIterator CategoryTree::preOrderEnd() const
{
    PreOrderIterator it;
    return it;
}

CategoryTree::PostOrderIterator CategoryTree::postOrderBegin() const
{
    PostOrderIterator it(this->root);
    return it;
}

CategoryTree::PostOrderIterator CategoryTree::postOrderEnd() const
{
    PostOrderIterator it;
    return it;
}

CategoryTree::BreadthFirstIterator CategoryTree::breadthFirstBegin() const
{
    BreadthFirstIterator it(this->root);
    return it;
}

CategoryTree::BreadthFirstIterator CategoryTree::breadthFirstEnd() const
{
    BreadthFirstIterator it;
    return it;
}