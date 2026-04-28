#include "user.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

// ─── Sprint 1: Validation ─────────────────
bool isValidEmail(string_view email) {
    return email.find('@') != string_view::npos;
}

bool isValidPassword(string_view password) {
    return password.length() >= 6;
}

// ─── Sprint 1: Lookup helpers ─────────────
// FIX: Logic extracted from main() into standalone functions.
// This eliminates nesting violations AND makes the logic unit-testable.

bool emailAlreadyExists(const vector<User>& users, string_view email) {
    for (const User& user : users) {      // depth 1 inside function — fine
        if (user.email == email) {         // depth 2 — well within limit
            return true;
        }
    }
    return false;
}

bool authenticateUser(const vector<User>& users,
                      string_view email,
                      string_view password) {
    for (const User& user : users) {                                // depth 1
        if (user.email == email && user.password == password) {    // depth 2
            return true;
        }
    }
    return false;
}

// ─── Sprint 1: Action handlers ────────────
void handleRegister(vector<User>& users) {
    string email;
    string password;

    cout << "\n========== REGISTER ==========\n";
    cout << "Enter email: ";
    getline(cin, email);

    if (!isValidEmail(email)) {                // depth 1
        cout << "ERROR: Invalid email!\n";
        return;
    }

    cout << "Enter password (min 6 characters): ";
    getline(cin, password);

    if (!isValidPassword(password)) {          // depth 1
        cout << "ERROR: Password must be at least 6 characters!\n";
        return;
    }

    if (emailAlreadyExists(users, email)) {    // depth 1  — NO nested loop here
        cout << "ERROR: Email already registered!\n";
    } else {
        users.push_back({email, password});
        cout << "SUCCESS: Registered successfully!\n";
    }
}

void handleLogin(const vector<User>& users) {
    string email;
    string password;

    cout << "\n========== LOGIN ==========\n";
    cout << "Enter email: ";
    getline(cin, email);
    cout << "Enter password: ";
    getline(cin, password);

    // FIX: authentication delegated to helper — no loop nested inside switch inside while
    if (authenticateUser(users, email, password)) {   // depth 1
        cout << "SUCCESS: Welcome back " << email << "!\n";
    } else {
        cout << "ERROR: Wrong email or password!\n";
    }
}

// ─── Sprint 2: Course Functions ───────────
void insertSampleCourses(vector<Course>& courses) {
    courses.push_back({1, "Mathematics 101",      "Dr. Ahmed",   30, "Sun/Tue 9:00-10:30"});
    courses.push_back({2, "Computer Science 201", "Dr. Sara",    25, "Mon/Wed 11:00-12:30"});
    courses.push_back({3, "Physics 101",          "Dr. Mohamed", 30, "Sun/Tue 1:00-2:30"});
    courses.push_back({4, "English 101",          "Dr. Nour",    35, "Mon/Wed 9:00-10:30"});
    courses.push_back({5, "Chemistry 201",        "Dr. Layla",   25, "Sun/Tue 3:00-4:30"});
    courses.push_back({6, "History 101",          "Dr. Omar",    30, "Mon/Wed 1:00-2:30"});
    courses.push_back({7, "Biology 201",          "Dr. Hana",    25, "Sun/Tue 11:00-12:30"});
    courses.push_back({8, "Economics 101",        "Dr. Karim",   30, "Mon/Wed 3:00-4:30"});
}

void displayCourses(const vector<Course>& courses) {
    if (courses.empty()) {                   // depth 1
        cout << "No courses available.\n";
        return;
    }
    cout << "\n========== AVAILABLE COURSES ==========\n";
    for (const Course& course : courses) {   // depth 1
        cout << "ID:         " << course.id         << "\n";
        cout << "Name:       " << course.name        << "\n";
        cout << "Instructor: " << course.instructor  << "\n";
        cout << "Schedule:   " << course.schedule    << "\n";
        cout << "Capacity:   " << course.capacity    << " students\n";
        cout << "---------------------------------------\n";
    }
}

void searchCourses(const vector<Course>& courses, string_view keyword) {
    bool found = false;
    cout << "\n========== SEARCH RESULTS ==========\n";

    string keywordLower = string(keyword);
    transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), ::tolower);

    for (const Course& course : courses) {              // depth 1
        string nameLower       = course.name;
        string instructorLower = course.instructor;
        transform(nameLower.begin(),       nameLower.end(),       nameLower.begin(),       ::tolower);
        transform(instructorLower.begin(), instructorLower.end(), instructorLower.begin(), ::tolower);

        bool nameMatch       = nameLower.find(keywordLower)       != string::npos;
        bool instructorMatch = instructorLower.find(keywordLower) != string::npos;

        if (nameMatch || instructorMatch) {             // depth 2 — within limit
            cout << "ID:         " << course.id         << "\n";
            cout << "Name:       " << course.name        << "\n";
            cout << "Instructor: " << course.instructor  << "\n";
            cout << "Schedule:   " << course.schedule    << "\n";
            cout << "Capacity:   " << course.capacity    << " students\n";
            cout << "---------------------------------------\n";
            found = true;
        }
    }

    if (!found) {   // depth 1
        cout << "No courses found for: " << keyword << "\n";
    }
}

// ─── Sprint 2: Action handlers ────────────
void handleViewCourses(const vector<Course>& courses) {
    displayCourses(courses);
}

void handleSearchCourses(const vector<Course>& courses) {
    string keyword;
    cout << "\nEnter search keyword: ";
    getline(cin, keyword);

    if (keyword.empty()) {                          // depth 1
        cout << "ERROR: Please enter a keyword!\n";
    } else {
        searchCourses(courses, keyword);
    }
}
