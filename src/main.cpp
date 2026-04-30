#include <iostream>
#include <vector>
#include <string>
#include "user.h"

using namespace std;

int main() {
    vector<User>   users;
    vector<Course> courses;
    string loggedInEmail = "";   // empty means no one is logged in

    insertSampleCourses(courses);

    int choice;
    cout << "==========================================\n";
    cout << "  University Course Registration System  \n";
    cout << "==========================================\n";

    while (true) {                        // depth 1
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

        if (loggedInEmail.empty()) {       // depth 2
            switch (choice) {              // depth 3 — max allowed ✅
                case 1:
                    handleRegister(users);
                    break;
                case 2:
                    handleLogin(users, loggedInEmail);
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
                    cout << "ERROR: Invalid choice!\n";
            }
            // FIX 3: removed the dead "return 0;" that was here.
            // (SonarQube: "'return' will never be executed")
            // while(true) never exits except via "return 0" inside case 5,
            // so any statement after the closing brace is unreachable dead code.
        } else {
            switch (choice) {              // depth 3 — max allowed ✅
                case 1:
                    handleViewCourses(courses);
                    break;
                case 2:
                    handleSearchCourses(courses);
                    break;
                case 3:
                    handleRegisterForCourse(users, loggedInEmail, courses);
                    break;
                case 4:
                    handleDropCourse(users, loggedInEmail, courses);
                    break;
                case 5:
                    handleViewSchedule(users, loggedInEmail, courses);
                    break;
                case 6:
                    cout << "Logged out successfully!\n";
                    loggedInEmail = "";
                    break;
                case 7:
                    cout << "Goodbye!\n";
                    return 0;
                default:
                    cout << "ERROR: Invalid choice!\n";
            }
        }
    }
}