#include <iostream>
#include <vector>
#include "user.h"
using namespace std;

int main() {
    vector<User> users;

    // FIXED: separate declarations
    string email;
    string password;

    cout << "=== Registration ===\n";
    cout << "Enter email: ";
    cin >> email;

    if (!isValidEmail(email)) {
        cout << " Invalid email\n";
        return 0;
    }

    cout << "Enter password: ";
    cin >> password;

    if (!isValidPassword(password)) {
        cout << " Password must be at least 6 characters\n";
        return 0;
    }

    users.push_back({email, password});
    cout << " Registered successfully\n";

    return 0;
}
