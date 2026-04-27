#ifndef USER_H
#define USER_H

#include <string>
#include <string_view>  

struct User {
    std::string email;
    std::string password;
};

// changed to string_view
bool isValidEmail(std::string_view email);
bool isValidPassword(std::string_view password);

#endif
