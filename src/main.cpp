#include <iostream>
#include <vector>
#include <string>
#include "user.h"

using namespace std;

int main() {
    // ─── Data Storage ─────────────────────
    vector<User>   users;
    vector<Course> courses;

    // Load sample courses at startup
    insertSampleCourses(courses);

    int choice;

    cout << "==========================================\n";
    cout << "  University Course Registration System  \n";
    cout << "==========================================\n";

    while (true) {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. View all courses\n";
        cout << "4. Search courses\n";
        cout << "5. Exit\n";
        cout << "Enter your choice (1-5): ";

        cin >> choice;
        cin.ignore();

        switch (choice) {

            // ─── Sprint 1: Register ───────
            case 1: {
                string email;
                string password;

                cout << "\n========== REGISTER ==========\n";
                cout << "Enter email: ";
                getline(cin, email);

                if (!isValidEmail(email)) {
                    cout << "ERROR: Invalid email!\n";
                    break;
                }

                cout << "Enter password (min 6 characters): ";
                getline(cin, password);

                if (!isValidPassword(password)) {
                    cout << "ERROR: Password must be at least 6 characters!\n";
                    break;
                }

                // Check if email already exists
                bool alreadyExists = false;
                for (const User& user : users) {
                    if (user.email == email) {
                        alreadyExists = true;
                        break;
                    }
                }

                if (alreadyExists) {
                    cout << "ERROR: Email already registered!\n";
                } else {
                    users.push_back({email, password});
                    cout << "SUCCESS: Registered successfully!\n";
                }
                break;
            }

            // ─── Sprint 1: Login ──────────
            case 2: {
                string email;
                string password;

                cout << "\n========== LOGIN ==========\n";
                cout << "Enter email: ";
                getline(cin, email);

                cout << "Enter password: ";
                getline(cin, password);

                bool loggedIn = false;
                for (const User& user : users) {
                    if (user.email == email && user.password == password) {
                        loggedIn = true;
                        break;
                    }
                }

                if (loggedIn) {
                    cout << "SUCCESS: Welcome back " << email << "!\n";
                } else {
                    cout << "ERROR: Wrong email or password!\n";
                }
                break;
            }

            // ─── Sprint 2: View Courses ───
            case 3: {
                displayCourses(courses);
                break;
            }

            // ─── Sprint 2: Search Courses ─
            case 4: {
                string keyword;
                cout << "\nEnter search keyword: ";
                getline(cin, keyword);

                if (keyword.empty()) {
                    cout << "ERROR: Please enter a keyword!\n";
                } else {
                    searchCourses(courses, keyword);
                }
                break;
            }

            case 5: {
                cout << "Goodbye!\n";
                return 0;
            }

            default: {
                cout << "ERROR: Please enter 1 to 5!\n";
            }
        }
    }

    return 0;
}
