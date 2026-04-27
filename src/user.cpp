#include "user.h"

bool isValidEmail(string email) {
    return email.find('@') != string::npos;
}

bool isValidPassword(string password) {
    return password.length() >= 6;
}
