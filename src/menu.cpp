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
                        const vector<Course>& courses,
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
