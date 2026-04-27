#include "user.h"

// FIXED passing by reference
bool isValidEmail(const string& email) {
    return email.find('@') != string::npos;
}

// FIXED
bool isValidPassword(const string& password) {
    return password.length() >= 6;
}
