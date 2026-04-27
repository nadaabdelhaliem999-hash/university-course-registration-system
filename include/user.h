#ifndef USER_H
#define USER_H
#include <string>

// removed: using namespace std;

struct User {
    std::string email;
    std::string password;
};

// Also fix here (combine with previous issue)
bool isValidEmail(const std::string& email);
bool isValidPassword(const std::string& password);

#endif
