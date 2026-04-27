#include "user.h"
#include <string_view>

//use string_view
bool isValidEmail(std::string_view email) {
    return email.find('@') != std::string_view::npos;
}

bool isValidPassword(std::string_view password) {
    return password.length() >= 6;
}
