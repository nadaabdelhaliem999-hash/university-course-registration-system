#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

struct User {
    string email;
    string password;
};

// FIXED: pass by const reference
bool isValidEmail(const string& email);
bool isValidPassword(const string& password);

#endif
