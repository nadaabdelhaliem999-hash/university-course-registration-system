#include <cassert>
#include <string>
using namespace std;

bool isValidEmail(string email) {
    return email.find('@') != string::npos;
}

bool isValidPassword(string password) {
    return password.length() >= 6;
}

int main() {
    assert(isValidEmail("test@gmail.com") == true);
    assert(isValidEmail("wrongemail") == false);

    assert(isValidPassword("123456") == true);
    assert(isValidPassword("123") == false);

    return 0;
}
