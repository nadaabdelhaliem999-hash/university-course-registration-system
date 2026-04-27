#include "user.h"
#include <string_view>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// ─── Sprint 1: Validation ─────────────────
bool isValidEmail(string_view email) {
    return email.find('@') != string_view::npos;
}

bool isValidPassword(string_view password) {
    return password.length() >= 6;
}

// ─── Sprint 2: Course Functions ───────────

// Insert sample courses into the list
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

// Display all courses
void displayCourses(const vector<Course>& courses) {
    if (courses.empty()) {
        cout << "No courses available.\n";
        return;
    }
    cout << "\n========== AVAILABLE COURSES ==========\n";
    for (const Course& course : courses) {
        cout << "ID:         " << course.id         << "\n";
        cout << "Name:       " << course.name       << "\n";
        cout << "Instructor: " << course.instructor << "\n";
        cout << "Schedule:   " << course.schedule   << "\n";
        cout << "Capacity:   " << course.capacity   << " students\n";
        cout << "───────────────────────────────────────\n";
    }
}

// Search courses by name or instructor
void searchCourses(const vector<Course>& courses, string_view keyword) {
    bool found = false;

    cout << "\n========== SEARCH RESULTS ==========\n";

    for (const Course& course : courses) {
        // Convert to lowercase for case-insensitive search
        string nameLower     = course.name;
        string instructorLower = course.instructor;
        string keywordLower  = string(keyword);

        transform(nameLower.begin(),       nameLower.end(),       nameLower.begin(),       ::tolower);
        transform(instructorLower.begin(), instructorLower.end(), instructorLower.begin(), ::tolower);
        transform(keywordLower.begin(),    keywordLower.end(),    keywordLower.begin(),    ::tolower);

        // Check if keyword matches name or instructor
        if (nameLower.find(keywordLower)       != string::npos ||
            instructorLower.find(keywordLower) != string::npos) {

            cout << "ID:         " << course.id         << "\n";
            cout << "Name:       " << course.name       << "\n";
            cout << "Instructor: " << course.instructor << "\n";
            cout << "Schedule:   " << course.schedule   << "\n";
            cout << "Capacity:   " << course.capacity   << " students\n";
            cout << "───────────────────────────────────────\n";
            found = true;
        }
    }

    if (!found) {
        cout << "No courses found for: " << keyword << "\n";
    }
}
