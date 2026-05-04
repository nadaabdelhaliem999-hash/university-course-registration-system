#include "user.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool handleGuestMenu(int choice,
                     vector<User>& users,
                     const vector<Course>& courses,
                     string& loggedInEmail) {
    switch (choice) {
        case 1:
            handleRegister(users);
            return true;
        case 2:
            handleLogin(users, loggedInEmail);
            return true;
        case 3:
            handleViewCourses(courses);
            return true;
        case 4:
            handleSearchCourses(courses);
            return true;
        case 5:
            cout << "Goodbye!\n";
            return false;
        default:
            cout << "ERROR: Invalid choice!\n";
            return true;
    }
}

bool handleLoggedInMenu(int choice,
                        vector<User>& users,
                        vector<Course>& courses,
                        string& loggedInEmail) {
    switch (choice) {
        case 1:
            handleViewCourses(courses);
            return true;
        case 2:
            handleSearchCourses(courses);
            return true;
        case 3:
            handleRegisterForCourse(users, loggedInEmail, courses);
            return true;
        case 4:
            handleDropCourse(users, loggedInEmail, courses);
            return true;
        case 5:
            handleViewSchedule(users, loggedInEmail, courses);
            return true;
        case 6:
            cout << "Logged out successfully!\n";
            loggedInEmail = "";
            return true;
        case 7:
            cout << "Goodbye!\n";
            return false;
        default:
            cout << "ERROR: Invalid choice!\n";
            return true;
    }
}

void runApp(vector<User>& users, vector<Course>& courses) {
    string loggedInEmail = "";
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
            return;
        }
    }
}
