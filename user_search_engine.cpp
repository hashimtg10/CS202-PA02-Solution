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
}

bool UserSearchEngine::addUser(User *user)
{
  return false;
}

bool UserSearchEngine::removeUser(int userID)
{
  return false;
}

bool UserSearchEngine::removeUser(const string &username)
{
  return false;
}

User *UserSearchEngine::searchByID(int userID) const
{
  return nullptr;
}

User *UserSearchEngine::searchByUsername(const std::string &username) const
{
  return nullptr;
}

std::vector<User *> UserSearchEngine::searchByUsernamePrefix(const string &prefix) const
{
  vector<User *> vect;
  return vect;
}

std::vector<User *> UserSearchEngine::getUsersInIDRange(int minID, int maxID) const
{
  vector<User *> vect;
  return vect;
}

std::vector<User *> UserSearchEngine::fuzzyUsernameSearch(const string &username, int maxEditDistance) const
{
  vector<User *> vect;
  return vect;
}

int UserSearchEngine::calculateEditDistance(const string &str1, const string &str2) const
{
  return 0;
}

vector<User *> UserSearchEngine::getAllUsersSorted(bool byID) const
{
  vector<User*> vect;
  return vect;
}

size_t UserSearchEngine::getTotalUsers() const
{
  return 0;
}

void UserSearchEngine::displaySearchStats() const
{
}

bool UserSearchEngine::isConsistent() const
{
  return false;
}