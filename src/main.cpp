#include <iostream>
#include <vector>
#include <string>
#include "user.h"

using namespace std;

int main() {
    vector<User>   users;
    vector<Course> courses;
    string loggedInEmail = "";

    insertSampleCourses(courses);

    int choice;
    cout << "==========================================\n";
    cout << "  University Course Registration System  \n";
    cout << "==========================================\n";

    while (true) {
        cout << "\n========== MAIN MENU ==========\n";
        if (loggedInEmail.empty()) {
            cout << "1. Register\n";
            cout << "2. Login\n";
            cout << "3. View all courses\n";
            cout << "4. Search courses\n";
            cout << "5. Exit\n";
        } else {
            cout << "Logged in as: " << loggedInEmail << "\n";
            cout << "1. View all courses\n";
            cout << "2. Search courses\n";
            cout << "3. Register for a course\n";
            cout << "4. Drop a course\n";
            cout << "5. View my schedule\n";
            cout << "6. Logout\n";
            cout << "7. Exit\n";
        }
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        bool continueLoop;
        if (loggedInEmail.empty()) {
            continueLoop = handleGuestMenu(choice, users, courses, loggedInEmail);
        } else {
            continueLoop = handleLoggedInMenu(choice, users, courses, loggedInEmail);
        }
        if (!continueLoop) {
            return 0;
        }
    }
}
