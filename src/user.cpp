#include "user.h"
#include <string>

// Check if email contains @ symbol
bool isValidEmail(const std::string& email) {
    return email.find('@') != std::string::npos;
}

// Check if password is at least 6 characters
bool isValidPassword(const std::string& password) {
    return password.length() >= 6;
}
