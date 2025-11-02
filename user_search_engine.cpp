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
  return res;
}
/* Gave grok the following query: "can you please elaborate in words only", and pasted the description of the function in the manual.pdf
 */
int UserSearchEngine::calculateEditDistance(const string &str1, const string &str2) const
{
  int dist = editDistanceHelper(str1, str2, 0, 0);
  return dist;
}
/*For this function, I only had half an hour to submit, and I was getting full marks in the test case (have that version saved on git and github), but new it is missing on an edge case. Therefore, this was the first time as far as I can recall that I exchanged code with an LLM to get debugged. The only problem was that I was out of time; I forwarded the issue to TA's as well but was not expecting a response that late.
*/
/*Following is the query I gave to grok and ChatGpt: can you just give me a minor hint  of where is the problem in this function i have written*/
/*Sadly, the issue remained, yet the test cases passed*/
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
  size_t way1 = 0;
  size_t way2 = 0;
  size_t way3 = 0;
  if (str1[i1] == str2[i2])
  {
    return editDistanceHelper(str1, str2, i1+1, i2+1);
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