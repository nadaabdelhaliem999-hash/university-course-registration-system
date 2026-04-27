#ifndef USER_H
#define USER_H
#include <string>
using namespace std;

struct User {
    string email;
    string password;
};

bool isValidEmail(string email);
bool isValidPassword(string password);

#endif
