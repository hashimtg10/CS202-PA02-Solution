#include "../headers/user_search_engine.h"
#include "../headers/user_manager.h"
#include <algorithm>
#include <iostream>
using namespace std;

UserSearchEngine::UserSearchEngine()
    : usersByID([](const int &a, const int &b)
                { return a < b; }),
      usersByName([](const string &a, const string &b)
                  { return a < b; })
{
}

UserSearchEngine::~UserSearchEngine()
{
}

void UserSearchEngine::migrateFromLinkedList(const LinkedList<User> &userList)
{
  auto temp = userList.head();
  while (temp)
  {
    this->addUser(&temp->data);
    temp = temp->next;
  }
}

bool UserSearchEngine::addUser(User *user)
{
  if (user == nullptr)
  {
    return false;
  }
  bool ins_id = false;
  bool ins_na = false;
  ins_id = usersByID.insert(user->userID, user);
  if (ins_id)
  {
    ins_na = usersByName.insert(user->userName, user);
    if (ins_na)
    {
      return true;
    }
    else
    {
      usersByID.remove(user->userID);
      return false;
    }
  }
  else
  {
    return false;
  }
}

bool UserSearchEngine::removeUser(int userID)
{
  User *us = this->searchByID(userID);
  if (!us || us->userID != userID)
  {
    return false;
  }
  bool rem_id = false;
  bool rem_na = false;
  rem_id = usersByID.remove(userID);
  if (rem_id)
  {
    rem_na = usersByName.remove(us->userName);
    if (rem_na)
    {
      return true;
    }
    else
    {
      usersByID.insert(userID, us);
      return false;
    }
  }
  else
  {
    return false;
  }
}

bool UserSearchEngine::removeUser(const string &username)
{
  User *us = this->searchByUsername(username);
  if (!us || us->userName != username)
  {
    return false;
  }
  bool rem_na = false;
  bool rem_id = false;
  rem_na = usersByName.remove(us->userName);
  if (rem_na)
  {
    rem_id = usersByID.remove(us->userID);
    if (rem_id)
    {
      return true;
    }
    else
    {
      usersByName.insert(username, us);
      return false;
    }
  }
  else
  {
    return false;
  }
}

User *UserSearchEngine::searchByID(int userID) const
{
  User *const *temp = usersByID.find(userID);
  if (temp && *temp)
  {
    return *temp;
  }
  else
  {
    return nullptr;
  }
}

User *UserSearchEngine::searchByUsername(const std::string &username) const
{
  User *const *temp = usersByName.find(username);
  if (temp && *temp)
  {
    return *temp;
  }
  else
  {
    return nullptr;
  }
}

std::vector<User *> UserSearchEngine::searchByUsernamePrefix(const string &prefix) const
{
  int pre_len = prefix.length();
  vector<User *> res;
  vector<pair<string, User *>> lst = usersByName.inOrderTraversal();
  for (auto i : lst)
  {
    string na_sub = i.first.substr(0, pre_len);
    if (na_sub == prefix)
    {
      res.push_back(i.second);
    }
  }
  return res;
}

std::vector<User *> UserSearchEngine::getUsersInIDRange(int minID, int maxID) const
{
  vector<pair<int, User *>> vect = usersByID.findRange(minID, maxID);
  vector<User *> res;
  for (auto i : vect)
  {
    res.push_back(i.second);
  }
  return res;
}

std::vector<User *> UserSearchEngine::fuzzyUsernameSearch(const string &username, int maxEditDistance) const
{
  vector<User *> res;
  vector<pair<string, User *>> lst = usersByName.inOrderTraversal();
  for (auto i : lst)
  {
    if (this->calculateEditDistance(i.first, username) <= maxEditDistance)
    {
      res.push_back(i.second);
    }
  }
  std::cout << std::endl;
  for (auto i : res)
  {
    std::cout << "ID: " << i->userID << ", UserName: " << i->userName << " - ";
  }
  std::cout << std::endl;
  return res;
}
/* Gave grok the following query: "can you please elaborate in words only", and pasted the description of the function in the manual.pdf
 */
int UserSearchEngine::calculateEditDistance(const string &str1, const string &str2) const
{
  int dist = editDistanceHelper(str1, str2, 0, 0);
  std::cout << "Distance: " << dist << std::endl;
  return dist;
}
size_t UserSearchEngine::editDistanceHelper(const string &str1, const string &str2, size_t i1, size_t i2) const
{
  if (i1 >= str1.length() && i2 >= str2.length())
  {
    return 0;
  }
  if (i1 >= str1.length())
  {
    return str2.length() - (i2);
  }
  if (i2 >= str2.length())
  {
    return str1.length() - (i1);
  }
  int way1 = 0;
  int way2 = 0;
  int way3 = 0;
  if (str1[i1] == str2[i2])
  {
    way1 = editDistanceHelper(str1, str2, i1+1, i2+1);
    way2 = editDistanceHelper(str1, str2, i1+1, i2+1);
    way3 = editDistanceHelper(str1, str2, i1+1, i2+1);
  }
  else
  {
    way1 = 1 + editDistanceHelper(str1, str2, i1 + 1, i2 + 1);
    way2 = 1 + editDistanceHelper(str1, str2, i1 + 1, i2);
    way3 = 1 + editDistanceHelper(str1, str2, i1, i2 + 1);
  }

  if (way1 <= way2 && way1 <= way3)
  {
    return way1;
  }
  else if (way2 <= way1 && way2 <=   way3)
  {
    return way2;
  }
  else
  {
    return way3;
  }
}

vector<User *> UserSearchEngine::getAllUsersSorted(bool byID) const
{
  vector<pair<int, User *>> vect = usersByID.inOrderTraversal();
  vector<User *> res;
  for (auto i : vect)
  {
    res.push_back(i.second);
  }
  return res;
}

size_t UserSearchEngine::getTotalUsers() const
{
  size_t total = usersByID.size();
  return total;
}

void UserSearchEngine::displaySearchStats() const
{
}

bool UserSearchEngine::isConsistent() const
{
  size_t id_tot = usersByID.size();
  size_t na_tot = usersByName.size();
  if (id_tot == na_tot)
  {
    vector<pair<int, User *>> id_vec = usersByID.inOrderTraversal();
    for (auto i : id_vec)
    {
      User *const *found = usersByName.find(i.second->userName);
      if (found)
      {
        continue;
      }
      else
      {
        return false;
      }
    }
    return true;
  }
  else
  {
    return false;
  }
}