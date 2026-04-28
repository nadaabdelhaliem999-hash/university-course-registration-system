#include <iostream>
#include <vector>
#include <string>
#include "user.h"

using namespace std;

int main() {
    // ─── Data Storage ─────────────────────
    vector<User>   users;
    vector<Course> courses;

    insertSampleCourses(courses);

    int choice;
    cout << "==========================================\n";
    cout << "  University Course Registration System  \n";
    cout << "==========================================\n";

    // FIX: switch cases now only call handler functions.
    // Nesting depth inside main() never exceeds:
    //   while(true) → switch → (nothing more here)  =  depth 2  ✅
    while (true) {                        // depth 1
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. View all courses\n";
        cout << "4. Search courses\n";
        cout << "5. Exit\n";
        cout << "Enter your choice (1-5): ";
        cin >> choice;
        cin.ignore();

        switch (choice) {                 // depth 2
            case 1:
                handleRegister(users);
                break;
            case 2:
                handleLogin(users);
                break;
            case 3:
                handleViewCourses(courses);
                break;
            case 4:
                handleSearchCourses(courses);
                break;
            case 5:
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "ERROR: Please enter 1 to 5!\n";
        }
    }

    return 0;
}
